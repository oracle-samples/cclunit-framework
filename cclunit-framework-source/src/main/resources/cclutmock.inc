%#ifndef CCLUTMOCK_INC
%#def CCLUTMOCK_INC
;**********************************************************************************************************************************
;
; Provides support functions that can be leveraged by consumers to construct mock objects and use those mocks in the script under
; test.
;
;**********************************************************************************************************************************

%i cclsource:cclutmock_table.inc

declare cclutAddMockImplementation(originalName = vc(val), replaceName = vc(val)) = null with protect
declare cclutRemoveMockImplementation(originalName = vc(val)) = null with protect
declare cclutRemoveAllMockImplementations(null) = null with protect
declare cclutExecuteProgramWithMocks(programName = vc(val), params = vc(val),
    namespace = vc(val, "PUBLIC")) = null with protect
declare cclutRemoveAllMocks(null) = null with protect

record cclut_mockImplementations (
    1 implementations[*]
		2 originalName = vc
		2 replaceName = vc
) with protect

/**
Adds a mock implementation to be utilized by cclutExecuteProgramWithMocks.  This can include record structures,
subroutines, or other programs.  originalName and replaceName are required.  If originalName is already being mocked,
the mock will be overwritten with the new replaceName.

@param originalName
    The object to be mocked.
@param replaceName
    The mocked object.

Example:
call cclutAddMockImplementation("uar_get_code_by", "mock_uar_get_code_by")
**/
subroutine cclutAddMockImplementation(originalName, replaceName)
    ;Declare local variables
    declare CCLUT_ORIGINAL_NAME = vc with protect, constant(trim(cnvtupper(originalName), 3))
    declare CCLUT_REPLACE_NAME = vc with protect, constant(trim(cnvtupper(replaceName), 3))
    declare cclutMockImplementationLength =
        i4 with protect, noconstant(size(cclut_mockImplementations->implementations, 5))
    declare cclutMockImplementationPos = i4 with protect, noconstant(0)
    declare cclutMockImplementationIndex = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (CCLUT_ORIGINAL_NAME <= " ")
        call cclexception(100, "E", "cclutAddMockImplementation() - originalName is a required field")
        return
    endif
    if (CCLUT_REPLACE_NAME <= " ")
        call cclexception(100, "E", "cclutAddMockImplementation() - replaceName is a required field")
        return
    endif

    ;Validate that the implementation exists
    set cclutMockImplementationPos =
        locateval(cclutMockImplementationIndex, 1, cclutMockImplementationLength,
        CCLUT_ORIGINAL_NAME, cclut_mockImplementations->implementations[cclutMockImplementationIndex].originalName)
    if (cclutMockImplementationPos < 1)
        set cclutMockImplementationPos = cclutMockImplementationLength + 1
        set stat = alterlist(cclut_mockImplementations->implementations, cclutMockImplementationPos)
        set cclut_mockImplementations->implementations[cclutMockImplementationPos].originalName = CCLUT_ORIGINAL_NAME
    endif

    set cclut_mockImplementations->implementations[cclutMockImplementationPos].replaceName = CCLUT_REPLACE_NAME
end ;cclutAddMockImplementation

/**
Removes a mock implementation.

@param originalName
    The object that is mocked.

Example:
call cclutRemoveMockImplementation("uar_get_code_by")
**/
subroutine cclutRemoveMockImplementation(originalName)
    ;Declare local variables
    declare CCLUT_ORIGINAL_NAME = vc with protect, constant(trim(cnvtupper(originalName), 3))
    declare cclutMockImplementationLength =
            i4 with protect, noconstant(size(cclut_mockImplementations->implementations, 5))
    declare cclutMockImplementationPos = i4 with protect, noconstant(0)
    declare cclutMockImplementationIndex = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (CCLUT_ORIGINAL_NAME <= " ")
        call cclexception(100, "E", "cclutRemoveMockImplementation() - originalName is a required field")
        return
    endif

    ;Validate that the implementation exists
    set cclutMockImplementationPos =
        locateval(cclutMockImplementationIndex, 1, cclutMockImplementationLength,
        CCLUT_ORIGINAL_NAME, cclut_mockImplementations->implementations[cclutMockImplementationIndex].originalName)
    if (cclutMockImplementationPos > 0)
        set stat = alterlist(cclut_mockImplementations->implementations, cclutMockImplementationLength - 1,
            cclutMockImplementationPos - 1)
    endif
end ;cclutRemoveMockImplementation

/**
Removes all mock implementations.

Example:
call cclutRemoveAllMockImplementations(null)
**/
subroutine cclutRemoveAllMockImplementations(null)
    while (size(cclut_mockImplementations->implementations, 5) > 0)
        call cclutRemoveMockImplementation(cclut_mockImplementations->implementations[1].originalName)
    endwhile
end ;cclutRemoveAllMockImplementations

/**
Executes a program with all mocks currently added through cclutAddMockImplementation() and cclutCreateMockTable().
programName is required.  params is a string parameter to be sent directly to the program, so all commas and string
delimiters must be specified.  If namespace is omitted, it will default to the PUBLIC namespace.

Example:
call cclutExecuteProgramWithMocks("ccl_my_program", "^MINE^, 1.0, ^string parameter^", "MYNAMESPACE")
**/
subroutine cclutExecuteProgramWithMocks(programName, params, namespace)
    ;Declare local variables
    declare CCLUT_PROGRAM_NAME = vc with protect, constant(trim(cnvtupper(programName), 3))
    declare CCLUT_PARAMS = vc with protect, constant(params)
    declare CCLUT_NAMESPACE = vc with protect, constant(namespace)
    declare cclutMockImplementationLength =
            i4 with protect, noconstant(size(cclut_mockImplementations->implementations, 5))
    declare cclutMockImplementationIndex = i4 with protect, noconstant(0)
    declare cclutMockTableLength = i4 with protect, noconstant(size(cclut_mockTables->tables, 5))
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutHasComma = i4 with protect, noconstant(0)
    declare cclutParserText = vc with protect, noconstant("")

    ;Validate inbound parameters
    if (CCLUT_PROGRAM_NAME <= " ")
        call cclexception(100, "E", "cclutExecuteProgramWithMocks() - programName is a required field")
        return
    endif

    set cclutParserText = concat(" execute ", CCLUT_PROGRAM_NAME, " ")
    if (params > " ")
        set cclutParserText = concat(cclutParserText, " ", params)
    endif
    set cclutParserText = concat(cclutParserText, " with ")

    ;Add mock implementations
    for (cclutMockImplementationIndex = 1 to cclutMockImplementationLength)
        if (cclutHasComma)
            set cclutParserText = concat(cclutParserText, ", ")
        endif
        set cclutParserText = concat(cclutParserText, ^ replace("^,
            cclut_mockImplementations->implementations[cclutMockImplementationIndex].originalName, ^", ^,
            cclut_mockImplementations->implementations[cclutMockImplementationIndex].replaceName, ^) ^)
        set cclutHasComma = 1
    endfor

    ;Add mock tables
    for (cclutMockTableIndex = 1 to cclutMockTableLength)
        if (cclut_mockTables->tables[cclutMockTableIndex].isFinalized = TRUE)
            if (cclutHasComma)
                set cclutParserText = concat(cclutParserText, ", ")
            endif
            set cclutParserText =
                concat(cclutParserText, ^ replace("^, cclut_mockTables->tables[cclutMockTableIndex].originalName, ^", ^,
                cclut_mockTables->tables[cclutMockTableIndex].replaceName, ^) ^)
            set cclutHasComma = 1
        endif
    endfor

    ;Add namespace
    if (cclutHasComma)
        set cclutParserText = concat(cclutParserText, ", ")
    endif
    set cclutParserText = concat(cclutParserText, ^ curnamespace = "^, CCLUT_NAMESPACE, ^" go ^)
    call parser(cclutParserText)
end ;cclutExecuteProgramWithMocks

/**
Removes all mock implementations and mock tables that have been added through the cclutAddMockImplementation() and
cclutCreateMockTable() APIs.  This should be called at the completion of a test suite to clean up all mocks.

Example:
call cclutRemoveAllMocks(null)
**/
subroutine cclutRemoveAllMocks(null)
    call cclutRemoveAllMockImplementations(null)
    call cclutRemoveAllMockTables(null)
end ;cclutRemoveAllMocks

%#endif CCLUTMOCK_INC