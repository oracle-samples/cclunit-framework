package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;

import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Tests for the {@code cclut_reflection_subs.inc} file.
 *
 * @author Joshua Hyde
 *
 */

public class CclutReflectionSubsTest extends AbstractCclutTest {
    /**
     * Test the retrieval of the test name.
     */
    @Test
    public void testCclutGetTestName() {
        final String scriptName = "testCclutGetTestName";
        final File scriptFile = new File(getTestResourceDirectory(), scriptName + ".prg");

        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addVC("testName").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getVC("testName")).isEqualTo("i.am.a.test.name");
    }
}
