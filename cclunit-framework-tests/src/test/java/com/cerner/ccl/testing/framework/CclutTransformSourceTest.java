package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.util.List;
import java.util.Locale;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.junit.Before;
import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.enums.OutputType;
import com.cerner.ccl.j4ccl.record.DynamicRecordList;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.Structure;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Tests for the {@code cclut_transform_source.prg} script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutTransformSourceTest extends AbstractCclutTest {
    private final String scriptName = "cclut_transform_source";
    private Record request;
    private Record reply;

    /**
     * Create the request and reply record structures for each test.
     */
    @Before
    public void setUp() {
        final Structure requestStructure = StructureBuilder.getBuilder().addVC("programName")
                .addVC("compileDate")
                .addDynamicList("source", StructureBuilder.getBuilder().addVC("line").build()).build();
        final Structure replyStructure = StructureBuilder.getBuilder().addVC("xml").build();

        request = RecordFactory.create("request", requestStructure);
        reply = RecordFactory.create("reply", replyStructure);
    }

    /**
     * Test the transform of basic lines.
     */
    @Test
    public void testTransformLines() {
        final String[] expectedText = new String[] { "line.a", "line.b",
                "<!-- whoo hoo I would break without CDATA -->" };

        final DynamicRecordList source = request.getDynamicList("source");
        for (final String text : expectedText)
            source.addItem().setVC("line", text);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("cclutRequest", request).withReplace("cclutReply", reply)
                .commit();
        executor.execute();

        final List<Element> lines = getLines(reply.getVC("xml"));
        assertThat(lines).hasSize(expectedText.length);
        for (int i = 0; i < expectedText.length; i++) {
            final Element lineElement = lines.get(i);
            assertThat(lineElement.getChild("NBR").getText()).isEqualTo(Integer.toString(i + 1));
            assertThat(lineElement.getChild("TEXT").getText()).isEqualTo(expectedText[i]);
        }
    }

    /**
     * A duplicate variable declaration in {@code cclut_xml_functions.inc} causes an informational message to appear.
     * This verifies that the issue has been corrected.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testTransformDuplicateVariableDeclaration() throws Exception {
        final DynamicRecordList source = request.getDynamicList("source");
        source.addItem().setVC("line", "test");

        final ByteArrayOutputStream stream = new ByteArrayOutputStream();
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("cclutRequest", request).withReplace("cclutReply", reply)
                .commit();
        executor.setOutputStream(stream, OutputType.CCL_SESSION);
        executor.execute();

        assertThat(new String(stream.toByteArray(), "utf-8"))
                .doesNotMatch("The variable \\(.*\\) has already been defined.");
    }

    /**
     * Given an indicator that the line was for the end of an include file, everything after the marker should be
     * returned in an {@code <END_OF_INC />} tag.
     */
    @Test
    public void testTransformLinesEndInc() {
        final String expectedText = "i am the expected text";

        request.getDynamicList("source").addItem().setVC("line", ";;;;CCLUT_END_INC_FILE " + expectedText);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("cclutRequest", request).withReplace("cclutReply", reply)
                .commit();
        executor.execute();

        final List<Element> lines = getLines(reply.getVC("xml"));
        assertThat(lines).hasSize(1);
        final Element endIncElement = lines.get(0).getChild("END_OF_INC");
        assertThat(endIncElement).isNotNull();
        assertThat(endIncElement.getText()).isEqualTo(expectedText);
    }

    /**
     * Given an indicator that the line was the start of an include file, everything after the marker should returned in
     * a {@code <START_OF_INC />} tag.
     */
    @Test
    public void testTransformLinesStartInc() {
        final String expectedText = "i am the expected text";

        request.getDynamicList("source").addItem().setVC("line", ";;;;CCLUT_START_INC_FILE " + expectedText);

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("cclutRequest", request).withReplace("cclutReply", reply)
                .commit();
        executor.execute();

        final List<Element> lines = getLines(reply.getVC("xml"));
        assertThat(lines).hasSize(1);
        final Element endIncElement = lines.get(0).getChild("START_OF_INC");
        assertThat(endIncElement).isNotNull();
        assertThat(endIncElement.getText()).isEqualTo(expectedText);
    }

    /**
     * The provided listing name should be injected into response.
     */
    @Test
    public void testTransformListingName() {
        final String listingName = "i.am.the.listing.name";

        request.setVC("programName", listingName);
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("cclutRequest", request).withReplace("cclutReply", reply)
                .commit();
        executor.execute();

        final Document document = getDocument(reply.getVC("xml"));
        final Element listingNameElement = document.getRootElement().getChild("LISTING_NAME");
        assertThat(listingNameElement).isNotNull();
        assertThat(listingNameElement.getText()).isEqualTo(listingName.toUpperCase(Locale.getDefault()));
    }

    /**
     * The provided compilation date/time should be injected into the response.
     */
    @Test
    public void testTransformCompileDate() {
        final String compilationDate = "i.am./the/compilation/date";

        request.setVC("compileDate", compilationDate);
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(scriptName).withReplace("cclutRequest", request).withReplace("cclutReply", reply)
                .commit();
        executor.execute();

        final Document document = getDocument(reply.getVC("xml"));
        final Element listingNameElement = document.getRootElement().getChild("COMPILE_DATE");
        assertThat(listingNameElement).isNotNull();
        assertThat(listingNameElement.getText()).isEqualTo(compilationDate.toUpperCase(Locale.getDefault()));
    }

    /**
     * Build a document object out of the given XML.
     *
     * @param xml
     *            The XML to be turned into a document.
     * @return A {@link Document} representing the given XML.
     */
    private Document getDocument(final String xml) {
        try {
            return new SAXBuilder().build(new StringReader(xml));
        } catch (final Exception e) {
            throw new RuntimeException("Failed to create document.", e);
        }
    }

    /**
     * Get the contents of the {@code <LINES />} element within the given XML document.
     *
     * @param xml
     *            The XML from which the lines are to be retrieved.
     * @return A {@link List} of {@link Element} objects representing the contents of the {@code <LINES />} elements in
     *         the given XML.
     */
    @SuppressWarnings("unchecked")
    private List<Element> getLines(final String xml) {
        final Element linesElement = getDocument(xml).getRootElement().getChild("LINES");
        return linesElement.getChildren("LINE");
    }
}
