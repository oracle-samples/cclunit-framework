declare testWithAssertionSuccess(null) = null

subroutine testWithAssertionSuccess(null)
    set stat = cclutAsserti2Equal(CURREF, 'this should succeed', 1, 1)
end
