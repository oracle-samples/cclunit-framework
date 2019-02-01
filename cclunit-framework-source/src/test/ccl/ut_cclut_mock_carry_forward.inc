;**********************************************************************************************************************************
;** This is a special test case that is specifically designed to test that mocks carry forward from test to test until they are
;** explicitly removed.  The framework removes all mocks at the end of every test case.
;**********************************************************************************************************************************
/* test_cclutAddMockImplementation_happy ********************************************************************
*  Scenario: Adds mock implementations that can be used with cclutExecuteProgramWithMocks                   *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_happy(null)
    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy 001",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("second_subroutine", "new_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy 004",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 005",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 006",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 007",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 008",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy

/* test_cclutAddMockImplementation_not_cleared **************************************************************
*  Scenario: Demonstrates that the mocks from the previous test still exist since they have not been cleared*
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_not_cleared(null)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_not_cleared 001",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 004",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_not_cleared 005",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy