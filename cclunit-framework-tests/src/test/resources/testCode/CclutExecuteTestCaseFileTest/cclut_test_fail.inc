declare testFail(null) = null

subroutine testFail(null)
    set stat = cclutAssertf8Equal(CURREF, 'This should fail.', 1.0, 0.0)
end
