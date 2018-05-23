declare testWithRuntimeError(null) = null

subroutine testWithRuntimeError(null)
    declare someVar = i4 with protect, noconstant(0)
    set someVar = subroutineDoesNotExist(2)
end
