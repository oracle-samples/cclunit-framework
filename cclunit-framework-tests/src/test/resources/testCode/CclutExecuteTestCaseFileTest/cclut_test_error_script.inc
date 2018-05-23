declare testExecuteScript(null) = null

subroutine testExecuteScript(null)
    execute cclutErrorScript
    
    ; Do a simple assert to populate the <ASSERT /> code
    set stat = cclutAssertf8Equal(CURREF, 'This should pass.', 0, 0)
    set stat = cclutAssertf8Equal(CURREF, 'This should fail.', 1, 0)
end
