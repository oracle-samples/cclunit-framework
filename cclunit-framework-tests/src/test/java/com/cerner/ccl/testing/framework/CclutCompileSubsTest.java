package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.net.URI;
import java.util.Collections;
import java.util.Locale;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.enums.OutputType;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;
import com.cerner.ftp.Uploader;
import com.cerner.ftp.data.factory.FileRequestFactory;

/**
 * Unit tests for the {@code cclut_compile_subs.inc} file.
 *
 * @author Joshua Hyde
 *
 */

public class CclutCompileSubsTest extends AbstractCclutTest {
    private final File incCompilerScriptFile = new File(getTestResourceDirectory(), "cclutCompileTestCaseTest.prg");
    private final String incCompilerScriptName = StringUtils.stripFilenameExtension(incCompilerScriptFile.getName());

    private final File prgCompilerScriptFile = new File(getTestResourceDirectory(), "cclutCompilePrgTest.prg");
    private final String prgCompilerScriptName = StringUtils.stripFilenameExtension(prgCompilerScriptFile.getName());

    /**
     * Compile the compiler script for each test.
     */
    @Before
    public void setUp() {
        final CclExecutor compilerExecutor = getCclExecutor();
        compilerExecutor.addScriptCompiler(incCompilerScriptFile).commit();
        compilerExecutor.addScriptCompiler(prgCompilerScriptFile).commit();
        compilerExecutor.execute();
    }

    /**
     * Drop the compiler script after each test.
     */
    @After
    public void tearDown() {
        final CclExecutor dropExecutor = getCclExecutor();
        dropExecutor.addScriptDropper(incCompilerScriptName).commit();
        dropExecutor.addScriptDropper(prgCompilerScriptName).commit();
        dropExecutor.execute();
    }

    /**
     * Test the compilation of a test INC file into a PRG.
     *
     * @throws Exception
     *             If any errors occur.
     */
    @Test
    public void testCompileTestCase() throws Exception {
        final File dummyScriptFile = new File(getTestResourceDirectory(), "cclutCompileTestCaseDummy.prg");
        final File localIncFile = new File(getTestResourceDirectory(), "cclutcompiletestcasetestinc.inc");
        final String compiledScriptName = StringUtils.stripFilenameExtension(dummyScriptFile.getName());
        final String cerTempLocation = getLogicalRetriever().getLogicalValue("CER_TEMP");

        // Upload the include file to be used
        final Uploader uploader = getUploader();
        uploader.upload(Collections.singleton(FileRequestFactory.create(localIncFile.toURI(),
                URI.create(cerTempLocation + "/" + localIncFile.getName()))));

        /*
         * Compile a dummy script that matches the name of the script to be compiled. This allows the compiled script to
         * be dropped through a CclExecutor. Additionally, compile the script that will be used to call the compilation
         * subroutine to be tested.
         */
        final CclExecutor compilerExecutor = getCclExecutor();
        compilerExecutor.addScriptCompiler(dummyScriptFile).commit();
        compilerExecutor.addScriptCompiler(incCompilerScriptFile).commit();
        compilerExecutor.execute();

        // Compile the INC file
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("incDirectory")
                .addVC("incName").addVC("listingDirectory").addVC("listingName").addVC("prgName").build());
        request.setVC("incDirectory", "cer_temp");
        request.setVC("listingDirectory", "cer_temp");
        request.setVC("listingName", "cclut_listingOutput");
        request.setVC("prgName", compiledScriptName);
        request.setVC("incName", localIncFile.getName());

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("compileResponse").addVC("errorMessage").build());

        final CclExecutor cclutCompilerExecutor = getCclExecutor();
        cclutCompilerExecutor.addScriptExecution(incCompilerScriptName).withReplace("request", request)
                .withReplace("reply", reply).commit();
        cclutCompilerExecutor.execute();
        assertThat(reply.getI2Boolean("compileResponse")).isTrue();

        // Execute the compiled script and verify its call echo()
        final ByteArrayOutputStream executeOutput = new ByteArrayOutputStream();
        final CclExecutor scriptCaller = getCclExecutor();
        scriptCaller.setOutputStream(executeOutput, OutputType.CCL_SESSION);
        scriptCaller.addScriptExecution(compiledScriptName).commit();
        scriptCaller.execute();

        assertThat(new String(executeOutput.toByteArray(), "utf-8")).contains("I am a test include file");

        // Drop all of the compiled scripts
        final CclExecutor dropExecutor = getCclExecutor();
        dropExecutor.addScriptDropper(compiledScriptName).commit();
        dropExecutor.execute();
    }

    /**
     * If compiling the test INC fails during compilation, then it should indicate that the compilation failed.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testCompileTestCaseCompileError() throws Exception {
        final File badIncFile = new File(getTestResourceDirectory(), "cclutcompiletestcasebadinc.inc");
        final String cerTempLocation = getLogicalRetriever().getLogicalValue("CER_TEMP");

        // Upload the include file to be used
        final Uploader uploader = getUploader();
        uploader.upload(Collections.singleton(FileRequestFactory.create(badIncFile.toURI(),
                URI.create(cerTempLocation + "/" + badIncFile.getName()))));

        // Try to compile the INC file
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("incDirectory")
                .addVC("incName").addVC("listingDirectory").addVC("listingName").addVC("prgName").build());
        request.setVC("incDirectory", "cer_temp");
        request.setVC("listingDirectory", "cer_temp");
        request.setVC("listingName", "cclut_listingOutput");
        request.setVC("prgName", "some_dummy_script_name");
        request.setVC("incName", badIncFile.getName());

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("compileResponse").addVC("errorMessage").build());

        final CclExecutor cclutCompilerExecutor = getCclExecutor();
        cclutCompilerExecutor.addScriptExecution(incCompilerScriptName).withReplace("request", request)
                .withReplace("reply", reply).commit();
        cclutCompilerExecutor.execute();
        assertThat(reply.getI2Boolean("compileResponse")).isFalse();
    }

    /**
     * Compiling when the specified include file cannot be found should fail.
     *
     * @throws Exception
     *             If any errors occur.
     */
    @Test
    public void testCompileTestCaseFileNotFound() throws Exception {
        // Compile a non-existent INC file
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("incDirectory")
                .addVC("incName").addVC("listingDirectory").addVC("listingName").addVC("prgName").build());
        request.setVC("incDirectory", "cer_temp");
        request.setVC("listingDirectory", "cer_temp");
        request.setVC("listingName", "cclut_listingOutput");
        request.setVC("prgName", "dummy_script_name");
        request.setVC("incName", "the_name_of_an_include_file_that_will_never_ever_exist_i_hope.inc");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("compileResponse").addVC("testCaseDirectory").addVC("errorMessage")
                        .build());

        final CclExecutor subInvokerExecutor = getCclExecutor();
        subInvokerExecutor.addScriptExecution(incCompilerScriptName).withReplace("reply", reply)
                .withReplace("request", request).commit();
        subInvokerExecutor.execute();
        assertThat(reply.getI2Boolean("compileResponse")).isFalse();

        assertThat(reply.getVC("errorMessage"))
                .isEqualTo("the_name_of_an_include_file_that_will_never_ever_exist_i_hope.inc not found in cer_temp");
    }

    /**
     * Test the compilation of a PRG file.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testCompilePrgFile() throws Exception {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutCompileTestCaseDummy.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());
        final String cerTempLocation = getLogicalRetriever().getLogicalValue("CER_TEMP");

        /*
         * Compile and drop the script to make sure it's registered, allowing for it to be dropped later.
         */
        final CclExecutor createAndDropExecutor = getCclExecutor();
        createAndDropExecutor.addScriptCompiler(scriptFile).commit();
        createAndDropExecutor.addScriptDropper(scriptName).commit();
        createAndDropExecutor.execute();

        // Upload the script to be compiled
        final Uploader scriptUploader = getUploader();
        scriptUploader.upload(Collections.singleton(FileRequestFactory.create(scriptFile.toURI(),
                URI.create(cerTempLocation + "/" + scriptFile.getName().toLowerCase(Locale.getDefault())))));

        // Invoke a script to compile the uploaded script
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("listingDirectory")
                .addVC("listingName").addVC("prgDirectory").addVC("prgName").build());
        request.setVC("listingDirectory", "cer_temp");
        request.setVC("listingName", "cclut_listingOutput");
        request.setVC("prgDirectory", "cer_temp");
        request.setVC("prgName", scriptFile.getName().toLowerCase(Locale.getDefault()));

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("compileResponse").addVC("errorMessage").build());

        final CclExecutor compileExecutor = getCclExecutor();
        compileExecutor.addScriptExecution(prgCompilerScriptName).withReplace("reply", reply)
                .withReplace("request", request).commit();
        compileExecutor.execute();
        assertThat(reply.getI2Boolean("compileResponse")).isTrue();

        // Verify that the script actually exists in the dictionary
        final ByteArrayOutputStream output = new ByteArrayOutputStream();
        final CclExecutor scriptExecutor = getCclExecutor();
        scriptExecutor.addScriptExecution(scriptName).commit();
        scriptExecutor.setOutputStream(output, OutputType.CCL_SESSION);
        scriptExecutor.execute();

        assertThat(new String(output.toByteArray(), "utf-8")).contains("I shouldn't be displayed");

        // Drop the compile script
        final CclExecutor dropExecutor = getCclExecutor();
        dropExecutor.addScriptDropper(scriptName).commit();
        dropExecutor.execute();
    }

    /**
     * Test that, if compilation fails, the response by the script compiler indicates as much.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testCompilePrgFileCompileError() throws Exception {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutCompilePrgTestBadPrg.prg");
        final String cerTempLocation = getLogicalRetriever().getLogicalValue("CER_TEMP");

        // Upload the script to be compiled
        final Uploader scriptUploader = getUploader();
        scriptUploader.upload(Collections.singleton(FileRequestFactory.create(scriptFile.toURI(),
                URI.create(cerTempLocation + "/" + scriptFile.getName().toLowerCase(Locale.getDefault())))));

        // Try to compile it, but it will fail!
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("listingDirectory")
                .addVC("listingName").addVC("prgDirectory").addVC("prgName").build());
        request.setVC("listingDirectory", "cer_temp");
        request.setVC("listingName", "cclut_listingOutput");
        request.setVC("prgDirectory", "cer_temp");
        request.setVC("prgName", scriptFile.getName().toLowerCase(Locale.getDefault()));

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("compileResponse").addVC("errorMessage").build());

        final CclExecutor compileExecutor = getCclExecutor();
        compileExecutor.addScriptExecution(prgCompilerScriptName).withReplace("reply", reply)
                .withReplace("request", request).commit();
        compileExecutor.execute();
        assertThat(reply.getI2Boolean("compileResponse")).isFalse();
    }

    /**
     * If an attempt is made to compile a non-existent file, then the compilation attempt should fail appropriately.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testCompilePrgFileNotFound() throws Exception {
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("listingDirectory")
                .addVC("listingName").addVC("prgDirectory").addVC("prgName").build());
        request.setVC("listingDirectory", "cer_temp");
        request.setVC("listingName", "cclut_listingOutput");
        request.setVC("prgDirectory", "cer_temp");
        request.setVC("prgName", "some_file_that_will_never_ever_exist.prg");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("compileResponse").addVC("programDirectory").addVC("errorMessage")
                        .build());

        final CclExecutor compileExecutor = getCclExecutor();
        compileExecutor.addScriptExecution(prgCompilerScriptName).withReplace("reply", reply)
                .withReplace("request", request).commit();
        compileExecutor.execute();
        assertThat(reply.getI2Boolean("compileResponse")).isFalse();

        assertThat(reply.getVC("errorMessage"))
                .isEqualTo("some_file_that_will_never_ever_exist.prg not found in cer_temp");
    }
}
