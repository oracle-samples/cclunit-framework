declare testSucceed(null) = null

subroutine testSucceed(null)
    set stat = cclutAssertf8Equal(CURREF, 'This should pass.', 0.0, 0.0)
end
