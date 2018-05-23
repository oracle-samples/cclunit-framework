package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import org.junit.Before;
import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Unit tests for the {@code cclut_parse_coverages.prg} CCL script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutParseCoveragesTest extends AbstractCclutTest {
    private Record request;
    private Record reply;

    /**
     * Set up the request and reply.
     */
    @Before
    public void setUp() {
        request = RecordFactory.create("testRequest",
                StructureBuilder.getBuilder().addVC("programName").addVC("coverageXml").build());
        reply = RecordFactory.create("testReply", StructureBuilder.getBuilder().addVC("coverageXml").build());
    }

    /**
     * If the request XML contains multiple listings of coverage XML for the same program, then they should be
     * consolidated using a call to the underlying {@code cclut_merge_cc} script.
     */
    @Test
    public void testParseMultipleCoverages() {
        final String programName = "A.PROGRAM.NAME";
        final String firstCoverageXml = "<COVERAGE><COVERAGE_NAME>" + programName
                + "</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>N</TYPE></LINE></LINES></COVERAGE>";
        final String secondCoverageXml = "<COVERAGE><COVERAGE_NAME>" + programName
                + "</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>C</TYPE></LINE></LINES></COVERAGE>";
        final String requestXml = "<A_BUNCH_OF_USELESS_STUFF><STILL_USELESS>" + firstCoverageXml + secondCoverageXml
                + "</STILL_USELESS></A_BUNCH_OF_USELESS_STUFF>";

        request.setVC("programName", programName);
        request.setVC("coverageXml", requestXml);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_parse_coverages").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("coverageXml")).isEqualTo(secondCoverageXml);
    }

    /**
     * If the request XML contains no references to the requested program, then a blank string should be returned.
     */
    @Test
    public void testParseProgramNotFound() {
        request.setVC("programName", "NEVER.WILL.BE.FOUND");
        request.setVC("coverageXml",
                "<COVERAGE><COVERAGE_NAME>A.PROGRAM.NAME</COVERAGE_NAME><SOME_BASIC_CONTENT /></COVERAGE>");

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_parse_coverages").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("coverageXml")).isEmpty();
    }

    /**
     * Test the parsing of a single {@code <COVERAGE />} tag from the given XML.
     */
    @Test
    public void testParseSingleCoverage() {
        final String programName = "A.PROGRAM.NAME";
        final String coverageXml = "<COVERAGE><COVERAGE_NAME>" + programName
                + "</COVERAGE_NAME><SOME_BASIC_CONTENT /></COVERAGE>";
        final String requestXml = "<A_BUNCH_OF_USELESS_STUFF><STILL_USELESS>" + coverageXml
                + "</STILL_USELESS></A_BUNCH_OF_USELESS_STUFF>";

        request.setVC("programName", programName);
        request.setVC("coverageXml", requestXml);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_parse_coverages").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        assertThat(reply.getVC("coverageXml")).isEqualTo(coverageXml);
    }
}
