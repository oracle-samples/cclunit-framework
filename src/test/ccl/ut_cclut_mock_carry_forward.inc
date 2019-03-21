;**********************************************************************************************************************************
;** This is a special test case that is specifically designed to test that mocks carry forward from test to test until they are
;** explicitly removed.  The framework removes all mocks at the end of every test case.
;**********************************************************************************************************************************

declare setupOnce(null) = null
declare tearDownOnce(null) = null

subroutine setupOnce(null)
    call cclutAddMockImplementation("setup_once_subroutine", "mock_setup_once_subroutine")
end ;setupOnce

subroutine tearDownOnce(null)
    declare mockIndex = i4 with protect, noconstant(0)
    declare mockPos = i4 with protect, noconstant(0)

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_not_cleared 001",
        size(cclut_mockImplementations->implementations, 5), 4)

    set mockPos = locateVal(mockIndex, 1, 4, "SETUP_ONCE_SUBROUTINE",
        cclut_mockImplementations->implementations[mockIndex].originalName)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 002",
        cclut_mockImplementations->implementations[mockPos].replaceName, "MOCK_SETUP_ONCE_SUBROUTINE")

    set mockPos = locateVal(mockIndex, 1, 4, "ONE_SUBROUTINE",
        cclut_mockImplementations->implementations[mockIndex].originalName)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 003",
        cclut_mockImplementations->implementations[mockPos].replaceName, "MOCK_ONE_SUBROUTINE")

    set mockPos = locateVal(mockIndex, 1, 4, "TWO_SUBROUTINE",
        cclut_mockImplementations->implementations[mockIndex].originalName)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 004",
        cclut_mockImplementations->implementations[mockPos].replaceName, "MOCK_TWO_SUBROUTINE")

    set mockPos = locateVal(mockIndex, 1, 4, "THREE_SUBROUTINE",
        cclut_mockImplementations->implementations[mockIndex].originalName)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 005",
        cclut_mockImplementations->implementations[mockPos].replaceName, "MOCK_THREE_SUBROUTINE")
end ;tearDownOnce

/* test_cclutAddMockImplementation_one **********************************************************************
*  Scenario: Adds a mock implementation that can be used with cclutExecuteProgramWithMocks                  *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_one(null)
    call cclutAddMockImplementation("one_subroutine", "mock_one_subroutine")
end ;test_cclutAddMockImplementation_one

/* test_cclutAddMockImplementation_two **********************************************************************
*  Scenario: Adds a mock implementation that can be used with cclutExecuteProgramWithMocks                  *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_two(null)
    call cclutAddMockImplementation("two_subroutine", "mock_two_subroutine")
end ;test_cclutAddMockImplementation_two

/* test_cclutAddMockImplementation_three ********************************************************************
*  Scenario: Adds a mock implementation that can be used with cclutExecuteProgramWithMocks                  *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_three(null)
    call cclutAddMockImplementation("three_subroutine", "mock_three_subroutine")
end ;test_cclutAddMockImplementation_three