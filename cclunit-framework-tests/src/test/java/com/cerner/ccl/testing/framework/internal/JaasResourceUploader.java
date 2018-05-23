package com.cerner.ccl.testing.framework.internal;

import java.io.File;
import java.net.URI;
import java.security.PrivilegedAction;
import java.util.Map;

import javax.security.auth.Subject;

import com.cerner.ccl.j4ccl.util.CclResourceUploader;

/**
 * A {@link CclResourceUploader} that runs a given delegate in the context of a given {@link Subject}.
 *
 * @author Joshua Hyde
 *
 */

public class JaasResourceUploader extends CclResourceUploader {
    private final CclResourceUploader delegate;
    private final Subject subject;

    /**
     * Create an uploader.
     *
     * @param subject
     *            The {@link Subject} to be used to execute the uploading.
     * @param delegate
     *            A {@link CclResourceUploader} to which the work of this uploader is to be delegated.
     */
    public JaasResourceUploader(final Subject subject, final CclResourceUploader delegate) {
        this.subject = subject;
        this.delegate = delegate;
    }

    @Override
    public void queueUpload(final File file) {
        delegate.queueUpload(file);
    }

    @Override
    public Map<File, URI> upload() {
        return Subject.doAs(subject, new PrivilegedAction<Map<File, URI>>() {
            @SuppressWarnings("synthetic-access")
            public Map<File, URI> run() {
                return delegate.upload();
            }
        });
    }
}
