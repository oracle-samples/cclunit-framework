package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import org.junit.Before;
import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Tests for the {@code cclut_merge_cc.prg} CCL script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutMergeCcTest extends AbstractCclutTest {
    private Record request;
    private Record reply;

    /**
     * Set up the request and reply for each test.
     */
    @Before
    public void setUp() {
        request = RecordFactory.create("testRequest",
                StructureBuilder.getBuilder().addVC("targetXml").addVC("sourceXml").build());
        reply = RecordFactory.create("testReply", StructureBuilder.getBuilder().addVC("mergedXml").build());
    }

    /**
     * If the first XML string is blank, then whatever is provided as the second XML string should be returned.
     */
    @Test
    public void testFirstBlank() {
        final String expectedXml = "i should be returned, even though I am not XML";
        request.setVC("targetXml", "");
        request.setVC("sourceXml", expectedXml);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_merge_cc").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("mergedXml")).isEqualTo(expectedXml);
    }

    /**
     * If the first XML string says a line is covered, then whatever's in the second XML string should be ignored.
     */
    @Test
    public void testFirstCovered() {
        final String programName = "A.PROGRAM.NAME";
        final String originalXml = "<COVERAGE><COVERAGE_NAME>" + programName
                + "</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>C</TYPE></LINE></LINES></COVERAGE>";
        final String mergeXml = "<COVERAGE><COVERAGE_NAME>" + programName
                + "</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>N</TYPE></LINE></LINES></COVERAGE>";

        request.setVC("targetXml", originalXml);
        request.setVC("sourceXml", mergeXml);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_merge_cc").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("mergedXml")).isEqualTo(originalXml);
    }

    /**
     * If the second XML string does not say a line is covered, then whatever's in the second XML string should be
     * returned.
     */
    @Test
    public void testFirstNotCovered() {
        final String originalXml = "<COVERAGE><COVERAGE_NAME>A.PROGRAM.NAME</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>C</TYPE></LINE></LINES></COVERAGE>";
        final String mergeXml = "<COVERAGE><COVERAGE_NAME>A.PROGRAM.NAME</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>C</TYPE></LINE></LINES></COVERAGE>";

        request.setVC("targetXml", originalXml);
        request.setVC("sourceXml", mergeXml);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_merge_cc").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("mergedXml")).isEqualTo(mergeXml);
    }

    /**
     * If the second XML string is blank and the first XML string is not blank, then whatever's in the first string
     * should be returned.
     */
    @Test
    public void testSecondBlank() {
        final String expectedXml = "i should be returned, even though I am not XML";
        request.setVC("targetXml", expectedXml);
        request.setVC("sourceXml", "");

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_merge_cc").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("mergedXml")).isEqualTo(expectedXml);
    }
}
