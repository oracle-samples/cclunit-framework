package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.util.Calendar;
import java.util.Date;

import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Field;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.Structure;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Unit tests for the {@code cclutassert.inc} file.
 * 
 * @author Joshua Hyde
 * 
 */

public class CclutAssertTest extends AbstractCclutTest {
    /**
     * Test the cclutAssertAlmostEqual subroutine.
     */
    @Test
    public void testAlmostEqual() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestAlmostEqual.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestStructBuilder = StructureBuilder.getBuilder();
        final Structure listBuilder = StructureBuilder.getBuilder().addF8("expected").addF8("actual").addF8("delta")
                .build();
        requestStructBuilder.addList("almostEqual", listBuilder, 1);
        requestStructBuilder.addList("notAlmostEqual", listBuilder, 1);
        final Record request = RecordFactory.create("request", requestStructBuilder.build());
        request.getList("almostEqual").get(0).setF8("expected", 3.0);
        request.getList("almostEqual").get(0).setF8("actual", 2.8);
        request.getList("almostEqual").get(0).setF8("delta", 0.5);

        request.getList("notAlmostEqual").get(0).setF8("expected", 3.0);
        request.getList("notAlmostEqual").get(0).setF8("actual", 2.0);
        request.getList("notAlmostEqual").get(0).setF8("delta", 0.5);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("almostEqualResponse").addI2("notAlmostEqualResponse").build());

        // Set all reply fields to invalid boolean values to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("almostEqualResponse")).isTrue();
        assertThat(reply.getI2Boolean("notAlmostEqualResponse")).isFalse();
    }

    /**
     * Test the cclutAssertContains subroutine.
     */
    @Test
    public void testContains() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestContains.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Structure list = StructureBuilder.getBuilder().addVC("string").addVC("substring").build();
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addList("contains", list, 1).addList("notContains", list, 1).build());
        request.getList("contains").get(0).setVC("string", "abcdefg");
        request.getList("contains").get(0).setVC("substring", "abc");
        request.getList("notContains").get(0).setVC("string", "abc");
        request.getList("notContains").get(0).setVC("substring", "xyz");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("containsResponse").addI2("notContainsResponse").build());
        // Fill the reply with non-boolean values to avoid false positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("containsResponse")).isTrue();
        assertThat(reply.getI2Boolean("notContainsResponse")).isFalse();
    }

    /**
     * Test the cclutAssertEndsWith subroutine.
     */
    @Test
    public void testEndsWith() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestEndsWith.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Structure list = StructureBuilder.getBuilder().addVC("string").addVC("substring").build();
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addList("endsWith", list, 1).addList("notEndsWith", list, 1).build());
        request.getList("endsWith").get(0).setVC("string", "abcdefg");
        request.getList("endsWith").get(0).setVC("substring", "efg");
        request.getList("notEndsWith").get(0).setVC("string", "abcdefg");
        request.getList("notEndsWith").get(0).setVC("substring", "xyz");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("endsWithResponse").addI2("notEndsWithResponse").build());
        // Fill the reply with non-boolean values to avoid false positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("endsWithResponse")).isTrue();
        assertThat(reply.getI2Boolean("notEndsWithResponse")).isFalse();
    }

    /**
     * Test all of the assert*Equal subroutines.
     */
    @Test
    public void testEqual() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestEqual.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestBuilder = StructureBuilder.getBuilder();
        requestBuilder.addList("f8Equal", StructureBuilder.getBuilder().addF8("value").build(), 1);
        requestBuilder.addList("f8Inequal", StructureBuilder.getBuilder().addF8("expected").addF8("actual").build(), 1);
        requestBuilder.addList("i4Equal", StructureBuilder.getBuilder().addI4("value").build(), 1);
        requestBuilder.addList("i4Inequal", StructureBuilder.getBuilder().addI4("expected").addI4("actual").build(), 1);
        requestBuilder.addList("i2Equal", StructureBuilder.getBuilder().addI2("value").build(), 1);
        requestBuilder.addList("i2Inequal", StructureBuilder.getBuilder().addI4("expected").addI4("actual").build(), 1);
        requestBuilder.addList("vcEqual", StructureBuilder.getBuilder().addVC("value").build(), 1);
        requestBuilder.addList("vcInequal", StructureBuilder.getBuilder().addVC("expected").addVC("actual").build(), 1);
        requestBuilder.addList("dateEqual", StructureBuilder.getBuilder().addDQ8("value").build(), 1);
        requestBuilder.addList("dateInequal", StructureBuilder.getBuilder().addDQ8("expected").addDQ8("actual").build(),
                1);
        final Record request = RecordFactory.create("request", requestBuilder.build());

        request.getList("f8Equal").get(0).setF8("value", 2.5);
        request.getList("f8Inequal").get(0).setF8("expected", 2.0);
        request.getList("f8Inequal").get(0).setF8("actual", 1.5);
        request.getList("i4Equal").get(0).setI4("value", 3);
        request.getList("i4Inequal").get(0).setI4("expected", 3);
        request.getList("i4Inequal").get(0).setI4("actual", 2);
        request.getList("i2Equal").get(0).setI2("value", (short) 1);
        request.getList("i2Inequal").get(0).setI2("expected", (short) 1);
        request.getList("i2Inequal").get(0).setI2("actual", (short) 2);
        request.getList("vcEqual").get(0).setVC("value", "test");
        request.getList("vcInequal").get(0).setVC("expected", "test123");
        request.getList("vcInequal").get(0).setVC("actual", "321tset");
        request.getList("dateEqual").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateInequal").get(0).setDQ8("expected", Calendar.getInstance().getTime());
        request.getList("dateInequal").get(0).setDQ8("actual", new Date(System.currentTimeMillis() / 3));

        final StructureBuilder replyBuilder = StructureBuilder.getBuilder();
        replyBuilder.addI2("f8EqualResponse").addI2("f8InequalResponse").addI2("i4EqualResponse")
                .addI2("i4InequalResponse").addI2("i2EqualResponse").addI2("i2InequalResponse").addI2("vcEqualResponse")
                .addI2("vcInequalResponse").addI2("dateEqualResponse").addI2("dateInequalResponse");
        final Record reply = RecordFactory.create("reply", replyBuilder.build());

        // Set all of the fields to an invalid boolean value to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("f8EqualResponse")).isTrue();
        assertThat(reply.getI2Boolean("f8InequalResponse")).isFalse();
        assertThat(reply.getI2Boolean("i4EqualResponse")).isTrue();
        assertThat(reply.getI2Boolean("i4InequalResponse")).isFalse();
        assertThat(reply.getI2Boolean("i2EqualResponse")).isTrue();
        assertThat(reply.getI2Boolean("i2InequalResponse")).isFalse();
        assertThat(reply.getI2Boolean("vcEqualResponse")).isTrue();
        assertThat(reply.getI2Boolean("vcInequalResponse")).isFalse();
        assertThat(reply.getI2Boolean("dateEqualResponse")).isTrue();
        assertThat(reply.getI2Boolean("dateInequalResponse")).isFalse();
    }

    /**
     * Test all of the assert*LessThan subroutines.
     */
    @Test
    public void testLessThan() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestLessThan.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestBuilder = StructureBuilder.getBuilder();
        requestBuilder.addList("f8LessThan", StructureBuilder.getBuilder().addF8("value").build(), 1);
        requestBuilder.addList("f8NotLessThan",
                StructureBuilder.getBuilder().addF8("value").addF8("comparisonValue").build(), 1);
        requestBuilder.addList("i4LessThan", StructureBuilder.getBuilder().addI4("value").build(), 1);
        requestBuilder.addList("i4NotLessThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("i2LessThan", StructureBuilder.getBuilder().addI2("value").build(), 1);
        requestBuilder.addList("i2NotLessThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("vcLessThan", StructureBuilder.getBuilder().addVC("value").build(), 1);
        requestBuilder.addList("vcNotLessThan",
                StructureBuilder.getBuilder().addVC("value").addVC("comparisonValue").build(), 1);
        requestBuilder.addList("dateLessThan", StructureBuilder.getBuilder().addDQ8("value").build(), 1);
        requestBuilder.addList("dateNotLessThan",
                StructureBuilder.getBuilder().addDQ8("value").addDQ8("comparisonValue").build(), 1);
        final Record request = RecordFactory.create("request", requestBuilder.build());

        request.getList("f8LessThan").get(0).setF8("value", 2.5);
        request.getList("f8NotLessThan").get(0).setF8("value", 2.0);
        request.getList("f8NotLessThan").get(0).setF8("comparisonValue", 1.5);
        request.getList("i4LessThan").get(0).setI4("value", 3);
        request.getList("i4NotLessThan").get(0).setI4("value", 3);
        request.getList("i4NotLessThan").get(0).setI4("comparisonValue", 2);
        request.getList("i2LessThan").get(0).setI2("value", (short) 1);
        request.getList("i2NotLessThan").get(0).setI2("value", (short) 2);
        request.getList("i2NotLessThan").get(0).setI2("comparisonValue", (short) 1);
        request.getList("vcLessThan").get(0).setVC("value", "test");
        request.getList("vcNotLessThan").get(0).setVC("value", "test123");
        request.getList("vcNotLessThan").get(0).setVC("comparisonValue", "321tset");
        request.getList("dateLessThan").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateNotLessThan").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateNotLessThan").get(0).setDQ8("comparisonValue", new Date(System.currentTimeMillis() / 3));

        final StructureBuilder replyBuilder = StructureBuilder.getBuilder();
        replyBuilder.addI2("f8LessThanResponse").addI2("f8NotLessThanResponse").addI2("i4LessThanResponse")
                .addI2("i4NotLessThanResponse").addI2("i2LessThanResponse").addI2("i2NotLessThanResponse")
                .addI2("vcLessThanResponse").addI2("vcNotLessThanResponse").addI2("dateLessThanResponse")
                .addI2("dateNotLessThanResponse");
        final Record reply = RecordFactory.create("reply", replyBuilder.build());

        // Set all of the fields to an invalid boolean value to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("f8LessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("f8NotLessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i4LessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i4NotLessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i2LessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i2NotLessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("vcLessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("vcNotLessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("dateLessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("dateNotLessThanResponse")).isFalse();
    }

    /**
     * Test all of the assert*LessThan subroutines.
     */
    @Test
    public void testGreaterThan() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestGreaterThan.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestBuilder = StructureBuilder.getBuilder();
        requestBuilder.addList("f8GreaterThan", StructureBuilder.getBuilder().addF8("value").build(), 1);
        requestBuilder.addList("f8NotGreaterThan",
                StructureBuilder.getBuilder().addF8("value").addF8("comparisonValue").build(), 1);
        requestBuilder.addList("i4GreaterThan", StructureBuilder.getBuilder().addI4("value").build(), 1);
        requestBuilder.addList("i4NotGreaterThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("i2GreaterThan", StructureBuilder.getBuilder().addI2("value").build(), 1);
        requestBuilder.addList("i2NotGreaterThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("vcGreaterThan", StructureBuilder.getBuilder().addVC("value").build(), 1);
        requestBuilder.addList("vcNotGreaterThan",
                StructureBuilder.getBuilder().addVC("value").addVC("comparisonValue").build(), 1);
        requestBuilder.addList("dateGreaterThan", StructureBuilder.getBuilder().addDQ8("value").build(), 1);
        requestBuilder.addList("dateNotGreaterThan",
                StructureBuilder.getBuilder().addDQ8("value").addDQ8("comparisonValue").build(), 1);
        final Record request = RecordFactory.create("request", requestBuilder.build());

        request.getList("f8GreaterThan").get(0).setF8("value", 2.5);
        request.getList("f8NotGreaterThan").get(0).setF8("value", 1.5);
        request.getList("f8NotGreaterThan").get(0).setF8("comparisonValue", 2.0);
        request.getList("i4GreaterThan").get(0).setI4("value", 3);
        request.getList("i4NotGreaterThan").get(0).setI4("value", 2);
        request.getList("i4NotGreaterThan").get(0).setI4("comparisonValue", 3);
        request.getList("i2GreaterThan").get(0).setI2("value", (short) 2);
        request.getList("i2NotGreaterThan").get(0).setI2("value", (short) 1);
        request.getList("i2NotGreaterThan").get(0).setI2("comparisonValue", (short) 2);
        request.getList("vcGreaterThan").get(0).setVC("value", "test");
        request.getList("vcNotGreaterThan").get(0).setVC("value", "test12");
        request.getList("vcNotGreaterThan").get(0).setVC("comparisonValue", "test123");
        request.getList("dateGreaterThan").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateNotGreaterThan").get(0).setDQ8("value", new Date(System.currentTimeMillis() / 3));
        request.getList("dateNotGreaterThan").get(0).setDQ8("comparisonValue", Calendar.getInstance().getTime());

        final StructureBuilder replyBuilder = StructureBuilder.getBuilder();
        replyBuilder.addI2("f8GreaterThanResponse").addI2("f8NotGreaterThanResponse").addI2("i4GreaterThanResponse")
                .addI2("i4NotGreaterThanResponse").addI2("i2GreaterThanResponse").addI2("i2NotGreaterThanResponse")
                .addI2("vcGreaterThanResponse").addI2("vcNotGreaterThanResponse").addI2("dateGreaterThanResponse")
                .addI2("dateNotGreaterThanResponse");
        final Record reply = RecordFactory.create("reply", replyBuilder.build());

        // Set all of the fields to an invalid boolean value to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("f8GreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("f8NotGreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i4GreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i4NotGreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i2GreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i2NotGreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("vcGreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("vcNotGreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("dateGreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("dateNotGreaterThanResponse")).isFalse();
    }

    /**
     * Test the cclutAssertNotContains subroutine.
     */
    @Test
    public void testNotContains() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotContains.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Structure list = StructureBuilder.getBuilder().addVC("string").addVC("substring").build();
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addList("contains", list, 1).addList("notContains", list, 1).build());
        request.getList("contains").get(0).setVC("string", "abcdefg");
        request.getList("contains").get(0).setVC("substring", "abc");
        request.getList("notContains").get(0).setVC("string", "abc");
        request.getList("notContains").get(0).setVC("substring", "xyz");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("containsResponse").addI2("notContainsResponse").build());
        // Fill the reply with non-boolean values to avoid false positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("containsResponse")).isFalse();
        assertThat(reply.getI2Boolean("notContainsResponse")).isTrue();
    }

    /**
     * Test the cclutAssertAlmostNotEqual subroutine.
     */
    @Test
    public void testNotAlmostEqual() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotAlmostEqual.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestStructBuilder = StructureBuilder.getBuilder();
        final Structure listBuilder = StructureBuilder.getBuilder().addF8("expected").addF8("actual").addF8("delta")
                .build();
        requestStructBuilder.addList("almostEqual", listBuilder, 1);
        requestStructBuilder.addList("notAlmostEqual", listBuilder, 1);
        final Record request = RecordFactory.create("request", requestStructBuilder.build());
        request.getList("almostEqual").get(0).setF8("expected", 3.0);
        request.getList("almostEqual").get(0).setF8("actual", 2.8);
        request.getList("almostEqual").get(0).setF8("delta", 0.5);

        request.getList("notAlmostEqual").get(0).setF8("expected", 3.0);
        request.getList("notAlmostEqual").get(0).setF8("actual", 2.0);
        request.getList("notAlmostEqual").get(0).setF8("delta", 0.5);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("almostEqualResponse").addI2("notAlmostEqualResponse").build());

        // Set all reply fields to invalid boolean values to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("almostEqualResponse")).isFalse();
        assertThat(reply.getI2Boolean("notAlmostEqualResponse")).isTrue();
    }

    /**
     * Test the cclutAssertNotEndsWith subroutine.
     */
    @Test
    public void testNotEndsWith() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotEndsWith.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Structure list = StructureBuilder.getBuilder().addVC("string").addVC("substring").build();
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addList("endsWith", list, 1).addList("notEndsWith", list, 1).build());
        request.getList("endsWith").get(0).setVC("string", "abcdefg");
        request.getList("endsWith").get(0).setVC("substring", "efg");
        request.getList("notEndsWith").get(0).setVC("string", "abcdefg");
        request.getList("notEndsWith").get(0).setVC("substring", "xyz");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("endsWithResponse").addI2("notEndsWithResponse").build());
        // Fill the reply with non-boolean values to avoid false positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("endsWithResponse")).isFalse();
        assertThat(reply.getI2Boolean("notEndsWithResponse")).isTrue();
    }

    /**
     * Test the cclutAssert*NotEqual subroutines.
     */
    @Test
    public void testNotEqual() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotEqual.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestBuilder = StructureBuilder.getBuilder();
        requestBuilder.addList("f8Equal", StructureBuilder.getBuilder().addF8("value").build(), 1);
        requestBuilder.addList("f8Inequal", StructureBuilder.getBuilder().addF8("expected").addF8("actual").build(), 1);
        requestBuilder.addList("i4Equal", StructureBuilder.getBuilder().addI4("value").build(), 1);
        requestBuilder.addList("i4Inequal", StructureBuilder.getBuilder().addI4("expected").addI4("actual").build(), 1);
        requestBuilder.addList("i2Equal", StructureBuilder.getBuilder().addI2("value").build(), 1);
        requestBuilder.addList("i2Inequal", StructureBuilder.getBuilder().addI4("expected").addI4("actual").build(), 1);
        requestBuilder.addList("vcEqual", StructureBuilder.getBuilder().addVC("value").build(), 1);
        requestBuilder.addList("vcInequal", StructureBuilder.getBuilder().addVC("expected").addVC("actual").build(), 1);
        requestBuilder.addList("dateEqual", StructureBuilder.getBuilder().addDQ8("value").build(), 1);
        requestBuilder.addList("dateInequal", StructureBuilder.getBuilder().addDQ8("expected").addDQ8("actual").build(),
                1);
        final Record request = RecordFactory.create("request", requestBuilder.build());

        request.getList("f8Equal").get(0).setF8("value", 2.5);
        request.getList("f8Inequal").get(0).setF8("expected", 2.0);
        request.getList("f8Inequal").get(0).setF8("actual", 1.5);
        request.getList("i4Equal").get(0).setI4("value", 3);
        request.getList("i4Inequal").get(0).setI4("expected", 3);
        request.getList("i4Inequal").get(0).setI4("actual", 2);
        request.getList("i2Equal").get(0).setI2("value", (short) 1);
        request.getList("i2Inequal").get(0).setI2("expected", (short) 1);
        request.getList("i2Inequal").get(0).setI2("actual", (short) 2);
        request.getList("vcEqual").get(0).setVC("value", "test");
        request.getList("vcInequal").get(0).setVC("expected", "test123");
        request.getList("vcInequal").get(0).setVC("actual", "321tset");
        request.getList("dateEqual").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateInequal").get(0).setDQ8("expected", Calendar.getInstance().getTime());
        request.getList("dateInequal").get(0).setDQ8("actual", new Date(System.currentTimeMillis() / 3));

        final StructureBuilder replyBuilder = StructureBuilder.getBuilder();
        replyBuilder.addI2("f8EqualResponse").addI2("f8InequalResponse").addI2("i4EqualResponse")
                .addI2("i4InequalResponse").addI2("i2EqualResponse").addI2("i2InequalResponse").addI2("vcEqualResponse")
                .addI2("vcInequalResponse").addI2("dateEqualResponse").addI2("dateInequalResponse");
        final Record reply = RecordFactory.create("reply", replyBuilder.build());

        // Set all of the fields to an invalid boolean value to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("f8EqualResponse")).isFalse();
        assertThat(reply.getI2Boolean("f8InequalResponse")).isTrue();
        assertThat(reply.getI2Boolean("i4EqualResponse")).isFalse();
        assertThat(reply.getI2Boolean("i4InequalResponse")).isTrue();
        assertThat(reply.getI2Boolean("i2EqualResponse")).isFalse();
        assertThat(reply.getI2Boolean("i2InequalResponse")).isTrue();
        assertThat(reply.getI2Boolean("vcEqualResponse")).isFalse();
        assertThat(reply.getI2Boolean("vcInequalResponse")).isTrue();
        assertThat(reply.getI2Boolean("dateEqualResponse")).isFalse();
        assertThat(reply.getI2Boolean("dateInequalResponse")).isTrue();
    }

    /**
     * Test all of the assert*LessThan subroutines.
     */
    @Test
    public void testNotLessThan() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotLessThan.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestBuilder = StructureBuilder.getBuilder();
        requestBuilder.addList("f8LessThan", StructureBuilder.getBuilder().addF8("value").build(), 1);
        requestBuilder.addList("f8NotLessThan",
                StructureBuilder.getBuilder().addF8("value").addF8("comparisonValue").build(), 1);
        requestBuilder.addList("i4LessThan", StructureBuilder.getBuilder().addI4("value").build(), 1);
        requestBuilder.addList("i4NotLessThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("i2LessThan", StructureBuilder.getBuilder().addI2("value").build(), 1);
        requestBuilder.addList("i2NotLessThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("vcLessThan", StructureBuilder.getBuilder().addVC("value").build(), 1);
        requestBuilder.addList("vcNotLessThan",
                StructureBuilder.getBuilder().addVC("value").addVC("comparisonValue").build(), 1);
        requestBuilder.addList("dateLessThan", StructureBuilder.getBuilder().addDQ8("value").build(), 1);
        requestBuilder.addList("dateNotLessThan",
                StructureBuilder.getBuilder().addDQ8("value").addDQ8("comparisonValue").build(), 1);
        final Record request = RecordFactory.create("request", requestBuilder.build());

        request.getList("f8LessThan").get(0).setF8("value", 2.5);
        request.getList("f8NotLessThan").get(0).setF8("value", 2.0);
        request.getList("f8NotLessThan").get(0).setF8("comparisonValue", 1.5);
        request.getList("i4LessThan").get(0).setI4("value", 3);
        request.getList("i4NotLessThan").get(0).setI4("value", 3);
        request.getList("i4NotLessThan").get(0).setI4("comparisonValue", 2);
        request.getList("i2LessThan").get(0).setI2("value", (short) 1);
        request.getList("i2NotLessThan").get(0).setI2("value", (short) 2);
        request.getList("i2NotLessThan").get(0).setI2("comparisonValue", (short) 1);
        request.getList("vcLessThan").get(0).setVC("value", "test");
        request.getList("vcNotLessThan").get(0).setVC("value", "test123");
        request.getList("vcNotLessThan").get(0).setVC("comparisonValue", "321tset");
        request.getList("dateLessThan").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateNotLessThan").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateNotLessThan").get(0).setDQ8("comparisonValue", new Date(System.currentTimeMillis() / 3));

        final StructureBuilder replyBuilder = StructureBuilder.getBuilder();
        replyBuilder.addI2("f8LessThanResponse").addI2("f8NotLessThanResponse").addI2("i4LessThanResponse")
                .addI2("i4NotLessThanResponse").addI2("i2LessThanResponse").addI2("i2NotLessThanResponse")
                .addI2("vcLessThanResponse").addI2("vcNotLessThanResponse").addI2("dateLessThanResponse")
                .addI2("dateNotLessThanResponse");
        final Record reply = RecordFactory.create("reply", replyBuilder.build());

        // Set all of the fields to an invalid boolean value to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("f8LessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("f8NotLessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i4LessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i4NotLessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i2LessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i2NotLessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("vcLessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("vcNotLessThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("dateLessThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("dateNotLessThanResponse")).isTrue();
    }

    /**
     * Test all of the assert*LessThan subroutines.
     */
    @Test
    public void testNotGreaterThan() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotGreaterThan.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final StructureBuilder requestBuilder = StructureBuilder.getBuilder();
        requestBuilder.addList("f8GreaterThan", StructureBuilder.getBuilder().addF8("value").build(), 1);
        requestBuilder.addList("f8NotGreaterThan",
                StructureBuilder.getBuilder().addF8("value").addF8("comparisonValue").build(), 1);
        requestBuilder.addList("i4GreaterThan", StructureBuilder.getBuilder().addI4("value").build(), 1);
        requestBuilder.addList("i4NotGreaterThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("i2GreaterThan", StructureBuilder.getBuilder().addI2("value").build(), 1);
        requestBuilder.addList("i2NotGreaterThan",
                StructureBuilder.getBuilder().addI4("value").addI4("comparisonValue").build(), 1);
        requestBuilder.addList("vcGreaterThan", StructureBuilder.getBuilder().addVC("value").build(), 1);
        requestBuilder.addList("vcNotGreaterThan",
                StructureBuilder.getBuilder().addVC("value").addVC("comparisonValue").build(), 1);
        requestBuilder.addList("dateGreaterThan", StructureBuilder.getBuilder().addDQ8("value").build(), 1);
        requestBuilder.addList("dateNotGreaterThan",
                StructureBuilder.getBuilder().addDQ8("value").addDQ8("comparisonValue").build(), 1);
        final Record request = RecordFactory.create("request", requestBuilder.build());

        request.getList("f8GreaterThan").get(0).setF8("value", 2.5);
        request.getList("f8NotGreaterThan").get(0).setF8("value", 1.5);
        request.getList("f8NotGreaterThan").get(0).setF8("comparisonValue", 2.0);
        request.getList("i4GreaterThan").get(0).setI4("value", 3);
        request.getList("i4NotGreaterThan").get(0).setI4("value", 2);
        request.getList("i4NotGreaterThan").get(0).setI4("comparisonValue", 3);
        request.getList("i2GreaterThan").get(0).setI2("value", (short) 2);
        request.getList("i2NotGreaterThan").get(0).setI2("value", (short) 1);
        request.getList("i2NotGreaterThan").get(0).setI2("comparisonValue", (short) 2);
        request.getList("vcGreaterThan").get(0).setVC("value", "test");
        request.getList("vcNotGreaterThan").get(0).setVC("value", "test12");
        request.getList("vcNotGreaterThan").get(0).setVC("comparisonValue", "test123");
        request.getList("dateGreaterThan").get(0).setDQ8("value", Calendar.getInstance().getTime());
        request.getList("dateNotGreaterThan").get(0).setDQ8("value", new Date(System.currentTimeMillis() / 3));
        request.getList("dateNotGreaterThan").get(0).setDQ8("comparisonValue", Calendar.getInstance().getTime());

        final StructureBuilder replyBuilder = StructureBuilder.getBuilder();
        replyBuilder.addI2("f8GreaterThanResponse").addI2("f8NotGreaterThanResponse").addI2("i4GreaterThanResponse")
                .addI2("i4NotGreaterThanResponse").addI2("i2GreaterThanResponse").addI2("i2NotGreaterThanResponse")
                .addI2("vcGreaterThanResponse").addI2("vcNotGreaterThanResponse").addI2("dateGreaterThanResponse")
                .addI2("dateNotGreaterThanResponse");
        final Record reply = RecordFactory.create("reply", replyBuilder.build());

        // Set all of the fields to an invalid boolean value to avoid false
        // positives
        for (Field field : reply.getStructure().getFields())
            reply.setI2(field.getName(), (short) 3);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("f8GreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("f8NotGreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i4GreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i4NotGreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("i2GreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("i2NotGreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("vcGreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("vcNotGreaterThanResponse")).isTrue();
        assertThat(reply.getI2Boolean("dateGreaterThanResponse")).isFalse();
        assertThat(reply.getI2Boolean("dateNotGreaterThanResponse")).isTrue();
    }

    /**
     * Test the cclutAssertNotStartsWith subroutine.
     */
    @Test
    public void testNotStartsWith() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestNotStartsWith.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Structure list = StructureBuilder.getBuilder().addVC("string").addVC("substring").build();
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addList("startsWith", list, 1).addList("notStartsWith", list, 1).build());
        request.getList("startsWith").get(0).setVC("string", "abcdefg");
        request.getList("startsWith").get(0).setVC("substring", "abc");
        request.getList("notStartsWith").get(0).setVC("string", "abc");
        request.getList("notStartsWith").get(0).setVC("substring", "xyz");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("startsWithResponse").addI2("notStartsWithResponse").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("startsWithResponse")).isFalse();
        assertThat(reply.getI2Boolean("notStartsWithResponse")).isTrue();
    }

    /**
     * Test the cclutAssertStartsWith subroutine.
     */
    @Test
    public void testStartsWith() {
        final File scriptFile = new File(getTestResourceDirectory(), "cclutAssertTestStartsWith.prg");
        final String scriptName = StringUtils.stripFilenameExtension(scriptFile.getName());

        final Structure list = StructureBuilder.getBuilder().addVC("string").addVC("substring").build();
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addList("startsWith", list, 1).addList("notStartsWith", list, 1).build());
        request.getList("startsWith").get(0).setVC("string", "abcdefg");
        request.getList("startsWith").get(0).setVC("substring", "abc");
        request.getList("notStartsWith").get(0).setVC("string", "abc");
        request.getList("notStartsWith").get(0).setVC("substring", "xyz");

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addI2("startsWithResponse").addI2("notStartsWithResponse").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(scriptFile).commit();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();
        executor.addScriptDropper(scriptName).commit();
        executor.execute();

        assertThat(reply.getI2Boolean("startsWithResponse")).isTrue();
        assertThat(reply.getI2Boolean("notStartsWithResponse")).isFalse();
    }
}
