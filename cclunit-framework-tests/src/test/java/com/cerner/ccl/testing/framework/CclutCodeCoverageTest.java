package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;
import static org.junit.Assert.assertEquals;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.net.URI;
import java.util.Collections;
import java.util.Locale;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.DataType;
import com.cerner.ccl.j4ccl.record.Field;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.Structure;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;
import com.cerner.ftp.Uploader;
import com.cerner.ftp.data.factory.FileRequestFactory;

/**
 * Unit tests for the {@code cclut_code_coverage.inc} file.
 *
 * @author Joshua Hyde
 * @author Nicholas Feldmann
 *
 */

public class CclutCodeCoverageTest extends AbstractCclutTest {
    /**
     * Test to show correct response is returned when there is no code coverage
     */
    @Test
    public void testNoCoverage() {
        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addVC("xml").build());

        final CclExecutor executor = getCclExecutor();

        executor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutTestXmlGetNoCoverage.prg")).commit();
        executor.addScriptExecution("cclutTestXmlGetNoCoverage").withReplace("REPLY", reply).commit();
        executor.addScriptDropper("cclutTestXmlGetNoCoverage").commit();
        executor.execute();

        assertEquals("Reply is incorrect", reply.getVC("xml"), "");
    }

    /**
     * Tests that correct coverage XML blocks are returned
     *
     * @throws IOException
     *             Not expected.
     */
    @Test
    public void testFilterCoverageXml() throws IOException {
        String dummyPrg1 = "TESTER1";
        String dummyPrg2 = "TESTER2";

        // CCL file
        final File scriptFile = new File(getTestResourceDirectory(), "cclutTestFilterCoverage.prg");

        final String scriptFileName = StringUtils.stripFilenameExtension(scriptFile.getName());

        // Compile test script
        final CclExecutor compiler = getCclExecutor();
        compiler.addScriptCompiler(scriptFile).commit();
        compiler.execute();

        // Create request record structure
        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("coverage_xml").build());

        // Create programs record structure
        final Record dtPrograms = RecordFactory.create("dtPrograms",
                StructureBuilder.getBuilder()
                        .addDynamicList("programs",
                                StructureBuilder.getBuilder().addVC("programName").addVC("coverageXml").build())
                        .build());

        // Populate the program names
        dt_request.setVC("coverage_xml", "<Lines>TEST</Lines>");
        dtPrograms.getDynamicList("programs").addItem().setVC("programName", dummyPrg1);
        dtPrograms.getDynamicList("programs").addItem().setVC("programName", dummyPrg2);

        // Expected return values
        String[] xml = new String[2];
        xml[0] = "<COVERAGE><COVERAGE_NAME>" + dummyPrg1 + "</COVERAGE_NAME></COVERAGE>";
        xml[1] = "<COVERAGE><COVERAGE_NAME>" + dummyPrg2 + "</COVERAGE_NAME></COVERAGE>";

        // Execute the script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptFileName).withReplace("dt_request", dt_request)
                .withReplace("dtPrograms", dtPrograms).commit();
        executor.execute();

        for (int i = 0; i < dtPrograms.getDynamicList("programs").getAll().size(); i++) {
            assertThat(dtPrograms.getDynamicList("programs").get(i).getVC("coverageXml")).isNotEmpty();
            assertEquals("XML not correct", dtPrograms.getDynamicList("programs").get(i).getVC("coverageXml"), xml[i]);
        }
    }

    /**
     * Make sure that the database version is not truncated by the testing framework.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testGetEnvironmentDataXml() throws Exception {
        final String getDbVersionScriptName = "cclutGetEnvironmentXml";

        final Structure envXmlStructure = StructureBuilder.getBuilder()
                .addRecord("actual", StructureBuilder.getBuilder().addVC("currdb").addVC("currdbname")
                        .addVC("currdbuser").addVC("currdbsys").addI4("ccl_major_version").addI4("ccl_minor_version")
                        .addI4("ccl_revision").addVC("cursys").addVC("curlocale").addVC("curuser").addI4("curutc")
                        .addI4("curutcdiff").addChar("curtimezone", 30).addI4("curtimezoneapp").addI4("curtimezonesys")
                        .addI4("currevafd").addI4("curgroup").addVC("dboptmode").addVC("dbversion").build())
                .addVC("retrievedXml").build();
        final Record reply = RecordFactory.create("envXmlReply", envXmlStructure);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), getDbVersionScriptName + ".prg")).commit();
        executor.addScriptExecution(getDbVersionScriptName).withReplace("reply", reply).commit();
//        executor.addScriptDropper(getDbVersionScriptName).commit();
        executor.execute();

        final Document document = new SAXBuilder().build(new StringReader(reply.getVC("retrievedXml")));
        final Element envElement = document.getRootElement();
        assertThat(envElement.getName()).isEqualTo("ENVIRONMENT");

        final Record actual = reply.getRecord("actual");
        for (Field field : actual.getStructure().getFields()) {
            if (field.getType() == DataType.VC)
                assertThat(envElement.getChild(field.getName().toUpperCase(Locale.getDefault())).getText())
                        .as("Field [" + field.getName() + "] does not match.").isEqualTo(actual.getVC(field.getName()));
            else if (field.getType() == DataType.I4) {
                // There's a special comparison for CCL versions
                if (!field.getName().startsWith("ccl_"))
                    assertThat(envElement.getChild(field.getName().toUpperCase(Locale.getDefault())).getText())
                            .as("Field [" + field.getName() + "] does not match.")
                            .isEqualTo(Integer.toString(actual.getI4(field.getName())));
            }
        }

        assertThat(envElement.getChild("CCLVER").getText()).isEqualTo(actual.getI4("ccl_major_version") + "."
                + actual.getI4("ccl_minor_version") + "." + actual.getI4("ccl_revision"));
    }

    /**
     * Test the retrieval of the listing output from compilation as XML.
     */
    @Test
    public void testGetListing() {
        String name = "cclutlisttest";
        String path = getLogicalRetriever().getLogicalValue("CER_TEMP") + "/";
        String filename = "cclutListTest";

        final File scriptTestFile = new File(getTestResourceDirectory(), "cclutCCListTest.prg");

        final String scriptTestName = StringUtils.stripFilenameExtension(scriptTestFile.getName());

        final CclExecutor createAndDropExecutor = getCclExecutor();
        createAndDropExecutor.addScriptCompiler(scriptTestFile).commit();
        createAndDropExecutor.addScriptDropper(scriptTestName).commit();
        createAndDropExecutor.execute();

        // Upload the script to be compiled
        final Uploader scriptUploader = getUploader();
        scriptUploader.upload(Collections.singleton(FileRequestFactory.create(scriptTestFile.toURI(),
                URI.create(path + "/" + scriptTestFile.getName().toLowerCase(Locale.getDefault())))));

        final File scriptFile = new File(getTestResourceDirectory(), "cclutCCGetListing.prg");

        final String scriptFileName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_build = RecordFactory.create("dt_build", StructureBuilder.getBuilder().addVC("listingDirectory")
                .addVC("listingName").addVC("prgDirectory").addVC("prgName").build());
        dt_build.setVC("listingDirectory", "cer_temp");
        dt_build.setVC("listingName", "cclutListTest");
        dt_build.setVC("prgDirectory", "cer_temp");
        dt_build.setVC("prgName", scriptTestFile.getName().toLowerCase(Locale.getDefault()));

        // Compile test script
        final CclExecutor compiler = getCclExecutor();
        compiler.addScriptCompiler(scriptFile).commit();
        compiler.execute();

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("xml").build());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("program_name").addVC("path").addVC("filename").build());

        dt_request.setVC("program_name", name);
        dt_request.setVC("path", path);
        dt_request.setVC("filename", filename);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptFileName).withReplace("dt_reply", dt_reply)
                .withReplace("dt_request", dt_request).withReplace("dt_build", dt_build).commit();
        executor.execute();

        assertThat(dt_reply.getVC("xml")).isNotEmpty();
    }

    /**
     * Test the retrieval of the include file compilation out as XML.
     */
    @Test
    public void testGetTestCaseListing() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutCCGetTestCaseListing.prg");

        final String scriptFileName = StringUtils.stripFilenameExtension(scriptFile.getName());

        // Compile test script
        final CclExecutor compiler = getCclExecutor();
        compiler.addScriptCompiler(scriptFile).commit();
        compiler.execute();

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("xml").build());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("program_name").addVC("path").addVC("filename").build());

        dt_request.setVC("program_name", "Test");
        dt_request.setVC("path", "Tester");
        dt_request.setVC("filename", "Testing");

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptFileName).withReplace("dt_reply", dt_reply)
                .withReplace("dt_request", dt_request).commit();
        executor.execute();

        assertThat(dt_reply.getVC("xml")).isNotEmpty();
    }

    /**
     * Test the retrieval of code coverage for an include file.
     */
    @Test
    public void testGetTestCaseCoverage() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutCCGetTestCaseCoverage.prg");

        final String scriptFileName = StringUtils.stripFilenameExtension(scriptFile.getName());

        // Compile test script
        final CclExecutor compiler = getCclExecutor();
        compiler.addScriptCompiler(scriptFile).commit();
        compiler.execute();

        // Reply and request structures
        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("xml").build());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("coverage_xml").addVC("program_name").addVC("listing").build());

        String coverageXml = "<COVERAGES>" + "<COVERAGE><COVERAGE_NAME>PROGRAM_A</COVERAGE_NAME><LINES>"
                + "<LINE><NBR>1</NBR><TYPE>U</TYPE></LINE><LINE><NBR>2</NBR><TYPE>C</TYPE></LINE><LINE><NBR>3</NBR><TYPE>N</TYPE></LINE>"
                + "</LINES></COVERAGE>" + "<COVERAGE><COVERAGE_NAME>PROGRAM_B</COVERAGE_NAME><LINES>"
                + "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE><LINE><NBR>2</NBR><TYPE>C</TYPE></LINE><LINE><NBR>3</NBR><TYPE>U</TYPE></LINE>"
                + "<LINE><NBR>4</NBR><TYPE>C</TYPE></LINE><LINE><NBR>5</NBR><TYPE>C</TYPE></LINE><LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>"
                + "</LINES></COVERAGE>" + "<COVERAGE><COVERAGE_NAME>PROGRAM_C</COVERAGE_NAME><LINES>"
                + "<LINE><NBR>1</NBR><TYPE>U</TYPE></LINE><LINE><NBR>2</NBR><TYPE>N</TYPE></LINE><LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>"
                + "</LINES></COVERAGE>" + "</COVERAGES>";
        String coverageXmlB = "<COVERAGE><COVERAGE_NAME>PROGRAM_B</COVERAGE_NAME><LINES>"
                + "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE><LINE><NBR>2</NBR><TYPE>C</TYPE></LINE><LINE><NBR>3</NBR><TYPE>U</TYPE></LINE>"
                + "<LINE><NBR>4</NBR><TYPE>C</TYPE></LINE><LINE><NBR>5</NBR><TYPE>C</TYPE></LINE><LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>"
                + "</LINES></COVERAGE>";
        String listingXml = "<LISTING><LISTING_NAME>PROGRAM_B</LISTING_NAME><COMPILE_DATE>02-DEC-2017 03:24:17</COMPILE_DATE>"
                + "<LINES>" + "<LINE><NBR>1</NBR><TEXT>line 1 text</TEXT></LINE>"
                + "<LINE><NBR>2</NBR><TEXT>line 2 text</TEXT></LINE>"
                + "<LINE><NBR>3</NBR><TEXT>line 3 text</TEXT></LINE>"
                + "<LINE><NBR>4</NBR><TEXT>line 4 text</TEXT></LINE>"
                + "<LINE><NBR>5</NBR><TEXT>line 5 text</TEXT></LINE>"
                + "<LINE><NBR>6</NBR><TEXT>line 6 text</TEXT></LINE>" + "</LINES></LISTING>";

        dt_request.setVC("coverage_xml", coverageXml);
        dt_request.setVC("program_name", "program_b");
        dt_request.setVC("listing", listingXml);


        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptFileName).withReplace("dt_reply", dt_reply)
                .withReplace("dt_request", dt_request).commit();
        executor.execute();

        assertThat(dt_reply.getVC("xml")).isNotEmpty();

        System.out.println(dt_reply.getVC("xml"));
        assertThat(dt_reply.getVC("xml")).isEqualTo(coverageXmlB);
    }
}
