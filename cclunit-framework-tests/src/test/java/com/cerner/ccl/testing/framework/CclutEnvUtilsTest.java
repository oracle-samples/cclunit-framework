package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;

import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Tests for the {@code cclut_env_utils.inc} files.
 * 
 * @author Joshua Hyde
 * 
 */

public class CclutEnvUtilsTest extends AbstractCclutTest {
    /**
     * If the major version of the LHS argument is less than the major version of the RHS argument, {@code true} should
     * be returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanMajorVersion() {
        assertThat(testComparison("7.5.2", "8.5.2")).isTrue();
    }

    /**
     * If the major versions match, but the minor version of the LHS is less than the minor version of the RHS, then
     * {@code true} should be returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanMinorVersion() {
        assertThat(testComparison("8.4.2", "8.5.2")).isTrue();
    }

    /**
     * If major and minor version match, but the LHS revision is less than the revision of the RHS, then {@code true}
     * should be returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanRevision() {
        assertThat(testComparison("8.5.20", "8.5.23")).isTrue();
    }

    /**
     * If the major version of the LHS is greater than the major version of the RHS, then {@code false} should be
     * returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanFalseMajorVersion() {
        assertThat(testComparison("9.5.2", "8.5.2")).isFalse();
    }

    /**
     * If the major versions match, but the minor version of the LHS is greater than that of the RHS, then {@code false}
     * should be returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanFalseMinorVersion() {
        assertThat(testComparison("8.6.2", "8.5.2")).isFalse();
    }

    /**
     * If the major and minor versions match, but the revision of the LHS is greater than that of the RHS, then
     * {@code false} should be returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanFalseRevision() {
        assertThat(testComparison("8.6.23", "8.6.20")).isFalse();
    }

    /**
     * If the two versions are completely equal, then {@code false} should be returned.
     */
    @Test
    public void testCclutIsCclVersionLessThanFalseEqual() {
        assertThat(testComparison("8.6.2", "8.6.2")).isFalse();
    }

    /**
     * Test the retrieval of the current CCL version.
     */
    @Test
    public void testCclutGetCclVersion() {
        final String scriptName = "cclutTestGetCclVersion";
        final Record reply = RecordFactory.create("testReply",
                StructureBuilder.getBuilder().addVC("expected").addVC("actual").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), scriptName + ".prg")).commit();
        executor.addScriptExecution(scriptName).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getVC("actual")).isEqualTo(reply.getVC("expected"));
    }

    /**
     * Compile and execute a script to perform a CCL version comparison.
     * 
     * @param lhs
     *            The "LHS" operator of the comparison.
     * @param rhs
     *            The "RHS" operator of the comparison.
     * @return A {@code boolean} representation of the value returned by the called script.
     */
    private boolean testComparison(String lhs, String rhs) {
        final String scriptName = "cclutTestIsCclVersionLessThan";
        final Record request = RecordFactory.create("comparisonRequest",
                StructureBuilder.getBuilder().addVC("lhs").addVC("rhs").build());
        final Record reply = RecordFactory.create("comparisonReply",
                StructureBuilder.getBuilder().addI2("isLessThan").build());

        request.setVC("lhs", lhs);
        request.setVC("rhs", rhs);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(new File(getTestResourceDirectory(), scriptName + ".prg")).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        return reply.getI2Boolean("isLessThan");
    }
}
