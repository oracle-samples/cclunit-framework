package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.util.Locale;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Unit tests for the {@code cclut_xml_functions.inc} file.
 *
 * @author Joshua Hyde
 *
 */

public class CclutXmlFunctionsTest extends AbstractCclutTest {
    private final File transformScriptFile = new File(getTestResourceDirectory(), "cclutXmlTransformTest.prg");
    private final String transformScriptName = StringUtils.stripFilenameExtension(transformScriptFile.getName());

    private final File parseScriptFile = new File(getTestResourceDirectory(), "cclutParseXmlTest.prg");
    private final String parseScriptName = StringUtils.stripFilenameExtension(parseScriptFile.getName());

    /**
     * Compile the testing scripts.
     */
    @Before
    public void setUp() {
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(transformScriptFile).commit();
        executor.addScriptCompiler(parseScriptFile).commit();
        executor.execute();
    }

    /**
     * Drop the testing scripts.
     */
    @After
    public void tearDown() {
        final CclExecutor executor = getCclExecutor();
        executor.addScriptDropper(transformScriptName).commit();
        executor.addScriptDropper(parseScriptName).commit();
        executor.execute();
    }

    /**
     * Test the transformation of a value to an XML tag.
     */
    @Test
    public void testCclutTransform() {
        final String xmlTagName = "tag";
        final String xmlValue = "value";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("xmlValue").addVC("xmlTagName").build());
        request.setVC("xmlValue", xmlValue);
        request.setVC("xmlTagName", xmlTagName);
        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addVC("xml").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(transformScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getVC("xml")).isEqualTo("<" + xmlTagName.toUpperCase(Locale.getDefault()) + ">" + xmlValue
                + "</" + xmlTagName.toUpperCase(Locale.getDefault()) + ">");
    }

    /**
     * Test the retrieval of a value from an XML tag.
     */
    @Test
    public void testCclutParseXmlValue() {
        final String xmlTag = "TEST";
        final String xmlValue = "value";
        final String xml = "<" + xmlTag + ">" + xmlValue + "</" + xmlTag + ">";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("xml").addVC("xmlTagName").addI4("startPos").build());
        request.setVC("xml", xml);
        request.setVC("xmlTagName", xmlTag);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addVC("xmlValue").addI2("foundInd").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(parseScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getI2Boolean("foundInd")).isTrue();
        assertThat(reply.getVC("xmlValue")).isEqualTo(xmlValue);
    }

    /**
     * If the XML value is wrapped in a CDATA tag, it should be extracted without the CDATA notation.
     */
    @Test
    public void testCclutParseXmlValueCDATA() {
        final String xmlTag = "TEST";
        final String xmlValue = "value";
        final String xml = "<" + xmlTag + "><![CDATA[" + xmlValue + "]]></" + xmlTag + ">";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("xml").addVC("xmlTagName").addI4("startPos").build());
        request.setVC("xml", xml);
        request.setVC("xmlTagName", xmlTag);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addVC("xmlValue").addI2("foundInd").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(parseScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getI2Boolean("foundInd")).isTrue();
        assertThat(reply.getVC("xmlValue")).isEqualTo(xmlValue);
    }

    /**
     * If prompted for <i>not</i> the first value for a given XML tag, then the desired value should be returned.
     */
    @Test
    public void testCclutParseXmlValueLaterValue() {
        final String xml = "<TEST>valueA</TEST><TEST>valueB</TEST>";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("xml").addVC("xmlTagName").addI4("startPos").build());
        request.setVC("xml", xml);
        request.setVC("xmlTagName", "TEST");
        request.setI4("startPos", 2);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addVC("xmlValue").addI2("foundInd").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(parseScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getI2Boolean("foundInd")).isTrue();
        assertThat(reply.getVC("xmlValue")).isEqualTo("valueB");
    }

    /**
     * If the opening XML tag is not found, then it should be indicated that nothing was found.
     */
    @Test
    public void testCclutParseXmlValueOpenTagNotFound() {
        final String xmlTag = "TEST";
        final String xmlValue = "value";
        final String xml = xmlValue + "</" + xmlTag + ">";

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("xml").addVC("xmlTagName").addI4("startPos").build());
        request.setVC("xml", xml);
        request.setVC("xmlTagName", xmlTag);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addVC("xmlValue").addI2("foundInd").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(parseScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getI2Boolean("foundInd")).isFalse();
        assertThat(reply.getVC("xmlValue")).isEmpty();
    }

    /**
     * If the closing XML tag is not found, then it should be indicated that nothing was found.
     */
    @Test
    public void testCclutParseXmlValueCloseTagNotFound() {
        final String xmlTag = "TEST";
        final String xmlValue = "value";
        final String xml = "<" + xmlTag + ">" + xmlValue;

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("xml").addVC("xmlTagName").addI4("startPos").build());
        request.setVC("xml", xml);
        request.setVC("xmlTagName", xmlTag);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addVC("xmlValue").addI2("foundInd").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(parseScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getI2Boolean("foundInd")).isFalse();
        assertThat(reply.getVC("xmlValue")).isEmpty();
    }
}