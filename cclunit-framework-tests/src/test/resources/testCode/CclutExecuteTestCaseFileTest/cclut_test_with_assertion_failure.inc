declare testWithAssertionFailure(null) = null

subroutine testWithAssertionFailure(null)
    set stat = cclutAsserti2Equal(CURREF, 'this should fail', 1, 0)
end
