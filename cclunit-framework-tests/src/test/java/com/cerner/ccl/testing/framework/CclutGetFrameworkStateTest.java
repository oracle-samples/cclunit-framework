package com.cerner.ccl.testing.framework;

import static org.fest.assertions.Assertions.assertThat;

import java.io.ByteArrayInputStream;

import org.jdom.Document;
import org.jdom.input.SAXBuilder;
import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * Tests for the {@code cclut_get_framework_state} CCL script.
 * 
 * @author Joshua Hyde
 * 
 */

public class CclutGetFrameworkStateTest extends AbstractCclutTest {
    @Autowired
    @Qualifier("projectVersion")
    private String projectVersion;

    /**
     * Test the retrieval of the version.
     * <p />
     * This test assumes that the project version of the testing framework matches this project's version.
     * 
     * @throws Exception
     *             If any errors occur during the test run.
     */
    @Test
    public void testGetVersion() throws Exception {
        final CclExecutor executor = getCclExecutor();
        final Record testReply = RecordFactory.create("testReply",
                StructureBuilder.getBuilder().addVC("state").build());
        executor.addScriptExecution("cclut_get_framework_state").withReplace("reply", testReply).commit();
        executor.execute();

        final Document document = new SAXBuilder()
                .build(new ByteArrayInputStream(testReply.getVC("state").getBytes("utf-8")));
        assertThat(document.getRootElement().getChildText("VERSION")).isEqualTo(projectVersion);
    }
}
