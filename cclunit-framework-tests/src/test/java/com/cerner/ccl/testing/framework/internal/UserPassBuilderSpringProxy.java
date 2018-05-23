package com.cerner.ccl.testing.framework.internal;

import java.net.URI;

import com.cerner.ftp.data.FtpProduct;
import com.cerner.ftp.data.sftp.UserPassBuilder;
import com.cerner.ftp.data.sftp.UserPassBuilder.UserPassProduct;

/**
 * A proxy to facilitate construction of a {@link FtpProduct}. This is needed because the return types of
 * {@link UserPassBuilder} do not play nicely with Spring's XML-driven configuration.
 * 
 * @author Joshua Hyde
 * 
 */

public class UserPassBuilderSpringProxy {
    private final UserPassBuilder builder;

    /**
     * Create a Spring-friendly proxy of a {@link UserPassBuilder}.
     * 
     * @param builder
     *            A {@link UserPassBuilder} object to be proxied.
     */
    public UserPassBuilderSpringProxy(UserPassBuilder builder) {
        this.builder = builder;
    }

    /**
     * Construct the user information into a data object.
     * 
     * @return A {@link UserPassProduct}.
     */
    public UserPassProduct build() {
        return builder.build();
    }

    /**
     * Set the password.
     * 
     * @param password
     *            The password.
     */
    public void setPassword(String password) {
        builder.setPassword(password);
    }

    /**
     * Set the server address.
     * 
     * @param serverAddress
     *            The server address.
     */
    public void setServerAddress(URI serverAddress) {
        builder.setServerAddress(serverAddress);
    }

    /**
     * Set the username.
     * 
     * @param username
     *            The username.
     */
    public void setUsername(String username) {
        builder.setUsername(username);
    }
}
