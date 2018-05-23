package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.util.ArrayList;
import java.util.Collection;

import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Tests for the {@code cclut_find_unit_tests} CCL script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutFindUnitTestsTest extends AbstractCclutTest {
    /**
     * Test the parsing of a test include file for the tests located within it.
     */
    @Test
    public void getTests() {
        final String scriptName = getClass().getSimpleName();

        // Request/reply for parsing script
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("programName").build());
        request.setVC("programName", scriptName);
        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder()
                        .addDynamicList("tests", StructureBuilder.getBuilder().addVC("testName").build())
                        .addStatusData().build());

        // Compile the include file in a script so that it can be parsed
        final CclExecutor executor = getCclExecutor();
        executor.addDynamicCompiler(new File(getTestResourceDirectory(), "cclutTwoTests.inc"))
                .withScriptName(scriptName).commit();
        executor.addScriptExecution("cclut_find_unit_tests").withReplace("cclutReply", reply)
                .withReplace("cclutRequest", request).commit();
        executor.execute();

        // Convert the records to strings to make comparison easier
        final Collection<String> testNames = new ArrayList<String>();
        for (Record record : reply.getDynamicList("tests"))
            testNames.add(record.getVC("testName"));

        assertThat(testNames).containsOnly("TESTA", "TESTB");
    }
}
