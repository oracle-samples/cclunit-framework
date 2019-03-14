declare tearDown(null) = null

subroutine tearDown(null)
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
            " cclutAddMockImplementation\(\) - originalName is a required field.")) ;^
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
            " cclutAddMockImplementation\(SAMPLE_SUBROUTINE\) - replaceName is a required field.")) ;^
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
        concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):", ;^
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
/* test_cclutExecuteProgramWithMocks_happy *******************************************************************************
*  Scenario: Demonstrates a program is successfully executed using mock tables and implementations that have been set up *
*************************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_happy(null)
    declare public_subroutine = i4 with protect, noconstant(0)
    declare mock_subroutine = i4 with protect, noconstant(0)
    declare test_subroutine = i4 with protect, noconstant(0)
    declare stat = i4 with protect, noconstant(0)
    declare sample_person_rdb_mock_table_name = vc with protect, noconstant("")
    declare sample_encounter_rdb_mock_table_name = vc with protect, noconstant("")
    declare sample_encounter_alias_rdb_mock_table_name = vc with protect, noconstant("")

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

    set sample_person_rdb_mock_table_name = cclutDefineMockTable("sample_person_rdb", "person_id", "f8")
    set sample_encounter_rdb_mock_table_name =
        cclutDefineMockTable("sample_encounter_rdb", "encounter_id|person_id", "f8|f8")
    set sample_encounter_alias_rdb_mock_table_name =
        cclutDefineMockTable("sample_encounter_alias_rdb", "encounter_id|encounter_alias", "f8|vc")
    call cclutCreateMockTable("sample_person_rdb")
    call cclutCreateMockTable("sample_encounter_rdb")
    call cclutCreateMockTable("sample_encounter_alias_rdb")

    call cclutAddMockData("sample_person", "1.0")
    call cclutAddMockData("sample_person", "2.0")
    call cclutAddMockData("sample_person", "3.0")
    call cclutAddMockData("sample_encounter", "4.0|1.0")
    call cclutAddMockData("sample_encounter", "5.0|2.0")
    call cclutAddMockData("sample_encounter_alias", "4.0|test alias")
    call cclutAddMockData("sample_person_rdb", "6.0")
    call cclutAddMockData("sample_person_rdb", "7.0")
    call cclutAddMockData("sample_person_rdb", "8.0")
    call cclutAddMockData("sample_encounter_rdb", "9.0|6.0")
    call cclutAddMockData("sample_encounter_rdb", "10.0|7.0")
    call cclutAddMockData("sample_encounter_alias_rdb", "9.0|test alias rdb")

    call cclutAddMockImplementation("reply", "executereply")
    call cclutAddMockImplementation("sample_person_rdb", sample_person_rdb_mock_table_name)
    call cclutAddMockImplementation("sample_encounter_rdb", sample_encounter_rdb_mock_table_name)
    call cclutAddMockImplementation("sample_encounter_alias_rdb", sample_encounter_alias_rdb_mock_table_name)

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", "10,^test param^")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 001", public_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 002", test_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 003", mock_subroutine, 0)

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 004", public::persistRecord->item, 1)

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 005", executeReply->number_parameter, 10)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 006", executeReply->string_parameter,
        "test param")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 007", size(executeReply->regular_join, 5),
        1)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 008", executeReply->regular_join[1].person_id,
        1.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 009",
        executeReply->regular_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 010",
        executeReply->regular_join[1].encounter_alias, "test alias")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 011", size(executeReply->left_join, 5), 3)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 012", executeReply->left_join[1].person_id,
        1.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 013",
        executeReply->left_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 014",
        executeReply->left_join[1].encounter_alias, "test alias")
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 015", executeReply->left_join[2].person_id,
        2.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 016",
        executeReply->left_join[2].encounter_id, 5.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 017",
        executeReply->left_join[2].encounter_alias, "")
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 018", executeReply->left_join[3].person_id,
        3.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy 019",
        executeReply->left_join[3].encounter_id, 0.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy 020",
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

    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_happy 021", executeReply->rdb_join[1].columns,
        "regexplike", "PERSON_ID.*ENCOUNTER_ID.*PERSON_ID.*ENCOUNTER_ID.*ENCOUNTER_ALIAS")
    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_happy 022", executeReply->rdb_join[1].data,
        "regexplike", "6.*9.*6.*9.*test alias rdb")
end ;test_cclutExecuteProgramWithMocks_happy


/* test_cclutExecuteProgramWithMocks_no_parameters **********************************************************
*  Scenario: Executes the program with no parameters and no namespace                                       *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_no_parameters(null)
    declare public_subroutine = i4 with protect, noconstant(0)
    declare sample_person_rdb_mock_table_name = vc with protect, noconstant("")
    declare sample_encounter_rdb_mock_table_name = vc with protect, noconstant("")
    declare sample_encounter_alias_rdb_mock_table_name = vc with protect, noconstant("")

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

    set sample_person_rdb_mock_table_name = cclutDefineMockTable("sample_person_rdb", "person_id", "f8")
    set sample_encounter_rdb_mock_table_name =
        cclutDefineMockTable("sample_encounter_rdb", "encounter_id|person_id", "f8|f8")
    set sample_encounter_alias_rdb_mock_table_name =
        cclutDefineMockTable("sample_encounter_alias_rdb", "encounter_id|encounter_alias", "f8|vc")
    call cclutCreateMockTable("sample_person_rdb")
    call cclutCreateMockTable("sample_encounter_rdb")
    call cclutCreateMockTable("sample_encounter_alias_rdb")

    call cclutAddMockImplementation("reply", "executereply")
    call cclutAddMockImplementation("sample_person_rdb", sample_person_rdb_mock_table_name)
    call cclutAddMockImplementation("sample_encounter_rdb", sample_encounter_rdb_mock_table_name)
    call cclutAddMockImplementation("sample_encounter_alias_rdb", sample_encounter_alias_rdb_mock_table_name)

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_no_parameters 001", public_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_no_parameters 002",
        executeReply->number_parameter, 0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_no_parameters 003",
        executeReply->string_parameter, "")
end ;test_cclutExecuteProgramWithMocks_no_parameters

/* test_cclutExecuteProgramWithMocks_namespace **************************************************************
*  Scenario: Executes the program using the supplied namespace                                              *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_namespace(null)
    declare test_subroutine = i4 with protect, noconstant(0)
    declare cclut::internalVariable = i4 with protect, noconstant(0)
    record cclut::internalRecord (
        1 item = i4
    ) with protect

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", "", "cclut")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace 001", test_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace 002", cclut::internalVariable, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace 003", cclut::internalRecord->item, 1)
end ;test_cclutExecuteProgramWithMocks_namespace
declare cclut::internalSubroutine(null) = null with protect
subroutine cclut::internalSubroutine(null)
    set test_subroutine = 1
end

/**
    Confirms that cclutExecuteProgramWithMocks can perform a mock substitutoin on a subroutine.
*/
subroutine test_cclutExecuteProgramWithMocks_mock_subroutine(null)
    declare public_subroutine = i4 with protect, noconstant(0)
    declare mock_subroutine = i4 with protect, noconstant(0)
    declare test_subroutine = i4 with protect, noconstant(0)
    declare cclut::internalVariable = i4 with protect, noconstant(0)
    record cclut::internalRecord (
        1 item = i4
    ) with protect

    call cclutAddMockImplementation("internalSubroutine", "mockSubroutine")

    call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", "10,^test param^", "cclut")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 001", public_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 002", test_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 003", mock_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 004", cclut::internalVariable, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine 005",
        cclut::internalRecord->item, 1)
end ;test_cclutExecuteProgramWithMocks_mock_subroutine
declare cclut::mockSubroutine(null) = null with protect
subroutine cclut::mockSubroutine(null)
    set mock_subroutine = 1
end


/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 6 when applying mock table substitutions
    if the CCL revision is 9 and the CCL version is 9.3.0.
*/
subroutine test_table_replace_pre_9_3(null)
    declare testver = i4 with protect, constant(90299)
    declare testNameSpace = vc with protect, constant("test_table_replace_pre_9_3")
    call table_replace_version(null)
end ;test_table_replace_pre_9_3
subroutine test_table_replace_pre_9_3::cclutGetCurrentRevision(null)
    return(9)
end
subroutine test_table_replace_pre_9_3::cclutGetCurrentVersion(null)
    return(90299)
end
subroutine test_table_replace_pre_9_3::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_pre_9_3 parser", 
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 0\)^)
    call parser(strParser)
end

/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 0 when applying mock table substitutions
    if the CCL revision is 9 and the CCL version is less than 9.3.0.
*/
subroutine test_table_replace_9_3(null)
    declare testver = i4 with protect, constant(90300)
    declare testNameSpace = vc with protect, constant("test_table_replace_9_3")
    call table_replace_version(null)
end ;test_table_replace_9_3
subroutine test_table_replace_9_3::cclutGetCurrentVersion(null)
    return(90300)
end
subroutine test_table_replace_9_3::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_9_3 parser", 
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 6\)^)
    call parser(strParser)
end

/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 0 when applying mock table substitutions
    if the CCL revision is 8 and the CCL version is less than 8.15.0.
*/
subroutine test_table_replace_pre_8_1_5(null)
    declare testver = i4 with protect, constant(81499)
    declare testNameSpace = vc with protect, constant("test_table_replace_pre_8_1_5")
    call table_replace_version(null)
end ;test_table_replace_pre_8_1_5
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentRevision(null)
    return(8)
end
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentVersion(null)
    return(81499)
end
subroutine test_table_replace_pre_8_1_5::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_pre_8_1_5 parser", 
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 0\)^)
    call parser(strParser)
end

/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 6 when applying mock table substitutions
    if the CCL revision is 8 and the CCL version is 8.15.0.
*/
subroutine test_table_replace_8_1_5(null)
    declare testver = i4 with protect, constant(81500)
    declare testNameSpace = vc with protect, constant("test_table_replace_8_1_5")
    call table_replace_version(null)
end ;test_table_replace_8_1_5
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentRevision(null)
    return(8)
end
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentVersion(null)
    return(81500)
end
subroutine test_table_replace_8_1_5::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_8_1_5 parser", 
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 6\)^)
    call parser(strParser)
end


/**
  Common code for the test_table_replace_* tests
*/
subroutine table_replace_version(null)
    if (curcclver < 81500 or (currev = 9 and curcclver < 90300))
        ;skip this test because the current CCL version is too old to test new functionality.
        call echo(concat("skipping test ", cclut::testName, " - the CCL version is ", curcclver))
        return(null)
    endif
    
    record ucetcf_request (
      1 testCaseDirectory = vc
      1 testCaseFileName = vc
      1 testNamePattern = vc
      1 programs[*]
        2 programName = vc
        2 compile = i2
      1 optimizerMode = vc
      1 enforcePredeclare = i2
      1 deprecatedFlag = vc
      1 legacyResultsFormat = i2
      1 failFast = i2
    ) with protect

    record ucetcf_reply (
      1 environmentXml = vc
      1 listingXml = vc
      1 coverageXml = vc
      1 resultsXml = vc
      1 programs[*]
        2 programName = vc
        2 listingXml = vc
        2 coverageXml = vc
%i cclsource:status_block.inc
    ) with protect

    record ucetcf_testCaseResults (
      1 resultInd = i2
      1 tests[*]
        2 name = vc
        2 asserts[*]
          3 lineNumber = i4
          3 context = vc
          3 resultInd = i2 ;TRUE = PASS, FALSE = FAIL
          3 condition = vc
        2 errors[*]
          3 lineNumber = i4
          3 errorText = vc
%i cclsource:status_block.inc
    ) with protect
    

    record cclut_testRecord (1 tableName = vc 1 value = vc)
    set ucetcf_request->testCaseDirectory = "cclsource"
    set ucetcf_request->testCaseFileName = "ut_ut_cclut_table_column_name"
    execute cclut_execute_test_case_file with
        replace("cclutrequest", ucetcf_request), 
        replace("cclutreply", ucetcf_reply),
        replace("ccluttestcaseresults", ucetcf_testCaseResults),
        curnamespace = testNameSpace
    
    call cclutAssertI2Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 2)
    call cclutAssertVcEqual(CURREF, "assert context 1", 
        ucetcf_testCaseResults->tests[1].asserts[1].context, concat(testNameSpace, " parser"))
    call cclutAssertVcEqual(CURREF, "assert context 2", ucetcf_testCaseResults->tests[1].asserts[2].context, "expected value")
    call cclutAssertStartsWith(CURREF, "assert condition 1", 
        ~operator('execute UT_CCLUT_TABLE_COLUMN_NAME with replace("SAMPLE_TABLE", CUST_CCLUT_~,
        ucetcf_testCaseResults->tests[1].asserts[1].condition)
    call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
    
    case(testver)
      of 81499:
        call cclutAssertVcEqual(CURREF, "assert success 1", 
            ucetcf_testCaseResults->tests[1].asserts[2].condition, ~"" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[1].asserts[2].resultInd, FALSE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 1)
        call cclutAssertVcOperator(CURREF, "error message", ucetcf_testCaseResults->tests[1].errors[1].errorText ,"regexplike",
            concat("%CCL-E-26-UT_CCLUT_TABLE_COLUMN_NAME\([^)]+\)[0-9]+:[0-9]+\{ST.",
                cclut_testRecord->tableName, "}The attribute \(", cclut_testRecord->tableName,
                "\) does not exist in the table \(ST\).")) ;^                
      of 81500:
        call cclutAssertVcEqual(CURREF, "assert success 1", 
            ucetcf_testCaseResults->tests[1].asserts[2].condition, ~"sample_value" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[1].asserts[2].resultInd, TRUE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 0)
      of 90299:
        call cclutAssertVcEqual(CURREF, "assert success 1", 
            ucetcf_testCaseResults->tests[1].asserts[2].condition, ~"" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[1].asserts[2].resultInd, FALSE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 1)
        call cclutAssertVcOperator(CURREF, "error message", ucetcf_testCaseResults->tests[1].errors[1].errorText ,"regexplike",
            concat("%CCL-E-26-UT_CCLUT_TABLE_COLUMN_NAME\([^)]+\)[0-9]+:[0-9]+\{ST.",
                cclut_testRecord->tableName, "}The attribute \(", cclut_testRecord->tableName,
                "\) does not exist in the table \(ST\).")) ;^
      of 90300:
        call cclutAssertVcEqual(CURREF, "assert success 1", 
            ucetcf_testCaseResults->tests[1].asserts[2].condition, ~"sample_value" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[1].asserts[2].resultInd, TRUE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 0)
      else
        call cclutAssertI2Equal(CURREF, "unexpected testver", TRUE, FALSE)
    endcase
    
end ;table_replace_version

/**
    Confirms that a CCL exception is raised if the table name passed to cclutExecuteProgramWithMocks trims to empty.
*/
subroutine test_cclutExecuteProgramWithMocks_missing_programName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutExecuteProgramWithMocks("     ", "")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_missing_programName 001",
        trim(errorMessage, 3), "regexplike",
        concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutExecuteProgramWithMocks\(\) - programName is a required field.")) ;^
end ;test_cclutExecuteProgramWithMocks_missing_programName

/**
    Confirms that cclutRemoveAllMocks removes all mock tables and implementations.
*/
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
