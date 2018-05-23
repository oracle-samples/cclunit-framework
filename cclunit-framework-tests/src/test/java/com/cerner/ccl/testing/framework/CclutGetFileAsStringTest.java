package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.File;
import java.net.URI;
import java.nio.charset.Charset;
import java.util.Collections;

import org.apache.commons.io.FileUtils;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.springframework.util.StringUtils;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;
import com.cerner.ftp.Uploader;
import com.cerner.ftp.data.factory.FileRequestFactory;

/**
 * Unit tests for the {@code cclut_get_file_as_string.inc} file.
 *
 * @author Joshua Hyde
 *
 */

public class CclutGetFileAsStringTest extends AbstractCclutTest {
    private final File retrieverScriptFile = new File(getTestResourceDirectory(), "cclutGetFileAsStringTest.prg");
    private final String retrieverScriptName = StringUtils.stripFilenameExtension(retrieverScriptFile.getName());

    /**
     * Compile the script used to interact with the test subroutine.
     */
    @Before
    public void setUp() {
        final CclExecutor executor = getCclExecutor();
        executor.addScriptCompiler(retrieverScriptFile).commit();
        executor.execute();
    }

    /**
     * Drop the retriever script.
     */
    @After
    public void tearDown() {
        final CclExecutor executor = getCclExecutor();
        executor.addScriptDropper(retrieverScriptName).commit();
        executor.execute();
    }

    /**
     * Test the retrieval of a file as a string.
     *
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testGetFileAsString() throws Exception {
        final File textFile = new File(getTestResourceDirectory(), "testTextFile.txt");
        final String cerTempLocation = getLogicalRetriever().getLogicalValue("CER_TEMP");
        final URI remoteFileLocation = URI.create(cerTempLocation + "/" + textFile.getName());

        // Upload a file
        final Uploader uploader = getUploader();
        uploader.upload(Collections.singleton(FileRequestFactory.create(textFile.toURI(), remoteFileLocation)));

        // Read the file
        final Record request = RecordFactory.create("request", StructureBuilder.getBuilder().addVC("filename").build());
        request.setVC("filename", remoteFileLocation.toString());
        final Record reply = RecordFactory.create("reply", StructureBuilder.getBuilder().addVC("text").build());
        final CclExecutor executor = getCclExecutor();
        executor.addScriptExecution(retrieverScriptName).withReplace("request", request).withReplace("reply", reply)
                .commit();
        executor.execute();

        assertThat(reply.getVC("text").trim())
                .isEqualTo(FileUtils.readFileToString(textFile, Charset.forName("ascii")));
    }
}
