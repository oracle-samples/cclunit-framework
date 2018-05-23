package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.net.URI;
import java.security.PrivilegedAction;

import javax.security.auth.Subject;

import org.junit.Test;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.DynamicRecordList;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;
import com.cerner.ccl.j4ccl.util.CclResourceUploader;

/**
 * Tests for the {@code cclut_parse_source.prg} script.
 *
 * @author Joshua Hyde
 *
 */

public class CclutParseSourceTest extends AbstractCclutTest {
    /**
     * Test the parsing of a compiled object's.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testSourceTransformation() throws Exception {
        final String scriptName = "cclutTestCompileOutput";

        // Compile the script and retrieve the output
        final File localListing = File.createTempFile("local-listing", null);
        final CclExecutor compileExecutor = getCclExecutor();
        compileExecutor.addScriptCompiler(new File(getTestResourceDirectory(), "cclutTestCompileOutput.prg"))
                .withListingOutput(localListing).commit();
        compileExecutor.addScriptDropper(scriptName).commit();
        compileExecutor.execute();

        // Upload the listing to a known location to be parsed
        final CclResourceUploader uploader = getResourceUploader();
        uploader.queueUpload(localListing);
        final URI remoteListing = Subject.doAs(getSubject(), new PrivilegedAction<URI>() {
            public URI run() {
                return uploader.upload().get(localListing);
            }
        });

        // Now, parse the source into XML
        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("programName").addVC("path").addVC("filename").build());
        request.setVC("programName", scriptName);
        final String fullRemotePath = remoteListing.toASCIIString();
        request.setVC("path", fullRemotePath.substring(0, fullRemotePath.lastIndexOf('/') + 1));
        request.setVC("filename", fullRemotePath.substring(fullRemotePath.lastIndexOf('/') + 1));

        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder()
                .addDynamicList("source", StructureBuilder.getBuilder().addVC("line").build()).build());

        final CclExecutor parserExecutor = getCclExecutor();
        parserExecutor.addScriptExecution("cclut_parse_source").withReplace("cclutRequest", request)
                .withReplace("cclutReply", reply).commit();
        parserExecutor.execute();

        final DynamicRecordList sourceList = reply.getDynamicList("source");
        assertThat(sourceList.get(0).getVC("line")).isEqualTo("create program cclutTestCompileOutput");
        assertThat(sourceList.get(1).getVC("line")).isEqualTo("call echo('Just a simple program to compile')");
        assertThat(sourceList.get(2).getVC("line")).isEqualTo("end");
        assertThat(sourceList.get(3).getVC("line")).isEqualTo("go");
    }
}
