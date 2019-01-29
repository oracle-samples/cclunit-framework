declare setup(dummyVar = i2) = null
declare tearDown(dummyVar = i2) = null

subroutine tearDown(dummyVar)
    call cclutRemoveAllMocks(null)
    rollback
end ;tearDown

;**********************************************************************************************************************************
;** cclutAddMockImplementation
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

/* test_cclutAddMockImplementation_different_cases **********************************************************
*  Scenario: Adds mock implementations with different cases                                                 *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_different_cases(null)
    call cclutAddMockImplementation("sAmpLE_subROUtine", "MoCK_suBROutiNE")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_different_cases 001",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("sEcOnD_sUbRoUtInE", "NeW_sUbRoUtInE")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_different_cases 004",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 005",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 006",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 007",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 008",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")
end ;test_cclutAddMockImplementation_different_cases

/* test_cclutAddMockImplementation_existing_implementation **************************************************
*  Scenario: Replaces the mock name of an implementation if it was already added                            *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_existing_implementation(null)
	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_existing_implementation 001",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("second_subroutine", "new_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_existing_implementation 004",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 005",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 006",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 007",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 008",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")

    call cclutAddMockImplementation("sample_subroutine", "replace_subroutine")
    call cclutAddMockImplementation("second_subroutine", "test_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_existing_implementation 009",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 010",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 011",
        cclut_mockImplementations->implementations[1].replaceName, "REPLACE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 012",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 013",
        cclut_mockImplementations->implementations[2].replaceName, "TEST_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy

/* test_cclutAddMockImplementation_missing_originalName *****************************************************
*  Scenario: Throws an error when an empty originalName is supplied to the function (after trimming)        *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_missing_originalName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockImplementation("     ", "mock_subroutine")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockImplementation_missing_originalName 001",
        trim(errorMessage, 3), "regexplike",
        concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockImplementation\(\) - originalName is a required field."))
end ;test_cclutAddMockImplementation_missing_originalName

/* test_cclutAddMockImplementation_missing_replaceName ******************************************************
*  Scenario: Throws an error when an empty replaceName is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_missing_replaceName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockImplementation("sample_subroutine", "     ")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockImplementation_missing_replaceName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockImplementation\(SAMPLE_SUBROUTINE\) - replaceName is a required field."))
end ;test_cclutAddMockImplementation_missing_replaceName

;**********************************************************************************************************************************
;** cclutRemoveMockImplementation
;**********************************************************************************************************************************
/* test_cclutRemoveMockImplementation_happy *****************************************************************
*  Scenario: Removes a mock implementation                                                                  *
************************************************************************************************************/
subroutine test_cclutRemoveMockImplementation_happy(null)
    call cclutAddMockImplementation("one_subroutine", "mock_one_subroutine")
    call cclutAddMockImplementation("two_subroutine", "mock_two_subroutine")
    call cclutAddMockImplementation("three_subroutine", "mock_three_subroutine")
    call cclutAddMockImplementation("four_subroutine", "mock_four_subroutine")
    call cclutAddMockImplementation("five_subroutine", "mock_five_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockImplementation_happy 001",
        size(cclut_mockImplementations->implementations, 5), 5)
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 002",
        cclut_mockImplementations->implementations[1].originalName, "ONE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_ONE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 004",
        cclut_mockImplementations->implementations[2].originalName, "TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 005",
        cclut_mockImplementations->implementations[2].replaceName, "MOCK_TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 006",
        cclut_mockImplementations->implementations[3].originalName, "THREE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 007",
        cclut_mockImplementations->implementations[3].replaceName, "MOCK_THREE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 008",
        cclut_mockImplementations->implementations[4].originalName, "FOUR_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 009",
        cclut_mockImplementations->implementations[4].replaceName, "MOCK_FOUR_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 010",
        cclut_mockImplementations->implementations[5].originalName, "FIVE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 011",
        cclut_mockImplementations->implementations[5].replaceName, "MOCK_FIVE_SUBROUTINE")

    call cclutRemoveMockImplementation("one_subroutine")
    call cclutRemoveMockImplementation("three_subroutine")
    call cclutRemoveMockImplementation("five_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockImplementation_happy 012",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 013",
        cclut_mockImplementations->implementations[1].originalName, "TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 014",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 015",
        cclut_mockImplementations->implementations[2].originalName, "FOUR_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 016",
        cclut_mockImplementations->implementations[2].replaceName, "MOCK_FOUR_SUBROUTINE")
end ;test_cclutRemoveMockImplementation_happy

/* test_cclutRemoveMockImplementation_missing_originalName **************************************************
*  Scenario: Throws an error when an empty originalName is supplied to the function (after trimming)        *
************************************************************************************************************/
subroutine test_cclutRemoveMockImplementation_missing_originalName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutRemoveMockImplementation("     ")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutRemoveMockImplementation_missing_originalName 001",
        trim(errorMessage, 3), "regexplike",
        concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutRemoveMockImplementation\(\) - originalName is a required field."))
end ;test_cclutRemoveMockImplementation_missing_originalName

;**********************************************************************************************************************************
;** cclutRemoveAllMockImplementations
;**********************************************************************************************************************************
/* test_cclutRemoveAllMockImplementations_happy *************************************************************
*  Scenario: Removes all mock implementations                                                               *
************************************************************************************************************/
subroutine test_cclutRemoveAllMockImplementations_happy(null)
    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")
    call cclutAddMockImplementation("sample_subroutine_2", "mock_subroutine_2")
    call cclutAddMockImplementation("sample_subroutine_3", "mock_subroutine_3")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockImplementations_happy 001",
        size(cclut_mockImplementations->implementations, 5), 3)

    call cclutRemoveAllMockImplementations(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockImplementations_happy 002",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveAllMockImplementations_happy

;**********************************************************************************************************************************
;** cclutExecuteProgramWithMocks
;**********************************************************************************************************************************
/* test_cclutExecuteProgramWithMocks_happy ******************************************************************
*  Scenario: Executes a program using mock tables and implementations that have been set up                 *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_happy(null)
    declare public_subroutine = i4 with protect, noconstant(0)
    declare mock_subroutine = i4 with protect, noconstant(0)
    declare test_subroutine = i4 with protect, noconstant(0)

    record executeReply (
        1 number_parameter = i4
        1 string_parameter = vc
        1 regular_join[*]
            2 person_id = f8
            2 encounter_id = f8
            2 encounter_alias = vc
        1 left_join[*]
            2 person_id = f8
            2 encounter_id = f8
            2 encounter_alias = vc
        1 rdb_join[*]
            2 columns = vc
            2 data = vc
    ) with protect

    call cclutDefineMockTable("sample_person", "person_id", "f8")
    call cclutDefineMockTable("sample_encounter", "encounter_id|person_id", "f8|f8")
    call cclutDefineMockTable("sample_encounter_alias", "encounter_id|encounter_alias", "f8|vc")
    call cclutCreateMockTable("sample_person")
    call cclutCreateMockTable("sample_encounter")
    call cclutCreateMockTable("sample_encounter_alias")

    call cclutAddMockData("sample_person", "1.0")
    call cclutAddMockData("sample_person", "2.0")
    call cclutAddMockData("sample_person", "3.0")
    call cclutAddMockData("sample_encounter", "4.0|1.0")
    call cclutAddMockData("sample_encounter", "5.0|2.0")
    call cclutAddMockData("sample_encounter_alias", "4.0|test alias")

    call cclutAddMockImplementation("REPLY", "EXECUTEREPLY")

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", "10,^test param^")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 001", public_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 002", test_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 003", mock_subroutine, 0)

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 004", executeReply->number_parameter, 10)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 005", executeReply->string_parameter,
        "test param")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 006", size(executeReply->regular_join, 5),
        1)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 007", executeReply->regular_join[1].person_id,
        1.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 008",
        executeReply->regular_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 009",
        executeReply->regular_join[1].encounter_alias, "test alias")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 010", size(executeReply->left_join, 5), 3)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 011", executeReply->left_join[1].person_id,
        1.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 012",
        executeReply->left_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 013",
        executeReply->left_join[1].encounter_alias, "test alias")
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 014", executeReply->left_join[2].person_id,
        2.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 015",
        executeReply->left_join[2].encounter_id, 5.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 016",
        executeReply->left_join[2].encounter_alias, "")
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 017", executeReply->left_join[3].person_id,
        3.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 018",
        executeReply->left_join[3].encounter_id, 0.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 019",
        executeReply->left_join[3].encounter_alias, "")

    ;The program ut_cclut_execute_mocks_happy performs an inner join for the rdb test and writes the data to the
    ;cclut_happy.dat file.
    set stat = alterlist(executeReply->rdb_join, 1)
    free define rtl2
    define rtl2 is "ccluserdir:cclut_happy.dat"
    select into "nl:"
    from rtl2t r
    head report
        executeReply->rdb_join[1].columns = r.line
    foot report
        executeReply->rdb_join[1].data = r.line
    with nocounter

    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_happy 020", executeReply->rdb_join[1].columns,
        "regexplike", "PERSON_ID.*ENCOUNTER_ID.*PERSON_ID.*ENCOUNTER_ID.*ENCOUNTER_ALIAS")
    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_happy 021", executeReply->rdb_join[1].data,
        "regexplike", "1.*4.*1.*4.*test alias")
end ;test_cclutExecuteProgramWithMocks_happy

/* test_cclutExecuteProgramWithMocks_namespace **************************************************************
*  Scenario: Executes the program using the supplied namespace                                              *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_namespace(null)
    declare public_subroutine = i4 with protect, noconstant(0)
    declare mock_subroutine = i4 with protect, noconstant(0)
    declare test_subroutine = i4 with protect, noconstant(0)

    declare cclut::internalSubroutine(null) = null with protect

    subroutine (cclut::internalSubroutine(null) = null with protect)
        set test_subroutine = 1
    end

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", "10,^test param^", "CCLUT")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace 001", public_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace 002", test_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace 003", mock_subroutine, 0)
end ;test_cclutExecuteProgramWithMocks_namespace

/* test_cclutExecuteProgramWithMocks_mock_subroutine ********************************************************
*  Scenario: Executes the program while replacing a subroutine with a mocked version                        *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_mock_subroutine(null)
    declare public_subroutine = i4 with protect, noconstant(0)
    declare mock_subroutine = i4 with protect, noconstant(0)
    declare test_subroutine = i4 with protect, noconstant(0)

    declare cclut::mockSubroutine(null) = null with protect

    subroutine (cclut::mockSubroutine(null) = null with protect)
        set mock_subroutine = 1
    end

    call cclutAddMockImplementation("internalSubroutine", "mockSubroutine")

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", "10,^test param^", "CCLUT")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 001", public_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 002", test_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 003", mock_subroutine, 1)
end ;test_cclutExecuteProgramWithMocks_mock_subroutine

/* test_cclutExecuteProgramWithMocks_missing_programName ****************************************************
*  Scenario: Throws an error when an empty programName is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_missing_programName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutExecuteProgramWithMocks("     ", "")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_missing_programName 001",
        trim(errorMessage, 3), "regexplike",
        concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutExecuteProgramWithMocks\(\) - programName is a required field."))
end ;test_cclutExecuteProgramWithMocks_missing_programName

;**********************************************************************************************************************************
;** cclutRemoveAllMocks
;**********************************************************************************************************************************
/* test_cclutRemoveAllMocks_happy ***************************************************************************
*  Scenario: Removes all mock tables and implementations so that the originals can be used                  *
************************************************************************************************************/
subroutine test_cclutRemoveAllMocks_happy(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_2", "sample_table_2_id|sample_table_2_text|sample_table_2_date",
        "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_3", "sample_table_3_id|sample_table_3_text|sample_table_3_date",
        "f8|vc|dq8")

    call cclutCreateMockTable("sample_table")

    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")
    call cclutAddMockImplementation("sample_subroutine_2", "mock_subroutine_2")
    call cclutAddMockImplementation("sample_subroutine_3", "mock_subroutine_3")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 001",
        size(cclut_mockTables->tables, 5), 3)
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 002",
        size(cclut_mockImplementations->implementations, 5), 3)

    call cclutRemoveAllMocks(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 003", size(cclut_mockTables->tables, 5), 0)
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 004",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveAllMocks_happy
