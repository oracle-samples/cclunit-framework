package com.cerner.ccl.testing.framework;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.List;

import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Unit tests for the {@code cclut_xml_access_subs.inc} file.
 * 
 * @author Joshua Hyde
 * @author Nicholas Feldmann
 * 
 */

public class CclutXmlAccessSubsTest extends AbstractCclutTest {
    private final String XMLString = "<TEST><TAGS><TAG><VALUE text='value1'>TAG1</VALUE>"
            + "</TAG><TAG><VALUE text='VALUE2'>TAG2</VALUE>" + "</TAG><TAG><VALUE/>" + "</TAG></TAGS></TEST>";

    /**
     * Tests the cclutParseXMLBuffer subroutine
     */
    @Test
    public void testParseXMLBuffer() {
        // Script file and name
        final File scriptFile = new File(getTestResourceDirectory(), "cclutParseBufferTest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("buffer").addI4("fileHandle").build());

        dt_request.setVC("buffer", XMLString);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addI4("root").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_request", dt_request).withReplace("dt_reply", dt_reply)
                .commit();

        executor.execute();

        assertTrue("Root is 0", dt_reply.getI4("root") != 0);
        assertTrue("File handle is 0", dt_request.getI4("fileHandle") != 0);
    }

    /**
     * Test for the cclutGetChildElementOccurrenceHandle subroutine
     */
    @Test
    public void testGetChildElementOccurance() {

        final File scriptFile = new File(getTestResourceDirectory(), "cclutGetChildElementTest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request", StructureBuilder.getBuilder().addVC("xmlBuffer")
                .addI4("elementHandle").addVC("childName").addI4("index").build());

        dt_request.setVC("childName", "TEST");
        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setI4("index", 1);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addI4("handle").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();

        executor.execute();

        assertTrue("Handle is 0", dt_reply.getI4("handle") != 0);
    }

    /**
     * Test for the cclutGetAttributeValue subroutine
     */
    @Test
    public void testGetAttributeValue() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutGetAttributeValTest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("xmlBuffer").addVC("childName").addVC("child2Name")
                        .addVC("child3Name").addVC("rootName").addVC("attrName").addI4("index").build());

        dt_request.setVC("rootName", "TEST");
        dt_request.setVC("childName", "TAGS");
        dt_request.setVC("child2Name", "TAG");
        dt_request.setVC("child3Name", "VALUE");
        dt_request.setVC("attrName", "text");
        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setI4("index", 1);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addI4("listSize")
                .addDynamicList("replys", StructureBuilder.getBuilder().addVC("val").build()).build());

        dt_reply.setI4("listSize", 3);

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();

        executor.execute();

        assertEquals("Not enough replys", dt_reply.getDynamicList("replys").getAll().size(),
                dt_reply.getI4("listSize"));

        // Set the expected values
        String[] attributes = new String[dt_reply.getI4("listSize")];
        attributes[0] = "value1";
        attributes[1] = "VALUE2";
        attributes[2] = "";

        List<Record> replyList = dt_reply.getDynamicList("replys").getAll();

        for (int i = 0; i < replyList.size(); i++) {
            assertTrue("Value is incorrect", replyList.get(i).getVC("val").equals(attributes[i]));
        }
    }

    /**
     * Test for the cclutGetChildNodeValue subroutine
     */
    @Test
    public void testGetChildNodeValue() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutGetChildValueTest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request", StructureBuilder.getBuilder().addVC("xmlBuffer")
                .addVC("childName").addVC("child2Name").addVC("child3Name").addVC("rootName").addI4("index").build());

        dt_request.setVC("rootName", "TEST");
        dt_request.setVC("childName", "TAGS");
        dt_request.setVC("child2Name", "TAG");
        dt_request.setVC("child3Name", "VALUE");
        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setI4("index", 1);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("val").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();
        //
        executor.execute();

        assertTrue("Value is incorrect", dt_reply.getVC("val").equals("TAG1"));
    }

    /**
     * Test for the cclutGetChildNodeAttributeValue subroutine
     */
    @Test
    public void testGetChildAttributeValue() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutGetChildAttrValTest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("xmlBuffer").addVC("childName").addVC("child2Name")
                        .addVC("child3Name").addVC("rootName").addI4("index").addVC("attrName").build());

        dt_request.setVC("rootName", "TEST");
        dt_request.setVC("childName", "TAGS");
        dt_request.setVC("child2Name", "TAG");
        dt_request.setVC("child3Name", "VALUE");
        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setVC("attrName", "text");
        dt_request.setI4("index", 1);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("val").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();
        executor.execute();

        assertTrue("Value is incorrect", dt_reply.getVC("val").equals("value1"));
    }

    /**
     * Test for the cclutWriteXMLElement subroutine
     */
    @Test
    public void testWriteXMLElement() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutWriteXMLtest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("xmlBuffer").addVC("rootName").addI4("index").build());

        dt_request.setVC("rootName", "TEST");
        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setI4("index", 1);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("val").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();
        executor.execute();

        String temp = XMLString.replace('\'', '"');

        assertTrue("Value is incorrect", dt_reply.getVC("val").equals(temp));
    }

    /**
     * Test for the cclutGetElementValue subroutine
     */
    @Test
    public void testGetElementValue() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutGetElementValTest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request", StructureBuilder.getBuilder().addVC("xmlBuffer")
                .addVC("childName").addVC("child2Name").addVC("child3Name").addVC("rootName").addI4("index").build());

        dt_request.setVC("rootName", "TEST");
        dt_request.setVC("childName", "TAGS");
        dt_request.setVC("child2Name", "TAG");
        dt_request.setVC("child3Name", "VALUE");
        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setI4("index", 1);

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("val").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();
        executor.execute();

        assertTrue("Value is incorrect", dt_reply.getVC("val").equals("TAG1"));
    }

    /**
     * Test for the cclutGetXPathValue subroutine
     */
    @Test
    public void testGetXPathValue() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutGetXPathtest.prg");

        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Record dt_request = RecordFactory.create("dt_request",
                StructureBuilder.getBuilder().addVC("xmlBuffer").addVC("expr").build());

        dt_request.setVC("xmlBuffer", XMLString);
        dt_request.setVC("expr", "/TEST/TAGS/TAG[2]/VALUE/@text");

        final Record dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addVC("val").build());

        // Compile and execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("dt_reply", dt_reply).withReplace("dt_request", dt_request)
                .commit();
        executor.execute();

        assertTrue("Wrong value returned", dt_reply.getVC("val").equals("VALUE2"));
    }
}
