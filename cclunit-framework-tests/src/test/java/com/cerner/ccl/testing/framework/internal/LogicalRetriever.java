package com.cerner.ccl.testing.framework.internal;

import java.io.File;
import java.security.PrivilegedAction;
import java.util.HashMap;
import java.util.Map;

import javax.security.auth.Subject;

import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.record.Record;
import com.cerner.ccl.j4ccl.record.StructureBuilder;
import com.cerner.ccl.j4ccl.record.factory.RecordFactory;

/**
 * A utility class used to retrieve logical values from the backend.
 * 
 * @author Joshua Hyde
 * 
 */

@Component
public class LogicalRetriever implements DisposableBean, InitializingBean {
    private final File scriptLocation = new File("src/test/resources/testCode/utilities/cclutGetLogicalUtility.prg");
    private final String scriptName = "cclutGetLogicalUtility";
    private final Map<String, String> logicalCache = new HashMap<String, String>();
    private final Subject subject;

    /**
     * Create a logical retriever.
     * 
     * @param subject
     *            The {@link Subject} under which the retriever should operate.
     */
    @Autowired
    public LogicalRetriever(final Subject subject) {
        if (subject == null)
            throw new IllegalArgumentException("Subject cannot be null.");

        this.subject = subject;
    }

    /**
     * {@inheritDoc}
     */
    public void destroy() throws Exception {
        final CclExecutor executor = CclExecutor.getExecutor();
        executor.addScriptDropper(scriptName).commit();

        Subject.doAs(subject, new PrivilegedAction<Void>() {
            public Void run() {
                executor.execute();
                return null;
            }
        });
    }

    /**
     * {@inheritDoc}
     */
    public void afterPropertiesSet() throws Exception {
        final CclExecutor executor = CclExecutor.getExecutor();
        executor.addScriptCompiler(scriptLocation).commit();

        Subject.doAs(subject, new PrivilegedAction<Void>() {
            public Void run() {
                executor.execute();
                return null;
            }
        });
    }

    /**
     * Get the logical's literal value.
     * 
     * @param logicalName
     *            The name of the logical whose value is to be retrieved.
     * @return The value of the logical.
     */
    public String getLogicalValue(final String logicalName) {
        if (logicalCache.containsKey(logicalName))
            return logicalCache.get(logicalName);

        final Record request = RecordFactory.create("request",
                StructureBuilder.getBuilder().addVC("logical_name").build());
        request.setVC("logical_name", logicalName);

        final Record reply = RecordFactory.create("reply",
                StructureBuilder.getBuilder().addVC("logical_value").build());
        final CclExecutor executor = CclExecutor.getExecutor();
        executor.addScriptExecution(scriptName).withReplace("request", request).withReplace("reply", reply).commit();

        Subject.doAs(subject, new PrivilegedAction<Void>() {
            public Void run() {
                executor.execute();
                return null;
            }
        });

        return reply.getVC("logical_value");
    }
}
