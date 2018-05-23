package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;

import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Unit tests for the {@code cclut_error_handling.inc} file.
 * 
 * @author Joshua Hyde
 * 
 */

public class CclutErrorHandlingTest extends AbstractCclutTest {
    /**
     * Test that the capturing of a CCL error works.
     */
    @Test
    public void testCclErrorOccurredTrue() {
        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("error_ind").addVC("error_msg").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutTestErrorOccurredTrue.prg")).commit();
        executor.addScriptExecution("cclutTestErrorOccurredTrue").withReplace("REPLY", reply).commit();
        executor.addScriptDropper("cclutTestErrorOccurredTrue").commit();
        executor.execute();

        assertThat(reply.getI2Boolean("error_ind")).isTrue();
        assertThat(reply.getVC("error_msg")).isNotEmpty();
    }

    /**
     * Test that, if an error does not occur, that no errors are reported.
     */
    @Test
    public void testCclErrorOccurredFalse() {
        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("error_ind").addVC("error_msg").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutTestErrorOccurredFalse.prg")).commit();
        executor.addScriptExecution("cclutTestErrorOccurredFalse").withReplace("REPLY", reply).commit();
        executor.addScriptDropper("cclutTestErrorOccurredFalse").commit();
        executor.execute();

        assertThat(reply.getI2Boolean("error_ind")).isFalse();
        assertThat(reply.getVC("error_msg")).isEmpty();
    }

    /**
     * Test the retrieval of the line number from a CCL-E error message.
     */
    @Test
    public void testGetCclErrorLineNumber() {
        final String message = "%CCL-E-259-CCLUTCCLERRORLINENUMBERTEST(120,0)S0L1.1r2{REQUEST->ERROR_MSG}.";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("error_msg").build());
        request.setVC("error_msg", message);

        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addI4("line_number").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutCclErrorLineNumberTest.prg")).commit();
        executor.addScriptExecution("cclutCclErrorLineNumberTest").withReplace("request", request)
                .withReplace("reply", reply).commit();
        executor.addScriptDropper("cclutCclErrorLineNumberTest").commit();
        executor.execute();

        assertThat(reply.getI4("line_number")).isEqualTo(120);
    }

    /**
     * If the given error message contains no comma following the parenthesis, then a zero should be returned.
     */
    @Test
    public void testGetCclErrorLineNumberNoComma() {
        final String message = "%CCL-E-259-CCLUTCCLERRORLINENUMBERTEST(120";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("error_msg").build());
        request.setVC("error_msg", message);

        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addI4("line_number").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutCclErrorLineNumberTest.prg")).commit();
        executor.addScriptExecution("cclutCclErrorLineNumberTest").withReplace("request", request)
                .withReplace("reply", reply).commit();
        executor.addScriptDropper("cclutCclErrorLineNumberTest").commit();
        executor.execute();

        assertThat(reply.getI4("line_number")).isZero();
    }

    /**
     * If the given error message contains no parenthesis, then a zero should be returned.
     */
    @Test
    public void testGetCclErrorLineNumberNoParenthesis() {

        final String message = "%CCL-E-259-CCLUTCCLERRORLINENUMBERTEST";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("error_msg").build());
        request.setVC("error_msg", message);

        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addI4("line_number").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutCclErrorLineNumberTest.prg")).commit();
        executor.addScriptExecution("cclutCclErrorLineNumberTest").withReplace("request", request)
                .withReplace("reply", reply).commit();
        executor.addScriptDropper("cclutCclErrorLineNumberTest").commit();
        executor.execute();

        assertThat(reply.getI4("line_number")).isZero();
    }
}
