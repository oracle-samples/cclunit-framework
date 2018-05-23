package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.io.StringReader;
import java.util.LinkedList;
import java.util.List;

import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.junit.Before;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.Structure;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;
import com.cerner.ccl.j4ccl.util.CclResourceUploader;
import com.cerner.ccl.testing.framework.internal.jaxb.TESTCASE;
import com.cerner.ccl.testing.framework.internal.jaxb.TESTCASE.TESTS.TEST;
import com.cerner.ccl.testing.framework.internal.jaxb.TESTCASE.TESTS.TEST.ASSERTS.ASSERT;
import com.cerner.ccl.testing.framework.internal.jaxb.TESTCASE.TESTS.TEST.ERRORS.ERROR;

/**
 * Tests for the {@code cclut_execute_test_case.prg} script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutExecuteTestCaseTest extends AbstractCclutTest {
    private static final Logger LOGGER = LoggerFactory.getLogger(CclutExecuteTestCaseTest.class);

    @Autowired
    @Qualifier("xsdDirectory")
    private String xsdDirectory;

    private Record request;
    private Record reply;

    /**
     * Set up the test request and reply.
     */
    @Before
    public void setUp() {
        final Structure programsRequestStructure = StructureBuilder.getBuilder().addVC("programName").addI2("compile")
                .build();
        final Structure requestStructure = StructureBuilder.getBuilder().addVC("testINCName")
                .addDynamicList("programs", programsRequestStructure).build();
        request = RecordFactory.create("request", requestStructure);

        final Structure programsReplyStructure = StructureBuilder.getBuilder().addVC("programName").addVC("listingXML")
                .addVC("coverageXML").build();
        final Structure replyStructure = StructureBuilder.getBuilder().addVC("environmentXML")
                .addVC("testINCListingXML").addVC("testINCCoverageXML").addVC("testINCResultsXML")
                .addDynamicList("programs", programsReplyStructure).addStatusData().build();
        reply = RecordFactory.create("reply", replyStructure);
    }

    /**
     * If the provided include file has bad syntax that prevents a successful compilation, the testing framework should
     * explicitly fail out.
     * <p />
     * This test only passes on pre-8.7.2 due to changes made to call compile() to error out. As such, it will not
     * execute itself if the version of CCL against which it runs is 8.7.2 or higher.
     */
    @Test
    public void testBadSyntaxIncludeFile() {
        {
            final String scriptName = "cclutCompareCclVersion";
            final CclExecutor versionCheckExecutor = getCclExecutor();

            final Record compareRequest = RecordFactory.create("compareRequest",
                    StructureBuilder.getBuilder().addVC("to_compare").build());
            compareRequest.setVC("to_compare", "8.7.2");

            final Record compareReply = RecordFactory.create("compareReply",
                    StructureBuilder.getBuilder().addI2("less_than_ind").addVC("current_version").build());

            versionCheckExecutor.addScriptCompiler(new File(getTestResourceDirectory(), scriptName + ".prg")).commit();
            versionCheckExecutor.addScriptExecution(scriptName).withReplace("request", compareRequest)
                    .withReplace("reply", compareReply).commit();
            versionCheckExecutor.addScriptDropper(scriptName).commit();
            versionCheckExecutor.execute();

            if (!compareReply.getI2Boolean("less_than_ind")) {
                LOGGER.info(
                        "The current CCL version is {}, which is too high for testBadSyntaxIncludeFile; this test will not be run.",
                        compareReply.getVC("current_version"));
                return;
            }
        }

        final String includeFileName = "cclut_invalid_syntax_test.inc";
        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), includeFileName));
        uploader.upload();

        setRequestTestInc(includeFileName);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("F");

        final Record subeventStatus = statusData.getList("subeventstatus").get(0);
        assertThat(subeventStatus.getChar("OperationName").trim()).isEqualTo("Compiling test");
        assertThat(subeventStatus.getChar("OperationStatus")).isEqualTo("F");
        assertThat(subeventStatus.getChar("TargetObjectName").trim()).isEqualTo("CCLUT_EXECUTE_TEST_CASE");
        assertThat(subeventStatus.getVC("TargetObjectValue"))
                .matches("Failed to find wrapper script 'prg_[0-9]+' for include file " + includeFileName
                        + " in CCL dictionary following compilation; verify that it is syntactically-valid CCL code\\.");
    }

    /**
     * Test the execution of a test with a failed assertion.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testFailWithAssertionFailure() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_with_assertion_failure.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        // Testing framework should not fail, because the test failed, not the testing framework itself
        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTWITHASSERTIONFAILURE");
        assertThat(test.getRESULT()).isEqualTo("FAILED");

        final List<ASSERT> assertions = test.getASSERTS().getASSERT();
        assertThat(assertions).hasSize(1);

        final ASSERT assertion = assertions.get(0);
        assertThat(assertion.getCONTEXT()).isEqualTo("this should fail");
        assertThat(assertion.getRESULT()).isEqualTo("FAILED");
        assertThat(assertion.getTEST()).isEqualTo("1=0");
    }

    /**
     * If the test causes a runtime error, the test execution should fail (but not the framework itself).
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testFailWithTestRuntimeError() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_with_runtime_error.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        // Testing framework should not fail, because the test errored, not the testing framework itself
        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTWITHRUNTIMEERROR");
        assertThat(test.getRESULT()).isEqualTo("ERRORED");

        assertThat(test.getASSERTS().getASSERT()).isEmpty();

        final List<ERROR> errors = test.getERRORS().getERROR();
        assertThat(errors).hasSize(1);
        assertThat(errors.get(0).getERRORTEXT())
                .contains("{SUBROUTINEDOESNOTEXIST()}Unknown Function (SUBROUTINEDOESNOTEXIST) encountered.");
    }

    /**
     * If the request sets the "enforcePredeclare" indicator, then a test script that uses an undeclared variable should
     * fail.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testExecuteWithPredeclare() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_without_declare.inc";

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final Structure withEnforcePredeclare = StructureBuilder.getBuilder(request.getStructure())
                .addI2("enforcePredeclare").build();
        final Record withEnforcePredeclareRequest = RecordFactory.create("enforcePredeclareRequest",
                withEnforcePredeclare);
        withEnforcePredeclareRequest.setI2("enforcePredeclare", true);
        addRequestProgram(programName, withEnforcePredeclareRequest);
        setRequestTestInc(testIncName, withEnforcePredeclareRequest);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", withEnforcePredeclareRequest)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTWITHOUTDECLARE");
        assertThat(test.getRESULT()).isEqualTo("ERRORED");

        assertThat(test.getASSERTS().getASSERT()).isEmpty();

        final List<ERROR> errors = test.getERRORS().getERROR();
        assertThat(errors).hasSize(1);
    }

    /**
     * If the request does not provide the optional "enforcePredeclare" indicator, then a test script that uses an
     * undeclared variable should fail.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testExecuteWithoutPredeclare() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testCaseName = "cclut_test_without_declare.inc";

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testCaseName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final Structure withoutEnforcePredeclare = StructureBuilder.getBuilder(request.getStructure()).build();
        final Record withoutEnforcePredeclareRequest = RecordFactory.create("withoutEnforcePredeclareRequest",
                withoutEnforcePredeclare);
        addRequestProgram(programName, withoutEnforcePredeclareRequest);
        setRequestTestInc(testCaseName, withoutEnforcePredeclareRequest);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case")
                .withReplace("cclutRequest", withoutEnforcePredeclareRequest).withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testCaseName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTWITHOUTDECLARE");
        assertThat(test.getRESULT()).isEqualTo("ERRORED");

        assertThat(test.getASSERTS().getASSERT()).isEmpty();

        final List<ERROR> errors = test.getERRORS().getERROR();
        assertThat(errors).hasSize(1);
    }

    /**
     * If the request sets the "enforcePredeclare" indicator to false, then a test script that uses an undeclared
     * variable should not fail.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testExecuteWithPredeclareFalse() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testCaseName = "cclut_test_without_declare.inc";

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testCaseName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final Structure withEnforcePredeclareFalse = StructureBuilder.getBuilder(request.getStructure())
                .addI2("enforcePredeclare").build();
        final Record withEnforcePredeclareFalseRequest = RecordFactory.create("enforcePredeclareFalseRequest",
                withEnforcePredeclareFalse);
        withEnforcePredeclareFalseRequest.setI2("enforcePredeclare", false);
        addRequestProgram(programName, withEnforcePredeclareFalseRequest);
        setRequestTestInc(testCaseName, withEnforcePredeclareFalseRequest);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case")
                .withReplace("cclutRequest", withEnforcePredeclareFalseRequest).withReplace("cclutReply", reply)
                .commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testCaseName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTWITHOUTDECLARE");
        assertThat(test.getRESULT()).isEqualTo("PASSED");

        assertThat(test.getASSERTS().getASSERT()).isEmpty();
        assertThat(test.getERRORS().getERROR()).isEmpty();
    }

    /**
     * Verify that a user can specify a pattern for the names of the unit tests to be executed.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testExecuteWithTestSubroutineName() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_filter_sub_name.inc";

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final Structure testStructure = StructureBuilder.getBuilder(request.getStructure()).addVC("testSubroutineName")
                .build();
        final Record testRecord = RecordFactory.create("testSubroutineNameRequest", testStructure);
        testRecord.setVC("testSubroutineName", "dorun");
        addRequestProgram(programName, testRecord);
        setRequestTestInc(testIncName, testRecord);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", testRecord)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTDORUN");
    }

    /**
     * Verify that the name of the currently running test name can be retrieved during the setup, execution, and
     * teardown of the test.
     */
    @Test
    public void testGetTestName() {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String expectedTestName1 = "TESTGETTESTNAME1";
        final String expectedTestName2 = "TESTGETTESTNAME2";
        final String testIncName = "cclut_test_get_test_name.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final Record testReply = RecordFactory.create("getTestNameReply",
                StructureBuilder.getBuilder().addVC("setupTestName").addVC("teardownTestName")
                        .addVC("testExecutionTestName1").addVC("testExecutionTestName2").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).withReplace(testReply.getName(), testReply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        assertThat(testReply.getVC("setupTestName")).isEqualTo(expectedTestName1 + ";" + expectedTestName2);
        assertThat(testReply.getVC("teardownTestName")).isEqualTo(expectedTestName1 + ";" + expectedTestName2);
        assertThat(testReply.getVC("testExecutionTestName1")).isEqualTo(expectedTestName1);
        assertThat(testReply.getVC("testExecutionTestName2")).isEqualTo(expectedTestName2);
    }

    /**
     * Test the execution of a test with all-successful assertions.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testSucceedWithAssertion() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_with_assertion_success.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        // Testing framework should not fail, because the test failed, not the testing framework itself
        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);
        final TEST test = tests.get(0);
        assertThat(test.getNAME()).isEqualTo("TESTWITHASSERTIONSUCCESS");
        assertThat(test.getRESULT()).isEqualTo("PASSED");

        final List<ASSERT> assertions = test.getASSERTS().getASSERT();
        assertThat(assertions).hasSize(1);

        final ASSERT assertion = assertions.get(0);
        assertThat(assertion.getCONTEXT()).isEqualTo("this should succeed");
        assertThat(assertion.getRESULT()).isEqualTo("PASSED");
        assertThat(assertion.getTEST()).isEqualTo("1=1");
    }

    /**
     * Verify that the XML in the response from the CCL Testing Framework adheres to the schema when an error occurs
     * during the run.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testValidXmlErrored() throws Exception {
        validateXml("cclut_test_error_script.inc", "cclutErrorScript");
    }

    /**
     * Verify that the XML in the response from the CCL Testing Framework adheres to the schema when an assertion fails
     * during the run.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testValidXmlFailed() throws Exception {
        validateXml("cclut_test_fail.inc", "cclutDummyTest");
    }

    /**
     * Verify that the XML in the response from the CCL Testing Framework adheres to the schema when an assertion fails
     * during the run.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testValidXmlSucceeded() throws Exception {
        validateXml("cclut_test_succeed.inc", "cclutDummyTest");
    }

    /**
     * If a compiled script that is tested contains an include file, validate the listing XML follows the schema.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testValidXmlWithCompiledInclude() throws Exception {
        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), "cclut_dummy_test_include.inc"));
        uploader.upload();

        validateXml("cclut_test_with_include.inc", "cclutDummyTestWithInc");
    }

    /**
     * Add a program that is to be tested to the default request record structure.
     *
     * @param programName
     *            The name of the CCL program (without the {@code .prg} file extension) that is to be tested.
     */
    private void addRequestProgram(final String programName) {
        addRequestProgram(programName, request);
    }

    /**
     * Add a program that is to be tested.
     *
     * @param programName
     *            The name of the CCL program (without the {@code .prg} file extension) that is to be tested.
     * @param request
     *            The {@link Record} to which the program name is to be added.
     */
    private void addRequestProgram(final String programName, final Record request) {
        final Record programRequest = request.getDynamicList("programs").addItem();
        programRequest.setVC("programName", programName);
        programRequest.setI2("compile", true);
    }

    /**
     * Get the location of the directory containing the XSD files.
     *
     * @return The location of the directory containing the XSD files.
     */
    private String getXsdDirectory() {
        // Maven escapes the colon in the file path - fix that
        return xsdDirectory.replaceAll("\\\\:", ":");
    }

    /**
     * Set the test {@code .inc} file that is to be run by the test framework in the default request structure.
     *
     * @param testIncName
     *            The name (including the {@code .inc} file extension) to be run by the testing framework.
     */
    private void setRequestTestInc(final String testIncName) {
        setRequestTestInc(testIncName, request);
    }

    /**
     * Set the test {@code .inc} file that is to be run by the test framework for the given request record structure.
     *
     * @param testIncName
     *            The name (including the {@code .inc} file extension) to be run by the testing framework.
     * @param request
     *            The {@link Record} whose test INC name is to be set.
     */
    private void setRequestTestInc(final String testIncName, final Record request) {
        request.setVC("testINCName", testIncName);
    }

    /**
     * Validate the response XML returned by the testing framework following an execution.
     *
     * @param testIncName
     *            The name of the test .inc file to be executed.
     * @param programs
     *            A varargs array of {@code String} objects identifying the names of the CCL scripts to be executed for
     *            code coverage.
     * @throws Exception
     *             If any errors occur during the test run.
     */
    private void validateXml(final String testIncName, final String... programs) throws Exception {
        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));

        // Build a list of all programs to be compiled and executed
        final List<File> programFiles = new LinkedList<File>();
        for (final String program : programs)
            programFiles.add(new File(getTestResourceDirectory(), program + ".prg"));

        for (final File programFile : programFiles) {
            uploader.queueUpload(programFile);
        }
        uploader.upload();

        setRequestTestInc(testIncName);
        for (final String programName : programs)
            addRequestProgram(programName);

        final CclExecutor executor = getCclExecutor();
        for (final File programFile : programFiles)
            executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        for (final String programName : programs)
            executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record program = reply.getDynamicList("programs").get(0);

        final Schema environmentSchema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI)
                .newSchema(new File(getXsdDirectory(), "environment.xsd"));
        environmentSchema.newValidator().validate(new StreamSource(new StringReader(reply.getVC("environmentXML"))));

        final Schema listingSchema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI)
                .newSchema(new File(getXsdDirectory(), "/listing.xsd"));
        listingSchema.newValidator().validate(new StreamSource(new StringReader(reply.getVC("testINCListingXML"))));
        listingSchema.newValidator().validate(new StreamSource(new StringReader(program.getVC("listingXML"))));

        final Schema coverageSchema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI)
                .newSchema(new File(getXsdDirectory(), "/coverage.xsd"));
        coverageSchema.newValidator().validate(new StreamSource(new StringReader(reply.getVC("testINCCoverageXML"))));
        coverageSchema.newValidator().validate(new StreamSource(new StringReader(program.getVC("coverageXML"))));

        final Schema testResults = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI)
                .newSchema(new File(getXsdDirectory(), "/test-results.xsd"));
        testResults.newValidator().validate(new StreamSource(new StringReader(reply.getVC("testINCResultsXML"))));
    }

    /**
     * Test the execution of a test case with multiple timers that all pass. No unit test duplicates a timer name, but
     * each test reuses the same timer names as the other tests.
     *
     * @throws Exception
     *             Not expected.
     */
    @Test
    public void testSucceedWithTimers() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_timer_success.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(2);

        final TEST testOne = tests.get(0);
        assertThat(testOne.getNAME()).isEqualTo("TESTONE");
        assertThat(testOne.getRESULT()).isEqualTo("PASSED");

        final List<ASSERT> assertionsOne = testOne.getASSERTS().getASSERT();
        assertThat(assertionsOne).hasSize(2);

        final ASSERT assertionOneOne = assertionsOne.get(0);
        assertThat(assertionOneOne.getCONTEXT()).isEqualTo("timerA - Actual <= Maximum");
        assertThat(assertionOneOne.getRESULT()).isEqualTo("PASSED");
        assertThat(assertionOneOne.getTEST()).contains("<=1.500000");

        final ASSERT assertionOneTwo = assertionsOne.get(1);
        assertThat(assertionOneTwo.getCONTEXT()).isEqualTo("timerB - Actual <= Maximum");
        assertThat(assertionOneTwo.getRESULT()).isEqualTo("PASSED");
        assertThat(assertionOneTwo.getTEST()).contains("<=2.500000");

        final TEST testTwo = tests.get(1);
        assertThat(testTwo.getNAME()).isEqualTo("TESTTWO");
        assertThat(testTwo.getRESULT()).isEqualTo("PASSED");

        final List<ASSERT> assertionsTwo = testTwo.getASSERTS().getASSERT();
        assertThat(assertionsTwo).hasSize(2);

        final ASSERT assertionTwoOne = assertionsTwo.get(0);
        assertThat(assertionTwoOne.getCONTEXT()).isEqualTo("timerA - Actual <= Maximum");
        assertThat(assertionTwoOne.getRESULT()).isEqualTo("PASSED");
        assertThat(assertionTwoOne.getTEST()).contains("<=1.250000");

        final ASSERT assertionTwoTwo = assertionsTwo.get(1);
        assertThat(assertionTwoTwo.getCONTEXT()).isEqualTo("timerB - Actual <= Maximum");
        assertThat(assertionTwoTwo.getRESULT()).isEqualTo("PASSED");
        assertThat(assertionTwoTwo.getTEST()).contains("<=2.250000");
    }

    /**
     * Test the execution of a test case with a test that duplicates timer names.
     *
     * @throws Exception
     *             Not expected.
     */
    @Test
    public void testFailWithDuplicateTimers() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_timer_fail_duplicate.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);

        final TEST testOne = tests.get(0);
        assertThat(testOne.getNAME()).isEqualTo("TESTONE");
        assertThat(testOne.getRESULT()).isEqualTo("ERRORED");

        final List<ERROR> errorsOne = testOne.getERRORS().getERROR();
        assertThat(errorsOne).hasSize(2);

        final ERROR errorOneOne = errorsOne.get(0);
        assertThat(errorOneOne.getERRORTEXT()).contains(
                "{CCLEXCEPTION()}Exception(100): endTimer() - Timer end time not set because specified timer was never started: tImera.");
        final ERROR errorOneTwo = errorsOne.get(1);
        assertThat(errorOneTwo.getERRORTEXT()).contains(
                "{CCLEXCEPTION()}Exception(100): startTimer() - A timer already exists with the specified name - tImera.");
    }

    /**
     * Test the execution of a test case in which a timer expires.
     *
     * @throws Exception
     *             Not expected.
     */
    @Test
    public void testFailWithExpiredTimers() throws Exception {
        final String programName = "cclutDummyTest";
        final File programFile = new File(getTestResourceDirectory(), programName + ".prg");
        final String testIncName = "cclut_test_timer_fail_expired.inc";

        setRequestTestInc(testIncName);
        addRequestProgram(programName);

        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(new File(getTestResourceDirectory(), testIncName));
        uploader.queueUpload(programFile);
        uploader.upload();

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(programFile).commit();
        executor.addScriptExecution("cclut_execute_test_case").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.addScriptDropper(programName).commit();
        executor.execute();

        final Record statusData = reply.getRecord("status_data");
        assertThat(statusData.getChar("status")).isEqualTo("S");

        final JAXBContext jaxbContext = JAXBContext.newInstance(TESTCASE.class);
        final TESTCASE testCase = (TESTCASE) jaxbContext.createUnmarshaller()
                .unmarshal(new StringReader(reply.getVC("testINCResultsXML")));
        assertThat(testCase.getNAME()).isEqualTo(testIncName);

        final List<TEST> tests = testCase.getTESTS().getTEST();
        assertThat(tests).hasSize(1);

        final TEST testOne = tests.get(0);
        assertThat(testOne.getNAME()).isEqualTo("TESTONE");
        assertThat(testOne.getRESULT()).isEqualTo("FAILED");

        final List<ASSERT> assertionsOne = testOne.getASSERTS().getASSERT();
        assertThat(assertionsOne).hasSize(3);

        final ASSERT assertionOneOne = assertionsOne.get(0);
        assertThat(assertionOneOne.getCONTEXT()).isEqualTo("timerA - Actual <= Maximum");
        assertThat(assertionOneOne.getRESULT()).isEqualTo("PASSED");
        assertThat(assertionOneOne.getTEST()).contains("<=1.500000");

        final ASSERT assertionOneTwo = assertionsOne.get(1);
        assertThat(assertionOneTwo.getCONTEXT()).isEqualTo("timerB - Actual <= Maximum");
        assertThat(assertionOneTwo.getRESULT()).isEqualTo("FAILED");
        assertThat(assertionOneTwo.getTEST()).contains("<=1.750000");

        final ASSERT assertionOneThree = assertionsOne.get(2);
        assertThat(assertionOneThree.getCONTEXT()).isEqualTo("timerC - Actual <= Maximum");
        assertThat(assertionOneThree.getRESULT()).isEqualTo("PASSED");
        assertThat(assertionOneThree.getTEST()).contains("<=3.250000");
    }
}
