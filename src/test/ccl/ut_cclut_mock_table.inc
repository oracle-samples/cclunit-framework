declare setup(dummyVar = i2) = null
declare tearDown(dummyVar = i2) = null

subroutine tearDown(dummyVar)
    call cclutRemoveAllMocks(null)
    rollback
end ;tearDown

;**********************************************************************************************************************************
;** cclutDefineMockTable
;**********************************************************************************************************************************
/* test_cclutDefineMockTable_happy **************************************************************************
*  Scenario: Defines a table with columns of various types that has not been mocked or defined previously.  *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_happy(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")

    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_happy 001", "CUST_CCLUT", mockTable)

    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_happy 002", size(cclut_mockTables->tables, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 003", cclut_mockTables->tables[1].originalName,
        "SAMPLE_TABLE")
    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_happy 004", "CUST_CCLUT",
        cclut_mockTables->tables[1].replaceName)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_happy 005", size(cclut_mockTables->tables[1].columns, 5),
        3)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 006",
        cclut_mockTables->tables[1].columns[1].columnName, "SAMPLE_TABLE_ID")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 007",
        cclut_mockTables->tables[1].columns[1].columnType, "F8")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 008",
        cclut_mockTables->tables[1].columns[2].columnName, "SAMPLE_TABLE_TEXT")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 009",
        cclut_mockTables->tables[1].columns[2].columnType, "VC")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 010",
        cclut_mockTables->tables[1].columns[3].columnName, "SAMPLE_TABLE_DATE")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_happy 011",
        cclut_mockTables->tables[1].columns[3].columnType, "DQ8")
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_happy 012", size(cclut_mockTables->tables[1].indexes, 5),
        0)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_happy 013", cclut_mockTables->tables[1].isFinalized, FALSE)
end ;test_cclutDefineMockTable_happy

/* test_cclutDefineMockTable_different_cases ****************************************************************
*  Scenario: Defines a table with columns of various types that has different cases within each word        *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_different_cases(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sAmpLE_tabLE", "SaMPle_TABle_iD|sAmPlE_tAbLe_TeXt|SaMpLe_TaBlE_dAtE", "f8|VC|dQ8")

    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_different_cases 001", "CUST_CCLUT", mockTable)

    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_different_cases 002", size(cclut_mockTables->tables, 5),
        1)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 003",
        cclut_mockTables->tables[1].originalName, "SAMPLE_TABLE")
    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_different_cases 004", "CUST_CCLUT",
        cclut_mockTables->tables[1].replaceName)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_different_cases 005",
        size(cclut_mockTables->tables[1].columns, 5), 3)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 006",
        cclut_mockTables->tables[1].columns[1].columnName, "SAMPLE_TABLE_ID")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 007",
        cclut_mockTables->tables[1].columns[1].columnType, "F8")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 008",
        cclut_mockTables->tables[1].columns[2].columnName, "SAMPLE_TABLE_TEXT")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 009",
        cclut_mockTables->tables[1].columns[2].columnType, "VC")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 010",
        cclut_mockTables->tables[1].columns[3].columnName, "SAMPLE_TABLE_DATE")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_different_cases 011",
        cclut_mockTables->tables[1].columns[3].columnType, "DQ8")
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_different_cases 012",
        size(cclut_mockTables->tables[1].indexes, 5), 0)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_different_cases 013",
        cclut_mockTables->tables[1].isFinalized, FALSE)
end ;test_cclutDefineMockTable_different_cases

/* test_cclutDefineMockTable_existing_mock ******************************************************************
*  Scenario: Removes the existing mock and adds a new one when cclutDefineMockTable is called again         *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_existing_mock(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    set mockTable = cclutDefineMockTable("sample_table", "new_table_id|new_table_text|new_table_date", "i4|c100|dm12")

    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_existing_mock 001", "CUST_CCLUT", mockTable)

    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_mock 002", size(cclut_mockTables->tables, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 003",
        cclut_mockTables->tables[1].originalName, "SAMPLE_TABLE")
    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_existing_mock 004", "CUST_CCLUT",
        cclut_mockTables->tables[1].replaceName)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_mock 005",
        size(cclut_mockTables->tables[1].columns, 5), 3)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 006",
        cclut_mockTables->tables[1].columns[1].columnName, "NEW_TABLE_ID")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 007",
        cclut_mockTables->tables[1].columns[1].columnType, "I4")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 008",
        cclut_mockTables->tables[1].columns[2].columnName, "NEW_TABLE_TEXT")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 009",
        cclut_mockTables->tables[1].columns[2].columnType, "C100")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 010",
        cclut_mockTables->tables[1].columns[3].columnName, "NEW_TABLE_DATE")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_mock 011",
        cclut_mockTables->tables[1].columns[3].columnType, "DM12")
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_mock 012",
        size(cclut_mockTables->tables[1].indexes, 5), 0)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_mock 013",
        cclut_mockTables->tables[1].isFinalized, FALSE)
end ;test_cclutDefineMockTable_existing_mock

/* test_cclutDefineMockTable_existing_created_mock **********************************************************
*  Scenario: Drops the existing created mock and adds a new one when cclutDefineMockTable is called again   *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_existing_created_mock(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")

    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutDefineMockTable_existing_created_mock 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclutDefineMockTable_existing_created_mock 003", mockDate,
        cnvtdatetime("27-NOV-2018 08:00"))

    set mockTable = cclutDefineMockTable("sample_table", "new_table_id|new_table_text|new_table_date", "i4|c100|dm12")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_existing_created_mock 004", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-18-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{\}Unable to add range, definition for",
            " table \(CUST_CCLUT_[^)]*\) not found in dictionary."))

    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_existing_created_mock 005", "CUST_CCLUT", mockTable)

    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_created_mock 006",
        size(cclut_mockTables->tables, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 007",
        cclut_mockTables->tables[1].originalName, "SAMPLE_TABLE")
    call cclutAssertStartsWith(CURREF, "test_cclutDefineMockTable_existing_created_mock 008", "CUST_CCLUT",
        cclut_mockTables->tables[1].replaceName)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_created_mock 009",
        size(cclut_mockTables->tables[1].columns, 5), 3)
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 010",
        cclut_mockTables->tables[1].columns[1].columnName, "NEW_TABLE_ID")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 011",
        cclut_mockTables->tables[1].columns[1].columnType, "I4")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 012",
        cclut_mockTables->tables[1].columns[2].columnName, "NEW_TABLE_TEXT")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 013",
        cclut_mockTables->tables[1].columns[2].columnType, "C100")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 014",
        cclut_mockTables->tables[1].columns[3].columnName, "NEW_TABLE_DATE")
    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_existing_created_mock 015",
        cclut_mockTables->tables[1].columns[3].columnType, "DM12")
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_created_mock 016",
        size(cclut_mockTables->tables[1].indexes, 5), 0)
    call cclutAsserti4Equal(CURREF, "test_cclutDefineMockTable_existing_created_mock 017",
        cclut_mockTables->tables[1].isFinalized, FALSE)
end ;test_cclutDefineMockTable_existing_created_mock

/* test_cclutDefineMockTable_missing_tableName **************************************************************
*  Scenario: Throws an error when an empty tableName is supplied to the function (after trimming)           *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_missing_tableName(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("     ", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_missing_tableName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - tableName is a required field."))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_missing_tableName 002", mockTable, "")
end ;test_cclutDefineMockTable_missing_tableName

/* test_cclutDefineMockTable_missing_columnNames ************************************************************
*  Scenario: Throws an error when an empty columnNames is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_missing_columnNames(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("sample_table", "     ", "f8|vc|dq8")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_missing_columnNames 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - columnNames is a required field."))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_missing_columnNames 002", mockTable, "")
end ;test_cclutDefineMockTable_missing_columnNames

/* test_cclutDefineMockTable_missing_columnTypes ************************************************************
*  Scenario: Throws an error when an empty columnTypes is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_missing_columnTypes(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "     ")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_missing_columnTypes 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - columnTypes is a required field."))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_missing_columnTypes 002", mockTable, "")
end ;test_cclutDefineMockTable_missing_columnTypes

/* test_cclutDefineMockTable_mismatch_column_count **********************************************************
*  Scenario: Throws an error when the number of column names is different than the number of column types   *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_mismatch_column_count(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_mismatch_column_count 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutDefineMockTable\(SAMPLE_TABLE\) - Size mismatch. 3 columnNames vs. 2 columnTypes"))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_mismatch_column_count 002", mockTable, "")
end ;test_cclutDefineMockTable_mismatch_column_count

/* test_cclutDefineMockTable_missing_column_name ************************************************************
*  Scenario: Throws an error if any of the individual column names are empty (after trimming)               *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_missing_column_name(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|     |sample_table_date", "f8|vc|dq8")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_missing_column_name 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutDefineMockTable\(\) - Empty columnName found in columnNames. columnNames: ",
            "SAMPLE_TABLE_ID|     |SAMPLE_TABLE_DATE."))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_missing_column_name 002", mockTable, "")
end ;test_cclutDefineMockTable_missing_column_name

/* test_cclutDefineMockTable_missing_column_type ************************************************************
*  Scenario: Throws an error if any of the individual column types are empty (after trimming)               *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_missing_column_type(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|     |dq8")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_missing_column_type 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutDefineMockTable\(\) - Empty columnType found in columnTypes. columnTypes: ",
            "F8|     |DQ8."))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_missing_column_type 002", mockTable, "")
end ;test_cclutDefineMockTable_missing_column_type

/* test_cclutDefineMockTable_duplicate_column_name **********************************************************
*  Scenario: Throws an error if any of the column names are duplicated (names are case-insensitive)         *
************************************************************************************************************/
subroutine test_cclutDefineMockTable_duplicate_column_name(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date|SAMPLE_table_text",
            "f8|vc|dq8|vc")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutDefineMockTable_duplicate_column_name 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutDefineMockTable\(SAMPLE_TABLE\) - Duplicate columnName found. columnName: SAMPLE_TABLE_TEXT"))

    call cclutAssertvcEqual(CURREF, "test_cclutDefineMockTable_duplicate_column_name 002", mockTable, "")
end ;test_cclutDefineMockTable_duplicate_column_name

;**********************************************************************************************************************************
;** cclutAddMockIndex
;**********************************************************************************************************************************
/* test_cclutAddMockIndex_happy *****************************************************************************
*  Scenario: Adds a mock index to a table that has already been defined with cclutDefineMockTable           *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_happy(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_2", "sample_table_2_id|sample_table_2_text|sample_table_2_date",
        "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_3", "sample_table_3_id|sample_table_3_text|sample_table_3_date",
        "f8|vc|dq8")

    call cclutAddMockIndex("sample_table_2", "sample_table_2_id", 1)
    call cclutAddMockIndex("sample_table_2", "sample_table_2_text|sample_table_2_id", 0)

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockIndex_happy 001", size(cclut_mockTables->tables[2].indexes, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockIndex_happy 002", cclut_mockTables->tables[2].indexes[1].indexColumns,
        "SAMPLE_TABLE_2_ID")
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockIndex_happy 003", cclut_mockTables->tables[2].indexes[1].isUnique, 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockIndex_happy 004", cclut_mockTables->tables[2].indexes[2].indexColumns,
        "SAMPLE_TABLE_2_TEXT|SAMPLE_TABLE_2_ID")
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockIndex_happy 005", cclut_mockTables->tables[2].indexes[2].isUnique, 0)
end ;test_cclutAddMockIndex_happy

/* test_cclutAddMockIndex_missing_tableName *****************************************************************
*  Scenario: Throws an error when an empty tableName is supplied to the function (after trimming)           *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_missing_tableName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockIndex("     ", "sample_table_id", 1)

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockIndex_missing_tableName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - tableName is a required field."))
end ;test_cclutAddMockIndex_missing_tableName

/* test_cclutAddMockIndex_missing_columnNames ***************************************************************
*  Scenario: Throws an error when an empty columnNames is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_missing_columnNames(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockIndex("sample_table", "     ", 1)

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockIndex_missing_columnNames 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - columnNames is a required field."))
end ;test_cclutAddMockIndex_missing_columnNames

/* test_cclutAddMockIndex_unmocked_table ********************************************************************
*  Scenario: Throws an error when the tableName specified has not been mocked through cclutDefineMockTable  *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_unmocked_table(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockIndex("sample_table", "sample_table_id", 1)

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockIndex_unmocked_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsDefined\(SAMPLE_TABLE\) - Table has not been mocked."))
end ;test_cclutAddMockIndex_unmocked_table

/* test_cclutAddMockIndex_created_table *********************************************************************
*  Scenario: Throws an error when the tableName specified has already been created                          *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_created_table(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockIndex("sample_table", "sample_table_id", 1)

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockIndex_created_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsNotCreated\(SAMPLE_TABLE\) - Table has already been created."))
end ;test_cclutAddMockIndex_created_table

/* test_cclutAddMockIndex_invalid_column ********************************************************************
*  Scenario: Throws an error when the column is not a valid column on the mocked table                      *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_invalid_column(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockIndex("sample_table", "wrong_column", 1)

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockIndex_invalid_column 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockIndex\(SAMPLE_TABLE\) - WRONG_COLUMN is not mocked on table."))
end ;test_cclutAddMockIndex_invalid_column

/* test_cclutAddMockIndex_duplicate_column_name *************************************************************
*  Scenario: Throws an error if any of the column names are duplicated (names are case-insensitive)         *
************************************************************************************************************/
subroutine test_cclutAddMockIndex_duplicate_column_name(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockIndex("sample_table", "sample_table_text|sample_table_id|sample_table_text", 1)

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockIndex_duplicate_column_name 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockIndex\(SAMPLE_TABLE\) - Duplicate columnName found. columnName: SAMPLE_TABLE_TEXT."))
end ;test_cclutAddMockIndex_duplicate_column_name

;**********************************************************************************************************************************
;** cclutCreateMockTable
;**********************************************************************************************************************************
/* test_cclutCreateMockTable_happy **************************************************************************
*  Scenario: Adds a mock table to the RDBMS with the values defined in cclutDefineMockTable                 *
************************************************************************************************************/
subroutine test_cclutCreateMockTable_happy(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")

    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutCreateMockTable_happy 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclutCreateMockTable_happy 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclutCreateMockTable_happy 003", mockDate, cnvtdatetime("27-NOV-2018 08:00"))
end ;test_cclutCreateMockTable_happy

/* test_cclutCreateMockTable_missing_tableName **************************************************************
*  Scenario: Throws an error when an empty tableName is supplied to the function (after trimming)           *
************************************************************************************************************/
subroutine test_cclutCreateMockTable_missing_tableName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutCreateMockTable("     ")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutCreateMockTable_missing_tableName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - tableName is a required field."))
end ;test_cclutCreateMockTable_missing_tableName

/* test_cclutCreateMockTable_unmocked_table *****************************************************************
*  Scenario: Throws an error when the tableName specified has not been mocked through cclutDefineMockTable  *
************************************************************************************************************/
subroutine test_cclutCreateMockTable_unmocked_table(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutCreateMockTable("sample_table")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutCreateMockTable_unmocked_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsDefined\(SAMPLE_TABLE\) - Table has not been mocked."))
end ;test_cclutCreateMockTable_unmocked_table

/* test_cclutCreateMockTable_created_table ******************************************************************
*  Scenario: Does nothing if the table has already been created                                             *
************************************************************************************************************/
subroutine test_cclutCreateMockTable_created_table(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")

    call cclutCreateMockTable("sample_table")

    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutCreateMockTable_happy 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclutCreateMockTable_happy 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclutCreateMockTable_happy 003", mockDate, cnvtdatetime("27-NOV-2018 08:00"))
end ;test_cclutCreateMockTable_created_table

;**********************************************************************************************************************************
;** cclutRemoveMockTable
;**********************************************************************************************************************************
/* test_cclutRemoveMockTable_happy **************************************************************************
*  Scenario: Removes a mock table that has not been created yet                                             *
************************************************************************************************************/
subroutine test_cclutRemoveMockTable_happy(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutRemoveMockTable("sample_table")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_happy 001", size(cclut_mockTables->tables, 5), 0)
end ;test_cclutRemoveMockTable_happy

/* test_cclutRemoveMockTable_created_table ******************************************************************
*  Scenario: Removes a mock table that has been created and drops it from the RDBMS                         *
************************************************************************************************************/
subroutine test_cclutRemoveMockTable_created_table(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")

    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutRemoveMockTable_created_table 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclutRemoveMockTable_created_table 003", mockDate,
        cnvtdatetime("27-NOV-2018 08:00"))

    call cclutRemoveMockTable("sample_table")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutRemoveMockTable_created_table 004", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-18-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{\}Unable to add range, definition for",
            " table \(CUST_CCLUT_[^)]*\) not found in dictionary."))

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table 005", size(cclut_mockTables->tables, 5), 0)
end ;test_cclutRemoveMockTable_created_table

/* test_cclutRemoveMockTable_created_table_group1 ***********************************************************
*  Scenario: Removes a mock table that has been created and drops the CCL definition as a group1 user       *
************************************************************************************************************/
subroutine test_cclutRemoveMockTable_created_table_group1(null)
    free record cclut_drop_table_reply
    record cclut_drop_table_reply (
%i cclsource:status_block.inc
    ) with protect
    set cclut_drop_table_reply->status_data.status = "S"

    declare mockTdbexecuteWasCalled = i4 with protect, noconstant(0)
    declare mock_cclutRemoveMockTable_created_table_group1_tdbexecute(appNum = f8, taskNum = f8, reqNum = f8,
        reqType = vc, req = vc(ref), repType = vc, rep = vc(ref)) = i4 with protect
    subroutine mock_cclutRemoveMockTable_created_table_group1_tdbexecute(appNum, taskNum, reqNum, reqType, req, repType,
        rep)
            call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1 001", appNum, 3070000)
            call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1 002", taskNum, 3070001)
            call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1 003", reqNum, 3050002)
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1 004", reqType, "REC")
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1 005",
                cclut_drop_table_request->program_name, "cclut_drop_table_definition")
            call cclutAssertStartsWith(CURREF, "test_cclutRemoveMockTable_created_table_group1 006",
                concat("^MINE^,^CUST_CCLUT"), cclut_drop_table_request->params)
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1 007", repType, "REC")
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1 008",
                cclut_drop_table_reply->status_data.status, "S")
            set mockTdbexecuteWasCalled = 1
            return(0)
    end

    call cclutAddMockImplementation("tdbexecute", "mock_cclutremovemocktable_created_table_group1_tdbexecute")
    call cclutExecuteProgramWithMocks("ut_cclut_removeMockTable_grp1", "")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1 009", mockTdbexecuteWasCalled, 1)
end ;test_cclutRemoveMockTable_created_table_group1

/* test_cclutRemoveMockTable_created_table_group1_error *****************************************************
*  Scenario: Generates an error when attempting to drop the CCL definition as a group1 user                 *
************************************************************************************************************/
subroutine test_cclutRemoveMockTable_created_table_group1_error(null)
    free record cclut_drop_table_reply
    record cclut_drop_table_reply (
%i cclsource:status_block.inc
    ) with protect
    set cclut_drop_table_reply->status_data.status = "S"

    declare mockTdbexecuteWasCalled = i4 with protect, noconstant(0)
    declare mock_cclutRemoveMockTable_created_table_group1_error_tdbexecute(appNum = f8, taskNum = f8, reqNum = f8,
        reqType = vc, req = vc(ref), repType = vc, rep = vc(ref)) = i4 with protect
    subroutine mock_cclutRemoveMockTable_created_table_group1_error_tdbexecute(appNum, taskNum, reqNum, reqType, req,
        repType, rep)
            call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 001", appNum, 3070000)
            call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 002", taskNum,
                3070001)
            call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 003", reqNum, 3050002)
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 004", reqType, "REC")
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 005",
                cclut_drop_table_request->program_name, "cclut_drop_table_definition")
            call cclutAssertStartsWith(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 006",
                concat("^MINE^,^CUST_CCLUT"), cclut_drop_table_request->params)
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 007", repType, "REC")
            call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 008",
                cclut_drop_table_reply->status_data.status, "S")
            set mockTdbexecuteWasCalled = 1
            return(1)
    end

    call cclutAddMockImplementation("tdbexecute", "mock_cclutremovemocktable_created_table_group1_error_tdbexecute")
    call cclutExecuteProgramWithMocks("ut_cclut_removeMockTable_grp1", "")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 009",
        trim(errorMessage, 3), "regexplike",
        concat("%CCL-E-392-UT_CCLUT_REMOVEMOCKTABLE_GRP1\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutRemoveMockTable\(SAMPLE_TABLE\) - Unable to drop table.  Manual cleanup may be required.  ",
            "Mock table name is.*"))
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockTable_created_table_group1_error 010", mockTdbexecuteWasCalled,
        1)
end ;test_cclutRemoveMockTable_created_table_group1

/* test_cclutRemoveMockTable_missing_tableName **************************************************************
*  Scenario: Throws an error when an empty tableName is supplied to the function (after trimming)           *
************************************************************************************************************/
subroutine test_cclutRemoveMockTable_missing_tableName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutRemoveMockTable("     ")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutRemoveMockTable_missing_tableName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - tableName is a required field."))
end ;test_cclutRemoveMockTable_missing_tableName

;**********************************************************************************************************************************
;** cclutRemoveAllMockTables
;**********************************************************************************************************************************
/* test_cclutRemoveAllMockTables_happy **********************************************************************
*  Scenario: Removes all mock tables that have been mocked regardless of whether they have been created     *
************************************************************************************************************/
subroutine test_cclutRemoveAllMockTables_happy(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_2", "sample_table_2_id|sample_table_2_text|sample_table_2_date",
        "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_3", "sample_table_3_id|sample_table_3_text|sample_table_3_date",
        "f8|vc|dq8")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockTables_happy 001", size(cclut_mockTables->tables, 5), 3)

    call cclutRemoveAllMockTables(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockTables_happy 002", size(cclut_mockTables->tables, 5), 0)
end ;test_cclutRemoveAllMockTables_happy

;**********************************************************************************************************************************
;** cclutAddMockData
;**********************************************************************************************************************************
/* test_cclutAddMockData_happy ******************************************************************************
*  Scenario: Adds the appropriate mock data to the table when the data has no escape characters or nulls    *
************************************************************************************************************/
subroutine test_cclutAddMockData_happy(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")
    call cclutAddMockData("sample_table", "2.0|test \|text\\ 2|28-NOV-2018 09:00")
    call cclutAddMockData("sample_table", "3.0|test text 3|29-NOV-2018 10:00")

    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    where m.SAMPLE_TABLE_ID = 1.0
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutAddMockData_happy 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_happy 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclutAddMockData_happy 003", mockDate, cnvtdatetime("27-NOV-2018 08:00"))

    select into "nl:"
    from (value(mockTable) m)
    where m.SAMPLE_TABLE_ID = 2.0
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutAddMockData_happy 004", mockId, 2.0)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_happy 005", mockText, "test |text\ 2")
    call cclutAssertf8Equal(CURREF, "test_cclutAddMockData_happy 006", mockDate, cnvtdatetime("28-NOV-2018 09:00"))

    select into "nl:"
    from (value(mockTable) m)
    where m.SAMPLE_TABLE_ID = 3.0
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclutAddMockData_happy 007", mockId, 3.0)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_happy 008", mockText, "test text 3")
    call cclutAssertf8Equal(CURREF, "test_cclutAddMockData_happy 009", mockDate, cnvtdatetime("29-NOV-2018 10:00"))
end ;test_cclutAddMockData_happy

/* test_cclutAddMockData_empty_single_column ****************************************************************
*  Scenario: Correctly handles adding an empty string to a table with a single column when rowData is empty *
************************************************************************************************************/
subroutine test_cclutAddMockData_empty_single_column(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_text", "vc")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", char(0))

    declare mockLength = i4 with protect, noconstant(0)
    declare mockIChar = i4 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockLength = textlen(m.SAMPLE_TABLE_TEXT)
        mockIChar = ichar(m.SAMPLE_TABLE_TEXT)
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 001", mockLength, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 002", mockIChar, 0)

    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "")

    declare mockLength = i4 with protect, noconstant(0)
    declare mockIChar = i4 with protect, noconstant(0)
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockLength = textlen(m.SAMPLE_TABLE_TEXT)
        mockIChar = ichar(m.SAMPLE_TABLE_TEXT)
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 003", mockLength, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 004", mockIChar, 0)

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", notrim(" "))

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockLength = textlen(m.SAMPLE_TABLE_TEXT)
        mockIChar = ichar(m.SAMPLE_TABLE_TEXT)
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 005", mockLength, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 006", mockIChar, 32)

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", trim(""))

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockLength = textlen(m.SAMPLE_TABLE_TEXT)
        mockIChar = ichar(m.SAMPLE_TABLE_TEXT)
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 007", mockLength, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 008", mockIChar, 32)

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", trim(" "))

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockLength = textlen(m.SAMPLE_TABLE_TEXT)
        mockIChar = ichar(m.SAMPLE_TABLE_TEXT)
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 009", mockLength, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_empty_single_column 010", mockIChar, 32)
end ;test_cclutAddMockData_empty_single_column

/* test_cclutAddMockData_empty_string_tests *****************************************************************
*  Scenario: Correctly handles empty strings regardless of where they are in the rowData parameter          *
************************************************************************************************************/
subroutine test_cclutAddMockData_empty_string_tests(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_text_1|sample_table_text_2|sample_table_text_3", "vc|vc|vc")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "|test|test 2")

    declare mockText1 = vc with protect, noconstant("")
    declare mockText2 = vc with protect, noconstant("")
    declare mockText3 = vc with protect, noconstant("")
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 001", mockText1, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 002", mockText2, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 003", mockText3, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test||test 2")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 004", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 005", mockText2, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 006", mockText3, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|test 2|")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 007", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 008", mockText2, "test 2")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 009", mockText3, "")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "||test")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 010", mockText1, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 011", mockText2, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 012", mockText3, "test")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "|test|")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 013", mockText1, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 014", mockText2, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 015", mockText3, "")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test||")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 016", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 017", mockText2, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 018", mockText3, "")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "||")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 019", mockText1, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 020", mockText2, "")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_empty_string_tests 021", mockText3, "")
end ;test_cclutAddMockData_empty_string_tests

/* test_cclutAddMockData_backslash_tests ********************************************************************
*  Scenario: Correctly handles strings with backslashes                                                     *
************************************************************************************************************/
subroutine test_cclutAddMockData_backslash_tests(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_text_1|sample_table_text_2|sample_table_text_3", "vc|vc|vc")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "\\\\\\|test|test 2")

    declare mockText1 = vc with protect, noconstant("")
    declare mockText2 = vc with protect, noconstant("")
    declare mockText3 = vc with protect, noconstant("")
    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 001", mockText1, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 002", mockText2, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 003", mockText3, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|\\\\\\|test 2")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 004", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 005", mockText2, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 006", mockText3, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|test 2|\\\\\\")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 007", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 008", mockText2, "test 2")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 009", mockText3, "\\\")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "\\\\\\|\\\\\\|test")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 010", mockText1, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 011", mockText2, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 012", mockText3, "test")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "\\\\\\|test|\\\\\\")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 013", mockText1, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 014", mockText2, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 015", mockText3, "\\\")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|\\\\\\|\\\\\\")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 016", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 017", mockText2, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 018", mockText3, "\\\")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "\\\\\\|\\\\\\|\\\\\\")

    select into "nl:"
    from (value(mockTable) m)
    detail
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 019", mockText1, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 020", mockText2, "\\\")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_backslash_tests 021", mockText3, "\\\")
end ;test_cclutAddMockData_backslash_tests

/* test_cclutAddMockData_null_tests *************************************************************************
*  Scenario: Correctly handles strings with nulls                                                           *
************************************************************************************************************/
subroutine test_cclutAddMockData_null_tests(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_text_1|sample_table_text_2|sample_table_text_3", "vc|vc|vc")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "\null|test|test 2")

    declare mockNull1 = i4 with protect, noconstant(0)
    declare mockNull2 = i4 with protect, noconstant(0)
    declare mockNull3 = i4 with protect, noconstant(0)
    declare mockText1 = vc with protect, noconstant("")
    declare mockText2 = vc with protect, noconstant("")
    declare mockText3 = vc with protect, noconstant("")
    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
        mockText2 = m.SAMPLE_TABLE_TEXT_2
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 001", mockNull1, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 002", mockNull2, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 003", mockNull3, 0)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 004", mockText2, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 005", mockText3, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|\null|test 2")

    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 006", mockNull1, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 007", mockNull2, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 008", mockNull3, 0)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 009", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 010", mockText3, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|test 2|\null")

    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
        mockText1 = m.SAMPLE_TABLE_TEXT_1
        mockText2 = m.SAMPLE_TABLE_TEXT_2
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 011", mockNull1, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 012", mockNull2, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 013", mockNull3, 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 014", mockText1, "test")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 015", mockText2, "test 2")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "\null|\null|test")

    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
        mockText3 = m.SAMPLE_TABLE_TEXT_3
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 016", mockNull1, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 017", mockNull2, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 018", mockNull3, 0)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 019", mockText3, "test")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "\null|test|\null")

    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
        mockText2 = m.SAMPLE_TABLE_TEXT_2
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 020", mockNull1, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 021", mockNull2, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 022", mockNull3, 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 023", mockText2, "test")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "test|\null|\null")

    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
        mockText1 = m.SAMPLE_TABLE_TEXT_1
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 024", mockNull1, 0)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 025", mockNull2, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 026", mockNull3, 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockData_null_tests 027", mockText1, "test")

    call cclutClearMockData("sample_table")
    call cclutAddMockData("sample_table", "\null|\null|\null")

    select into "nl:"
    sampleTableText1Null = nullind(m.SAMPLE_TABLE_TEXT_1),
    sampleTableText2Null = nullind(m.SAMPLE_TABLE_TEXT_2),
    sampleTableText3Null = nullind(m.SAMPLE_TABLE_TEXT_3)
    from (value(mockTable) m)
    detail
        mockNull1 = sampleTableText1Null
        mockNull2 = sampleTableText2Null
        mockNull3 = sampleTableText3Null
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 028", mockNull1, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 029", mockNull2, 1)
    call cclutAsserti4Equal(CURREF, "test_cclutAddMockData_null_tests 030", mockNull3, 1)
end ;test_cclutAddMockData_null_tests

/* test_cclutAddMockData_missing_tableName ******************************************************************
*  Scenario: Throws an error when an empty tableName is supplied to the function (after trimming)           *
************************************************************************************************************/
subroutine test_cclutAddMockData_missing_tableName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("     ", "1.0|test text|27-NOV-2018 08:00")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_missing_tableName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - tableName is a required field."))
end ;test_cclutAddMockData_missing_tableName

/* test_cclutAddMockData_unmocked_table *********************************************************************
*  Scenario: Throws an error when the tableName specified has not been mocked through cclutDefineMockTable  *
************************************************************************************************************/
subroutine test_cclutAddMockData_unmocked_table(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_unmocked_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsDefined\(SAMPLE_TABLE\) - Table has not been mocked."))
end ;test_cclutAddMockData_unmocked_table

/* test_cclutAddMockData_uncreated_table ********************************************************************
*  Scenario: Throws an error when the tableName specified has not been created through cclutCreateMockTable *
************************************************************************************************************/
subroutine test_cclutAddMockData_uncreated_table(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_uncreated_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsCreated\(SAMPLE_TABLE\) - Table has not been created."))
end ;test_cclutAddMockData_uncreated_table

/* test_cclutAddMockData_too_many_columns *******************************************************************
*  Scenario: Throws an error when there are more columns in the rowData than are defined for the table      *
************************************************************************************************************/
subroutine test_cclutAddMockData_too_many_columns(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00|extra")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_too_many_columns 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutConstructDBSetStatement\(SAMPLE_TABLE\) - Too many columns of data added to table."))
end ;test_cclutAddMockData_too_many_columns

/* test_cclutAddMockData_too_few_columns ********************************************************************
*  Scenario: Throws an error when there are fewer columns in the rowData than are defined for the table     *
************************************************************************************************************/
subroutine test_cclutAddMockData_too_few_columns(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("sample_table", "1.0|test text")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_too_few_columns 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutParseColumn\(\) - Too few columns of data. rowData: 1.0|test text."))
end ;test_cclutAddMockData_too_few_columns

/* test_cclutAddMockData_invalid_escape_sequence ************************************************************
*  Scenario: Throws an error when the sequence after the escape character is not valid per the API          *
************************************************************************************************************/
subroutine test_cclutAddMockData_invalid_escape_sequence(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("sample_table", "1.0|\a|27-NOV-2018 08:00")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_invalid_escape_sequence 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockData\(\) - Invalid escape sequence. rowData: 1.0|\a|27-NOV-2018 08:00."))
end ;test_cclutAddMockData_invalid_escape_sequence

/* test_cclutAddMockData_invalid_null ***********************************************************************
*  Scenario: Throws an error if the null escape sequence is not by itself in a column                       *
************************************************************************************************************/
subroutine test_cclutAddMockData_invalid_null(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutAddMockData("sample_table", "1.0|\nulla|27-NOV-2018 08:00")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutAddMockData_invalid_null 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutAddMockData\(\) - \null column must have no other characters.",
            " rowData: 1.0|\nulla|27-NOV-2018 08:00."))
end ;test_cclutAddMockData_invalid_null

;**********************************************************************************************************************************
;** cclutClearMockData
;**********************************************************************************************************************************
/* test_cclutClearMockData_happy ****************************************************************************
*  Scenario: Clears all data from the mock table but does not drop the table (essentially a truncate)       *
************************************************************************************************************/
subroutine test_cclutClearMockData_happy(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")
    call cclutAddMockData("sample_table", "2.0|test text 2|28-NOV-2018 09:00")
    call cclutAddMockData("sample_table", "3.0|test text 3|29-NOV-2018 10:00")

    declare tableCount = i4 with protect, noconstant(0)

    select into "nl:"
        totalCount = count(*)
    from (value(mockTable) m)
    head report
        tableCount = totalCount
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutClearMockData_happy 001", tableCount, 3)

    call cclutClearMockData("sample_table")

    select into "nl:"
        totalCount = count(*)
    from (value(mockTable) m)
    head report
        tableCount = totalCount
    with nocounter

    call cclutAsserti4Equal(CURREF, "test_cclutClearMockData_happy 002", tableCount, 0)
end ;test_cclutClearMockData_happy

/* test_cclutClearMockData_missing_tableName ****************************************************************
*  Scenario: Throws an error when an empty tableName is supplied to the function (after trimming)           *
************************************************************************************************************/
subroutine test_cclutClearMockData_missing_tableName(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutClearMockData("     ")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutClearMockData_missing_tableName 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateParameterIsNotEmpty\(\) - tableName is a required field."))
end ;test_cclutClearMockData_missing_tableName

/* test_cclutClearMockData_unmocked_table *******************************************************************
*  Scenario: Throws an error when the tableName specified has not been mocked through cclutDefineMockTable  *
************************************************************************************************************/
subroutine test_cclutClearMockData_unmocked_table(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutClearMockData("sample_table")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutClearMockData_unmocked_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsDefined\(SAMPLE_TABLE\) - Table has not been mocked."))
end ;test_cclutClearMockData_unmocked_table

/* test_cclutClearMockData_uncreated_table ******************************************************************
*  Scenario: Throws an error when the tableName specified has not been created through cclutCreateMockTable *
************************************************************************************************************/
subroutine test_cclutClearMockData_uncreated_table(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    call cclutClearMockData("sample_table")

    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_cclutClearMockData_uncreated_table 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\):",
            " cclutValidateMockTableIsCreated\(SAMPLE_TABLE\) - Table has not been created."))
end ;test_cclutClearMockData_uncreated_table
