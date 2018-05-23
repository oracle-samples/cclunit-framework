package com.cerner.ccl.testing.framework;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import javax.security.auth.Subject;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.TestExecutionListeners;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.DependencyInjectionTestExecutionListener;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.TerminalProperties;
import com.cerner.ccl.j4ccl.enums.OutputType;
import com.cerner.ccl.j4ccl.util.CclResourceUploader;
import com.cerner.ccl.testing.framework.internal.JaasCclExecutor;
import com.cerner.ccl.testing.framework.internal.JaasResourceUploader;
import com.cerner.ccl.testing.framework.internal.LogicalRetriever;
import com.cerner.ftp.Downloader;
import com.cerner.ftp.Uploader;
import com.cerner.ftp.data.FtpProduct;
import com.cerner.ftp.sftp.SftpUploader;

/**
 * Base configuration of a test used to unit test components of the CCL Testing Framework.
 *
 * @author Joshua Hyde
 *
 */

@RunWith(SpringJUnit4ClassRunner.class)
@TestExecutionListeners(listeners = { DependencyInjectionTestExecutionListener.class })
@ContextConfiguration("classpath:/META-INF/spring/applicationContext-cclutTests.xml")
public abstract class AbstractCclutTest {

    private final File testResourcesDirectory = new File("src/test/resources/testCode/", getClass().getSimpleName());

    @Autowired
    private FtpProduct ftpProduct;

    @Autowired
    private LogicalRetriever logicalRetriever;

    @Autowired
    @Qualifier("doDebug")
    private Boolean doDebug;

    @Autowired
    private Subject subject;


    /**
     * Set credential properties based on server properties.
     *
     * @throws IOException
     *             Not expected.
     */
    @BeforeClass
    public static void setupClass() throws IOException {
        Properties prop = new Properties();
        try (InputStream stream = AbstractCclutTest.class.getResourceAsStream("/spring/build.properties")) {
            prop.load(stream);
            String hostCredentialsId = prop.getProperty("ccl-hostCredentialsId");
            String cclCredentialsId = prop.getProperty("ccl-frontendCredentialsId");
            if (hostCredentialsId != null && !hostCredentialsId.isEmpty()) {
                String hostUsername = prop
                        .getProperty(String.format("settings.servers.%s.username", hostCredentialsId));
                String hostPassword = prop
                        .getProperty(String.format("settings.servers.%s.password", hostCredentialsId));
                System.setProperty("ccl-hostUsername", hostUsername);
                System.setProperty("ccl-hostPassword", hostPassword);
            }
            if (cclCredentialsId != null && !cclCredentialsId.isEmpty()) {
                String cclUsername = prop.getProperty(String.format("settings.servers.%s.username", cclCredentialsId));
                String cclPassword = prop.getProperty(String.format("settings.servers.%s.password", cclCredentialsId));
                System.setProperty("ccl-domainUsername", cclUsername);
                System.setProperty("ccl-domainPassword", cclPassword);
            }
            String cclHost = prop.getProperty("ccl-host");
            String cclDomain = prop.getProperty("ccl-domain");
            String hostUsername = System.getProperty("ccl-hostUsername");
            TerminalProperties
            .setGlobalTerminalProperties(TerminalProperties.getNewBuilder()
                    .setOsPromptPattern(
                                    TerminalProperties.constructDefaultOsPromptPattern(cclHost, cclDomain,
                                            hostUsername))
                            .setSpecifyDebugCcl(true).setLogfileLocation("target/ccl-log/ITest.log")
                            .setExpectationTimeout(15000).setSpecifyDebugCcl(false).build());
        }
    }

    /**
     * Final cleanup.
     *
     * @throws Exception
     *             Not expected.
     */
    @AfterClass
    public static void tearDownAfterClass() throws Exception {
    }

    /**
     * Get a {@link CclExecutor} to be used to execute CCL commands.
     *
     * @return A {@link CclExecutor}.
     */
    protected CclExecutor getCclExecutor() {
        final CclExecutor executor = CclExecutor.getExecutor();
        if (doDebug != null && doDebug.booleanValue())
            executor.setOutputStream(System.out, OutputType.FULL_DEBUG);
        return new JaasCclExecutor(getSubject(), executor);
    }

    /**
     * Get an {@link FtpProduct} that can be used to assemble {@link Uploader uploaders} and {@link Downloader
     * downloaders}.
     *
     * @return An {@link FtpProduct} object.
     */
    protected final FtpProduct getFtpProduct() {
        return ftpProduct;
    }

    /**
     * Get a logical retriever.
     *
     * @return The logical retriever.
     */
    protected final LogicalRetriever getLogicalRetriever() {
        return logicalRetriever;
    }

    /**
     * Get an uploader used to upload resources to the remote server.
     *
     * @return A {@link CclResourceUploader}.
     */
    protected CclResourceUploader getResourceUploader() {
        return new JaasResourceUploader(getSubject(), CclResourceUploader.getUploader());
    }

    /**
     * Get the subject that can be used to make authenticated calls via an {@link #getCclExecutor() executor}.
     *
     * @return A {@link Subject}.
     */
    protected Subject getSubject() {
        return subject;
    }

    /**
     * Get the directory in which test resources for this test are stored.
     *
     * @return A {@link File} object representing the directory in which the test resources for this test are stored.
     */
    protected final File getTestResourceDirectory() {
        return testResourcesDirectory;
    }

    /**
     * Get an uploader that can be used to upload resources to the remote server.
     *
     * @return An {@link Uploader} used to upload files to the remote server.
     */
    protected Uploader getUploader() {
        final Uploader uploader = SftpUploader.createUploader(getFtpProduct());
        uploader.ignoreChmodErrors(true);
        return uploader;
    }
}