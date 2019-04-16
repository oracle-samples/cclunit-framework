%#ifndef CCLUTMOCK_INC
%#def CCLUTMOCK_INC
;**********************************************************************************************************************************
;
; Provides support functions that can be leveraged by consumers to construct mock objects and use those mocks in the script under
; test.
;
;**********************************************************************************************************************************

%i cclsource:cclut_utils.inc
%i cclsource:cclutmock_table.inc

declare public::cclutAddMockImplementation(cclutOriginalName = vc(val), cclutReplaceName = vc(val)) = null with protect
declare public::cclutRemoveMockImplementation(cclutOriginalName = vc(val)) = null with protect
declare public::cclutRemoveAllMockImplementations(null) = null with protect
declare public::cclutExecuteProgramWithMocks(cclutProgramName = vc(val), cclutParams = vc(val, " "),
    cclutNamespace = vc(val, "PUBLIC")) = null with protect
declare public::cclutRemoveAllMocks(null) = null with protect
declare public::cclutInvokeParser(cclutParserString = vc) = null with protect

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
subroutine public::cclutAddMockImplementation(cclutOriginalName, cclutReplaceName)
    declare CCLUT_ORIGINAL_NAME = vc with protect, constant(cnvtupper(cclutOriginalName))
    declare CCLUT_REPLACE_NAME = vc with protect, constant(cnvtupper(cclutReplaceName))
    declare cclutMockImplementationLength =
        i4 with protect, noconstant(size(cclut_mockImplementations->implementations, 5))
    declare cclutMockImplementationPos = i4 with protect, noconstant(0)
    declare cclutMockImplementationIndex = i4 with protect, noconstant(0)
    declare cclutStat = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (cclutIsEmpty(CCLUT_ORIGINAL_NAME))
        call cclut::addStashedError(392, "cclutAddMockImplementation() - originalName is a required field.", CURREF)
        return
    endif
    if (cclutIsEmpty(CCLUT_REPLACE_NAME))
        call cclut::addStashedError(392,
            concat("cclutAddMockImplementation(", CCLUT_ORIGINAL_NAME, ") - replaceName is a required field."), CURREF)
        return
    endif

    ;Validate that the implementation exists
    set cclutMockImplementationPos =
        locateval(cclutMockImplementationIndex, 1, cclutMockImplementationLength,
        CCLUT_ORIGINAL_NAME, cclut_mockImplementations->implementations[cclutMockImplementationIndex].originalName)
    if (cclutMockImplementationPos < 1)
        set cclutMockImplementationPos = cclutMockImplementationLength + 1
        set cclutStat = alterlist(cclut_mockImplementations->implementations, cclutMockImplementationPos)
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
subroutine public::cclutRemoveMockImplementation(cclutOriginalName)
    declare CCLUT_ORIGINAL_NAME = vc with protect, constant(cnvtupper(cclutOriginalName))
    declare cclutMockImplementationLength =
            i4 with protect, noconstant(size(cclut_mockImplementations->implementations, 5))
    declare cclutMockImplementationPos = i4 with protect, noconstant(0)
    declare cclutMockImplementationIndex = i4 with protect, noconstant(0)
    declare cclutStat = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (cclutIsEmpty(CCLUT_ORIGINAL_NAME))
        call cclut::addStashedError(392, "cclutRemoveMockImplementation() - originalName is a required field.", CURREF)
        return
    endif

    ;Validate that the implementation exists
    set cclutMockImplementationPos =
        locateval(cclutMockImplementationIndex, 1, cclutMockImplementationLength,
        CCLUT_ORIGINAL_NAME, cclut_mockImplementations->implementations[cclutMockImplementationIndex].originalName)
    if (cclutMockImplementationPos > 0)
        set cclutStat = alterlist(cclut_mockImplementations->implementations, cclutMockImplementationLength - 1,
            cclutMockImplementationPos - 1)
    endif
end ;cclutRemoveMockImplementation

/**
Removes all mock implementations.

Example:
call cclutRemoveAllMockImplementations(null)
**/
subroutine public::cclutRemoveAllMockImplementations(null)
    while (size(cclut_mockImplementations->implementations, 5) > 0)
        call cclutRemoveMockImplementation(cclut_mockImplementations->implementations[1].originalName)
    endwhile
end ;cclutRemoveAllMockImplementations

/**
Executes a CCL program applying an indicated namespace and all mocks that have been specified using the functions
described below.  programName is required.  params is a stringified representation of the parameters to be passed to the
program, so all commas and string delimiters must be specified.  If namespace is omitted, it will default to the PUBLIC
namespace.

@param programName
    The program to be executed with mocks.
@param params
    The parameters to be sent to the program.
@param namespace
    The namespace under which to execute the program.

Example:
call cclutExecuteProgramWithMocks("ccl_my_program")
call cclutExecuteProgramWithMocks("ccl_my_program", "^MINE^, 1.0, ^string parameter^")
call cclutExecuteProgramWithMocks("ccl_my_program", "", "MYNAMESPACE")
call cclutExecuteProgramWithMocks("ccl_my_program", "^MINE^, 1.0, ^string parameter^", "MYNAMESPACE")
**/
subroutine public::cclutExecuteProgramWithMocks(cclutProgramName, cclutParams, cclutNamespace)
    declare CCLUT_PROGRAM_NAME = vc with protect, constant(cnvtupper(cclutProgramName))
    declare CCLUT_PARAMS = vc with protect, constant(cclutParams)
    declare CCLUT_NAMESPACE = vc with protect, constant(cclutNamespace)
    declare cclutMockImplementationLength =
            i4 with protect, noconstant(size(cclut_mockImplementations->implementations, 5))
    declare cclutMockImplementationIndex = i4 with protect, noconstant(0)
    declare cclutMockTableLength = i4 with protect, noconstant(size(cclut_mockTables->tables, 5))
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutReplaceOption = i4 with protect, noconstant(0)
    declare cclutParserText = vc with protect, noconstant("")

    ;Validate inbound parameters
    if (cclutIsEmpty(CCLUT_PROGRAM_NAME))
        call cclut::addStashedError(392, "cclutExecuteProgramWithMocks() - programName is a required field.", CURREF)
        return
    endif

    ;Use replace option 6 for table names if it is available.
    if (cclutGetCurrentVersion(null) >= 90300 or
        (cclutGetCurrentRevision(null) = 8 and cclutGetCurrentVersion(null) >= 81500))
            set cclutReplaceOption = 6
    endif

    set cclutParserText = concat("execute ", CCLUT_PROGRAM_NAME)
    if (cclutIsEmpty(CCLUT_PARAMS) = FALSE)
        set cclutParserText = concat(cclutParserText, " ", CCLUT_PARAMS)
    endif
    set cclutParserText = concat(cclutParserText, " with")

    ;Add mock implementations
    for (cclutMockImplementationIndex = 1 to cclutMockImplementationLength)
        set cclutParserText = concat(cclutParserText, ^ replace("^,
            cclut_mockImplementations->implementations[cclutMockImplementationIndex].originalName, ^", ^,
            cclut_mockImplementations->implementations[cclutMockImplementationIndex].replaceName, ^),^)
    endfor

    ;Add mock tables
    for (cclutMockTableIndex = 1 to cclutMockTableLength)
        if (cclut_mockTables->tables[cclutMockTableIndex].isFinalized = TRUE)
            set cclutParserText =
                concat(cclutParserText, ^ replace("^, cclut_mockTables->tables[cclutMockTableIndex].originalName, ^", ^,
                cclut_mockTables->tables[cclutMockTableIndex].replaceName)
             if (cclut_mockTables->tables[cclutMockTableIndex].isAccessedViaRdbCommands = FALSE)
                set cclutParserText = concat(cclutParserText, ^, ^, trim(cnvtstring(cclutReplaceOption), 3))
             endif
            set cclutParserText = concat(cclutParserText, ^),^)
        endif
    endfor

    ;Add namespace
    set cclutParserText = concat(cclutParserText, ^ curnamespace = "^, CCLUT_NAMESPACE, ^" go^)
    call cclut::stashErrors(null)
    call cclutInvokeparser(cclutParserText)
end ;cclutExecuteProgramWithMocks

/**
Removes all mock implementations and mock tables that have been added through the mocking APIs.  This should be called
at the completion of a test case to clean up all mocks.

Example:
call cclutRemoveAllMocks(null)
**/
subroutine public::cclutRemoveAllMocks(null)
    call cclutRemoveAllMockImplementations(null)
    call cclutRemoveAllMockTables(null)
end ;cclutRemoveAllMocks


/**
Invokes call parser on a specified string. Allows for overriding specific calls to call parser.

Warning: Adding/Removing usages could affect unit tests which mock this function. It might be better to create alternate versions.

@param cclutParserString
    The string to invoke call parser on.
*/
subroutine public::cclutInvokeParser(cclutParserString)
    call parser(cclutParserString)
end ;cclutInvokeParser
%#endif CCLUTMOCK_INC
