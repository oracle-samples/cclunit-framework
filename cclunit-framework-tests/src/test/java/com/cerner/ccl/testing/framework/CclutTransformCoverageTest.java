package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.io.StringReader;
import java.net.URI;
import java.security.PrivilegedAction;
import java.util.List;

import javax.security.auth.Subject;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;
import com.cerner.ccl.j4ccl.util.CclResourceUploader;

/**
 * Tests for the {@code cclut_transform_coverage.prg} script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutTransformCoverageTest extends AbstractCclutTest {
    /**
     * Test the transformation of coverage to XML.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testTransformCoverage() throws Exception {
        final String fileName = "cclut_sample_code_coverage.txt";
        final CclResourceUploader uploader = getResourceUploader();
        final File toUpload = new File(getTestResourceDirectory(), fileName);
        uploader.queueUpload(toUpload);
        final String targetLocation = (Subject.doAs(getSubject(), new PrivilegedAction<URI>() {
            public URI run() {
                return uploader.upload().get(toUpload);
            }
        })).toASCIIString();

        final Record request = RecordFactory.create("testRequest",
                StructureBuilder.getBuilder().addVC("path").addVC("filename").build());
        request.setVC("path", targetLocation.substring(0, targetLocation.indexOf(fileName)));
        request.setVC("filename", fileName);

        final Record reply = RecordFactory.create("testReply", StructureBuilder.getBuilder().addVC("xml").build());

        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution("cclut_transform_coverage").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        executor.execute();

        final Document document = new SAXBuilder().build(new StringReader(reply.getVC("xml")));
        final Element lineElement = document.getRootElement().getChild("LINES");
        assertThat(lineElement).isNotNull();
        @SuppressWarnings("unchecked")
        final List<Element> lines = lineElement.getChildren("LINE");
        assertThat(lines).hasSize(5);

        assertThat(lines.get(0).getChild("NBR").getText()).isEqualTo("1");
        assertThat(lines.get(0).getChild("TYPE").getText()).isEqualTo("N");

        assertThat(lines.get(1).getChild("NBR").getText()).isEqualTo("2");
        assertThat(lines.get(1).getChild("TYPE").getText()).isEqualTo("N");

        assertThat(lines.get(2).getChild("NBR").getText()).isEqualTo("3");
        assertThat(lines.get(2).getChild("TYPE").getText()).isEqualTo("C");

        assertThat(lines.get(3).getChild("NBR").getText()).isEqualTo("4");
        assertThat(lines.get(3).getChild("TYPE").getText()).isEqualTo("U");

        assertThat(lines.get(4).getChild("NBR").getText()).isEqualTo("5");
        assertThat(lines.get(4).getChild("TYPE").getText()).isEqualTo("N");
    }
}
