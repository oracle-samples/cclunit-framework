package com.cerner.ccl.testing.framework.internal;

import java.io.File;
import java.io.OutputStream;
import java.security.PrivilegedAction;

import javax.security.auth.Subject;

import com.cerner.ccl.j4ccl.CclExecutor;
import com.cerner.ccl.j4ccl.TerminalProperties;
import com.cerner.ccl.j4ccl.adders.DynamicCompilerAdder;
import com.cerner.ccl.j4ccl.adders.ScriptCompilerAdder;
import com.cerner.ccl.j4ccl.adders.ScriptDropAdder;
import com.cerner.ccl.j4ccl.adders.ScriptExecutionAdder;
import com.cerner.ccl.j4ccl.enums.OutputType;

/**
 * A {@link CclExecutor} that delegates all of its work to a given executor, performing the {@link #execute() execution}
 * of the queued commands in the context of a given {@link Subject}.
 *
 * @author Joshua Hyde
 *
 */

public class JaasCclExecutor extends CclExecutor {
    private final CclExecutor delegate;
    private final Subject subject;

    /**
     * Create an executor.
     *
     * @param subject
     *            The {@link Subject} to be used in the execution.
     * @param delegate
     *            The {@link CclExecutor} to which this executor delegates its tasks.
     */
    public JaasCclExecutor(final Subject subject, final CclExecutor delegate) {
        this.subject = subject;
        this.delegate = delegate;
    }

    @Override
    public DynamicCompilerAdder<?> addDynamicCompiler(final File includeFile) {
        return delegate.addDynamicCompiler(includeFile);
    }

    @Override
    public ScriptCompilerAdder<?> addScriptCompiler(final File file) {
        return delegate.addScriptCompiler(file);
    }

    @Override
    public ScriptDropAdder addScriptDropper(final String scriptName) {
        return delegate.addScriptDropper(scriptName);
    }

    @Override
    public ScriptExecutionAdder addScriptExecution(final String scriptName) {
        return delegate.addScriptExecution(scriptName);
    }

    @Override
    public void execute() {
        Subject.doAs(subject, new PrivilegedAction<Void>() {
            @SuppressWarnings("synthetic-access")
            public Void run() {
                delegate.execute();
                return null;
            }
        });
    }

    @Override
    public void setOutputStream(final OutputStream stream, final OutputType outputType) {
        delegate.setOutputStream(stream, outputType);
    }

    @Override
    public void setTerminalProperties(TerminalProperties terminalProperties) {
        delegate.setTerminalProperties(terminalProperties);
    }
}
