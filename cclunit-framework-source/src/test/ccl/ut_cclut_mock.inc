declare setup(dummyVar = i2) = null
declare tearDown(dummyVar = i2) = null

subroutine setup(dummyVar)
    null
end ;setup

subroutine tearDown(dummyVar)
    call cclutRemoveAllMocks(null)
	rollback
end ;tearDown

;**********************************************************************************************************************************
;** cclutAddMockImplementation
;**********************************************************************************************************************************
/* test_cclutAddMockImplementation_happy ********************************************************************
*  Scenario: Adds a mock implementation that can be used with cclutExecuteProgramWithMocks                  *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_happy(null)
	call echo("~~~***~~~***test_cclutAddMockImplementation_happy***~~~***~~~")

	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy

/* test_cclutAddMockImplementation_existing_implementation **************************************************
*  Scenario: Replaces the mock name of an implementation if it was already added                            *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_existing_implementation(null)
	call echo("~~~***~~~***test_cclutAddMockImplementation_existing_implementation***~~~***~~~")

	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("sample_subroutine", "new_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy",
        cclut_mockImplementations->implementations[1].replaceName, "NEW_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy

/* test_cclutAddMockImplementation_missing_originalName *****************************************************
*  Scenario: Throws an error when an empty originalName is supplied to the function (after trimming)        *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_missing_originalName(null)
	call echo("~~~***~~~***test_cclutAddMockImplementation_missing_originalName***~~~***~~~")

	declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
	call cclutAddMockImplementation("     ", "mock_subroutine")

	set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockImplementation_missing_originalName", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockImplementation\(\) - originalName is a required field."))
end ;test_cclutAddMockImplementation_missing_originalName

/* test_cclutAddMockImplementation_missing_replaceName ******************************************************
*  Scenario: Throws an error when an empty replaceName is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_missing_replaceName(null)
	call echo("~~~***~~~***test_cclutAddMockImplementation_missing_replaceName***~~~***~~~")

	declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
	call cclutAddMockImplementation("sample_subroutine", "     ")

	set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockImplementation_missing_replaceName", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockImplementation\(\) - replaceName is a required field."))
end ;test_cclutAddMockImplementation_missing_replaceName

;**********************************************************************************************************************************
;** cclutRemoveMockImplementation
;**********************************************************************************************************************************
/* test_cclutRemoveMockImplementation_happy *****************************************************************
*  Scenario: Removes a mock implementation so that the original can be used                                 *
************************************************************************************************************/
subroutine test_cclutRemoveMockImplementation_happy(null)
	call echo("~~~***~~~***test_cclutRemoveMockImplementation_happy***~~~***~~~")

	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockImplementation_happy",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutRemoveMockImplementation("sample_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockImplementation_happy",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveMockImplementation_happy

/* test_cclutRemoveMockImplementation_missing_originalName *****************************************************
*  Scenario: Throws an error when an empty originalName is supplied to the function (after trimming)        *
************************************************************************************************************/
subroutine test_cclutRemoveMockImplementation_missing_originalName(null)
	call echo("~~~***~~~***test_cclutRemoveMockImplementation_missing_originalName***~~~***~~~")

	declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
	call cclutRemoveMockImplementation("     ")

	set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutRemoveMockImplementation_missing_originalName", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutRemoveMockImplementation\(\) - originalName is a required field."))
end ;test_cclutRemoveMockImplementation_missing_originalName

;**********************************************************************************************************************************
;** cclutRemoveAllMockImplementations
;**********************************************************************************************************************************
/* test_cclutRemoveAllMockImplementations_happy *************************************************************
*  Scenario: Removes all mock implementations so that the originals can be used                             *
************************************************************************************************************/
subroutine test_cclutRemoveAllMockImplementations_happy(null)
	call echo("~~~***~~~***test_cclutRemoveAllMockImplementations_happy***~~~***~~~")

	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")
	call cclutAddMockImplementation("sample_subroutine_2", "mock_subroutine_2")
	call cclutAddMockImplementation("sample_subroutine_3", "mock_subroutine_3")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockImplementations_happy",
        size(cclut_mockImplementations->implementations, 5), 3)

    call cclutRemoveAllMockImplementations(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockImplementations_happy",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveAllMockImplementations_happy

;**********************************************************************************************************************************
;** cclutExecuteProgramWithMocks
;**********************************************************************************************************************************
/* test_cclutExecuteProgramWithMocks_happy ******************************************************************
*  Scenario: Executes the program using mock tables and implementations that have been set up               *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_happy(null)
	call echo("~~~***~~~***test_cclutExecuteProgramWithMocks_happy***~~~***~~~")

	record executeReply (
        1 number_parameter = i4
        1 string_parameter = vc
        1 public_subroutine = i4
        1 test_subroutine = i4
        1 mock_subroutine = i4
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

	call cclutExecuteProgramWithMocks("cclut_execute_mocks_happy", "10,^test param^")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->number_parameter, 10)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->string_parameter,
        "test param")
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->public_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->test_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->mock_subroutine, 0)

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", size(executeReply->regular_join, 5), 1)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->regular_join[1].person_id,
        1.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->regular_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->regular_join[1].encounter_alias, "test alias")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", size(executeReply->left_join, 5), 3)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->left_join[1].person_id,
        1.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->left_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->left_join[1].encounter_alias, "test alias")
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->left_join[2].person_id,
        2.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->left_join[2].encounter_id, 5.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->left_join[2].encounter_alias, "")
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->left_join[3].person_id,
        3.0)
    call cclutAssertf8Equal(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->left_join[3].encounter_id, 0.0)
    call cclutAssertvcEqual(CURREF, "test_cclutExecuteProgramWithMocks_happy",
        executeReply->left_join[3].encounter_alias, "")

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

    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->rdb_join[1].columns,
        "regexplike", "PERSON_ID.*ENCOUNTER_ID.*PERSON_ID.*ENCOUNTER_ID.*ENCOUNTER_ALIAS")
    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_happy", executeReply->rdb_join[1].data,
        "regexplike", "1.*4.*1.*4.*test alias")
end ;test_cclutExecuteProgramWithMocks_happy

declare CCLUT::InternalSubroutine(null) = null with protect

subroutine (CCLUT::InternalSubroutine(null) = null with protect)
    set executeReply->test_subroutine = 1
end

/* test_cclutExecuteProgramWithMocks_namespace **************************************************************
*  Scenario: Executes the program using the supplied namespace                                              *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_namespace(null)
	call echo("~~~***~~~***test_cclutExecuteProgramWithMocks_namespace***~~~***~~~")

	record executeReply (
        1 number_parameter = i4
        1 string_parameter = vc
        1 public_subroutine = i4
        1 test_subroutine = i4
        1 mock_subroutine = i4
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

	call cclutExecuteProgramWithMocks("cclut_execute_mocks_happy", "10,^test param^", "CCLUT")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace", executeReply->public_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace", executeReply->test_subroutine, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_namespace", executeReply->mock_subroutine, 0)
end ;test_cclutExecuteProgramWithMocks_namespace

declare CCLUT::MockSubroutine(null) = null with protect

subroutine (CCLUT::MockSubroutine(null) = null with protect)
    set executeReply->mock_subroutine = 1
end

/* test_cclutExecuteProgramWithMocks_mock_subroutine ********************************************************
*  Scenario: Executes the program while replacing a subroutine with a mocked version                        *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_mock_subroutine(null)
	call echo("~~~***~~~***test_cclutExecuteProgramWithMocks_mock_subroutine***~~~***~~~")

	record executeReply (
        1 number_parameter = i4
        1 string_parameter = vc
        1 public_subroutine = i4
        1 test_subroutine = i4
        1 mock_subroutine = i4
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
	call cclutAddMockImplementation("InternalSubroutine", "MockSubroutine")

	call cclutExecuteProgramWithMocks("cclut_execute_mocks_happy", "10,^test param^", "CCLUT")

    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine", executeReply->public_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine", executeReply->test_subroutine, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutExecuteProgramWithMocks_mock_subroutine", executeReply->mock_subroutine, 1)
end ;test_cclutExecuteProgramWithMocks_mock_subroutine

/* test_cclutExecuteProgramWithMocks_missing_programName ****************************************************
*  Scenario: Throws an error when an empty programName is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutExecuteProgramWithMocks_missing_programName(null)
	call echo("~~~***~~~***test_cclutExecuteProgramWithMocks_missing_programName***~~~***~~~")

	declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
	call cclutExecuteProgramWithMocks("     ", "")

	set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutExecuteProgramWithMocks_missing_programName", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutExecuteProgramWithMocks\(\) - programName is a required field."))
end ;test_cclutExecuteProgramWithMocks_missing_programName

;**********************************************************************************************************************************
;** cclutRemoveAllMocks
;**********************************************************************************************************************************
/* test_cclutRemoveAllMocks_happy ***************************************************************************
*  Scenario: Removes all mock tables and implementations so that the originals can be used                  *
************************************************************************************************************/
subroutine test_cclutRemoveAllMocks_happy(null)
	call echo("~~~***~~~***test_cclutRemoveAllMocks_happy***~~~***~~~")

	call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_2", "sample_table_2_id|sample_table_2_text|sample_table_2_date",
        "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_3", "sample_table_3_id|sample_table_3_text|sample_table_3_date",
        "f8|vc|dq8")

    call cclutCreateMockTable("sample_table")

	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")
	call cclutAddMockImplementation("sample_subroutine_2", "mock_subroutine_2")
	call cclutAddMockImplementation("sample_subroutine_3", "mock_subroutine_3")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy",
        size(cclut_mockTables->tables, 5), 3)
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy",
        size(cclut_mockImplementations->implementations, 5), 3)

    call cclutRemoveAllMocks(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy", size(cclut_mockTables->tables, 5), 0)
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveAllMocks_happy