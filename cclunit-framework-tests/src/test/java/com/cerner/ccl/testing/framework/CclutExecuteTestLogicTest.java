package com.cerner.ccl.testing.framework;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayOutputStream;
import java.io.File;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.enums.OutputType;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Unit tests for the {@code cclut_execute_test_logic.inc} file.
 *
 * @author Joshua Hyde
 * @author Nicholas Feldmann
 *
 */
public class CclutExecuteTestLogicTest extends AbstractCclutTest {
    // Script file and name
    private final File scriptFile = new File(getTestResourceDirectory(), "cclutEvaluateAssertsTest.prg");

    private final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

    // Record structures
    private Record dt_reply, dt_request, dt_assert, cclut_request, cclut_reply;

    // this is a hack. need to do with replace on a subroutine name, but j4ccl only knows how to do with replace on a
    // record structure name.
    private final Record doNothing = RecordFactory.create("doNothing",
            StructureBuilder.getBuilder().addVC("doNothing").build());

    /**
     * Compile the testing scripts and initiate records.
     */
    @Before
    public void setup() {
        // Compile script
        final CclExecutor compileExecutor = getCclExecutor();
        compileExecutor.addScriptCompiler(scriptFile).commit();
        compileExecutor.execute();

        // Create record structures
        dt_assert = RecordFactory.create("dt_assert", StructureBuilder.getBuilder()
                .addDynamicList("line",
                        StructureBuilder.getBuilder().addI4("lineNumber").addVC("context").addVC("result")
                                .addVC("datetime").addVC("condition").addI4("errorCodeBefore")
                                .addVC("errorMessageBefore").addI4("errorCode").addVC("errorMessage").build())
                .build());

        cclut_request = RecordFactory.create("cclut_request",
                StructureBuilder.getBuilder().addVC("testNamePattern").build());

        cclut_reply = RecordFactory.create("cclutReply", StructureBuilder.getBuilder().addI2("resultInd")
                .addDynamicList("tests", StructureBuilder.getBuilder().addVC("name")
                        .addDynamicList("asserts",
                                StructureBuilder.getBuilder().addI4("lineNumber").addVC("context").addI2("resultInd")
                                        .addVC("condition").build())
                        .addDynamicList("errors",
                                StructureBuilder.getBuilder().addI4("lineNumber").addVC("errorText").build())
                        .build())
                .addStatusData().build());

        dt_request = RecordFactory.create("dt_request", StructureBuilder.getBuilder().addI4("testIndex").build());

        dt_reply = RecordFactory.create("dt_reply", StructureBuilder.getBuilder().addI2("result").build());
    }

    /**
     * Drop the testing scripts and remove records.
     */
    @After
    public void tearDown() {
        // Drop record structures
        dt_reply = null;
        dt_request = null;
        dt_assert = null;
        cclut_reply = null;

        // Drop scripts
        final CclExecutor dropExecutor = getCclExecutor();
        dropExecutor.addScriptDropper(scriptName).commit();
        dropExecutor.execute();

    }

    /**
     * Test that true is returned when there no ccl errors or failed asserts
     */
    @Test
    public void testEvaluateAssertsTrue() {
        // Populate record structures
        dt_request.setI4("testIndex", 1);

        dt_assert.getDynamicList("line").addItem();
        dt_assert.getDynamicList("line").get(0).setI4("lineNumber", 1);
        dt_assert.getDynamicList("line").get(0).setVC("context", "pass context");
        dt_assert.getDynamicList("line").get(0).setVC("result", "PASS");
        dt_assert.getDynamicList("line").get(0).setVC("datetime", "NOW");
        dt_assert.getDynamicList("line").get(0).setVC("condition", "pass condition");
        dt_assert.getDynamicList("line").get(0).setI4("errorCodeBefore", 0);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessageBefore", "ERROR");
        dt_assert.getDynamicList("line").get(0).setI4("errorCode", 0);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessage", "ERROR");

        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("dt_assert", dt_assert).withReplace("dt_reply", dt_reply)
                .withReplace("dt_request", dt_request).withReplace("cclutRequest", cclut_request)
                .withReplace("cclutReply", cclut_reply).withReplace("cclut_runResult", dt_assert)
                .withReplace("executeTestLogic", doNothing).withReplace("executeTestLogic", doNothing).commit();
        executor.execute();

        // Assert the result is true
        assertEquals("Result not true", 1, dt_reply.getI2("result"));
        assertEquals(1, cclut_reply.getI2("resultInd"));
        assertEquals(1, cclut_reply.getDynamicList("tests").getSize());
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").getSize());
        assertEquals(1,
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("lineNumber"));
        assertEquals("pass context",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("context"));
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("resultInd"));
        assertEquals("pass condition",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("condition"));
        assertEquals(0, cclut_reply.getDynamicList("tests").get(0).getDynamicList("errors").getSize());
    }

    /**
     * Test that false is returned when there is a failed assert
     */
    @Test
    public void testEvaluateAssertsFail() {
        // Populate record structures
        dt_request.setI4("testIndex", 1);

        dt_assert.getDynamicList("line").addItem();
        dt_assert.getDynamicList("line").get(0).setI4("lineNumber", 2);
        dt_assert.getDynamicList("line").get(0).setVC("context", "fail context");
        dt_assert.getDynamicList("line").get(0).setVC("result", "FAIL");
        dt_assert.getDynamicList("line").get(0).setVC("datetime", "NOW");
        dt_assert.getDynamicList("line").get(0).setVC("condition", "fail condition");
        dt_assert.getDynamicList("line").get(0).setI4("errorCodeBefore", 0);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessageBefore", "ERROR");
        dt_assert.getDynamicList("line").get(0).setI4("errorCode", 0);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessage", "ERROR");

        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("dt_assert", dt_assert).withReplace("dt_reply", dt_reply)
                .withReplace("dt_request", dt_request).withReplace("cclutRequest", cclut_request)
                .withReplace("cclut_runResult", dt_assert).withReplace("executeTestLogic", doNothing)
                .withReplace("cclutReply", cclut_reply).commit();
        executor.execute();

        assertEquals("Result is not false", 0, dt_reply.getI2("result"));
        assertEquals(0, cclut_reply.getI2("resultInd"));
        assertEquals(1, cclut_reply.getDynamicList("tests").getSize());
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").getSize());
        assertEquals(2,
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("lineNumber"));
        assertEquals("fail context",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("context"));
        assertEquals(0, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("resultInd"));
        assertEquals("fail condition",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("condition"));
        assertEquals(0, cclut_reply.getDynamicList("tests").get(0).getDynamicList("errors").getSize());
    }

    /**
     * Test that false is returned when there is a CCL error before the asserts
     */
    @Test
    public void testEvaluateAssertsBeforeError() {
        // Populate record structures
        dt_request.setI4("testIndex", 1);

        dt_assert.getDynamicList("line").addItem();
        dt_assert.getDynamicList("line").get(0).setI4("lineNumber", 3);
        dt_assert.getDynamicList("line").get(0).setVC("context", "error before context");
        dt_assert.getDynamicList("line").get(0).setVC("result", "PASS");
        dt_assert.getDynamicList("line").get(0).setVC("datetime", "NOW");
        dt_assert.getDynamicList("line").get(0).setVC("condition", "error before condition");
        dt_assert.getDynamicList("line").get(0).setI4("errorCodeBefore", 1);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessageBefore", "ERROR_BEFORE");
        dt_assert.getDynamicList("line").get(0).setI4("errorCode", 0);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessage", "ERROR_DURING");

        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("dt_assert", dt_assert).withReplace("dt_reply", dt_reply)
                .withReplace("dt_request", dt_request).withReplace("cclutRequest", cclut_request)
                .withReplace("cclut_runResult", dt_assert).withReplace("executeTestLogic", doNothing)
                .withReplace("cclutReply", cclut_reply).commit();
        executor.execute();

        assertEquals("Result is not false", dt_reply.getI2("result"), 0);
        assertEquals(0, cclut_reply.getI2("resultInd"));
        assertEquals(1, cclut_reply.getDynamicList("tests").getSize());
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").getSize());
        assertEquals(3,
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("lineNumber"));
        assertEquals("error before context",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("context"));
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("resultInd"));
        assertEquals("error before condition",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("condition"));
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("errors").getSize());
        assertEquals("ERROR_BEFORE",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("errors").get(0).getVC("errorText"));
    }

    /**
     * Test that false is returned when there is a CCL error during the tests
     */
    @Test
    public void testEvaluateAssertsError() {
        // Populate record structures
        dt_request.setI4("testIndex", 1);

        dt_assert.getDynamicList("line").addItem();
        dt_assert.getDynamicList("line").get(0).setI4("lineNumber", 4);
        dt_assert.getDynamicList("line").get(0).setVC("context", "error during context");
        dt_assert.getDynamicList("line").get(0).setVC("result", "PASS");
        dt_assert.getDynamicList("line").get(0).setVC("datetime", "NOW");
        dt_assert.getDynamicList("line").get(0).setVC("condition", "error during condition");
        dt_assert.getDynamicList("line").get(0).setI4("errorCodeBefore", 0);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessageBefore", "ERROR_BEFORE");
        dt_assert.getDynamicList("line").get(0).setI4("errorCode", 1);
        dt_assert.getDynamicList("line").get(0).setVC("errorMessage", "ERROR_DURING");

        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("dt_assert", dt_assert).withReplace("dt_reply", dt_reply)
                .withReplace("cclut_runResult", dt_assert).withReplace("executeTestLogic", doNothing)
                .withReplace("dt_request", dt_request).withReplace("cclutRequest", cclut_request)
                .withReplace("cclutReply", cclut_reply).commit();
        executor.execute();

        assertEquals("Result is not false", 0, dt_reply.getI2("result"));
        assertEquals(0, cclut_reply.getI2("resultInd"));
        assertEquals(1, cclut_reply.getDynamicList("tests").getSize());
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").getSize());
        assertEquals(4,
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("lineNumber"));
        assertEquals("error during context",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("context"));
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI4("resultInd"));
        assertEquals("error during condition",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getVC("condition"));
        assertEquals(1, cclut_reply.getDynamicList("tests").get(0).getDynamicList("errors").getSize());
        assertEquals("ERROR_DURING",
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("errors").get(0).getVC("errorText"));
    }

    /**
     * Test that the correct status is returned, along with the correct assert results and test names when one/more
     * asserts have failed
     */
    @Test
    public void testExecuteFailTestLogic() {
        // Get test script file
        final File file = new File(getTestResourceDirectory(), "cclutExecuteFailTestLogic.prg");

        final String fileName = StringUtils.stripFilenameExtension(file.getName());

        // Compile test script
        final CclExecutor createExecutor = getCclExecutor();
        createExecutor.addScriptCompiler(file).commit();
        createExecutor.execute();

        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(fileName).withReplace("cclutRequest", cclut_request)
                .withReplace("cclutReply", cclut_reply).commit();
        executor.execute();

        assertEquals("Wrong status", "S", cclut_reply.getRecord("status_data").getChar("status"));

        assertEquals("Wrong result status", 0, cclut_reply.getI2("resultInd"));

        assertTrue("Wrong test name: " + cclut_reply.getDynamicList("tests").get(0).getVC("name"),
                cclut_reply.getDynamicList("tests").get(0).getVC("name").equalsIgnoreCase("testFail"));
        assertTrue("Wrong test name: " + cclut_reply.getDynamicList("tests").get(1).getVC("name"),
                cclut_reply.getDynamicList("tests").get(1).getVC("name").equalsIgnoreCase("testSuccess"));

        assertEquals("Wrong assert result", 0,
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI2("resultInd"));
        assertEquals("Wrong assert result", 1,
                cclut_reply.getDynamicList("tests").get(1).getDynamicList("asserts").get(0).getI2("resultInd"));

        // Drop test script
        final CclExecutor dropper = getCclExecutor();
        dropper.addScriptDropper(fileName).commit();
        dropper.execute();

    }

    /**
     * Test that the correct status is returned, along with the correct assert results and test names when no asserts
     * have failed
     */
    @Test
    public void testExecuteSuccessTestLogic() {
        // Get test script file
        final File file = new File(getTestResourceDirectory(), "cclutExecuteSuccessTestLogic.prg");

        final String fileName = StringUtils.stripFilenameExtension(file.getName());

        // Compile test script
        final CclExecutor createExecutor = getCclExecutor();
        createExecutor.addScriptCompiler(file).commit();
        createExecutor.execute();

        // Execute test script
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(fileName).withReplace("cclutRequest", cclut_request)
                .withReplace("cclutReply", cclut_reply).commit();
        final ByteArrayOutputStream executeOutput = new ByteArrayOutputStream();
        executor.setOutputStream(executeOutput, OutputType.CCL_SESSION);
        executor.execute();
        System.out.println(executeOutput.toString());

        assertEquals("Wrong status", "S", cclut_reply.getRecord("status_data").getChar("status"));

        assertEquals("Wrong result status", 1, cclut_reply.getI2("resultInd"));

        assertTrue("Wrong test name: " + cclut_reply.getDynamicList("tests").get(0).getVC("name"),
                cclut_reply.getDynamicList("tests").get(0).getVC("name").equalsIgnoreCase("test1"));
        assertTrue("Wrong test name: " + cclut_reply.getDynamicList("tests").get(1).getVC("name"),
                cclut_reply.getDynamicList("tests").get(1).getVC("name").equalsIgnoreCase("test2"));

        assertEquals("Wrong assert result", 1,
                cclut_reply.getDynamicList("tests").get(0).getDynamicList("asserts").get(0).getI2("resultInd"));
        assertEquals("Wrong assert result", 1,
                cclut_reply.getDynamicList("tests").get(1).getDynamicList("asserts").get(0).getI2("resultInd"));

        // Drop test script
        final CclExecutor dropper = getCclExecutor();
        dropper.addScriptDropper(fileName).commit();
        dropper.execute();
    }
}
