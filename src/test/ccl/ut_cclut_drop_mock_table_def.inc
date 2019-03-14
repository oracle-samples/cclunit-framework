declare tearDown(null) = null

declare test_dropTable_happy_with(null) = null with protect
declare test_dropTable_happy_without(null) = null with protect
declare dropTable_happy(null) = null with protect

declare test_dropTable_non_mock_with(null) = null with protect
declare test_dropTable_non_mock_without(null) = null with protect
declare dropTable_non_mock(null) = null with protect

declare test_dropTable_error_with(null) = null with protect
declare test_dropTable_error_without(null) = null with protect
declare dropTable_error(null) = null with protect

declare test_main_happy(null) = null with protect
declare test_main_happy::dropTable(tableName = vc) = null with protect

declare stat = i4 with protect, noconstant(0)

subroutine tearDown(null)
    call cclutRemoveAllMocks(null)
    rollback
end ;tearDown


/**
    Tests that cclut_drop_mock_table_def will successfully drop the definition for a CCL Unit mock table and return 
    status information in _memory_reply_string if declared.
*/
subroutine test_dropTable_happy_with(null)
    declare mockTable = vc with protect, noconstant("")
    declare _memory_reply_string = vc with protect, noconstant("")

    call dropTable_happy(null)

    set stat = cnvtjsontorec(_memory_reply_string)
    call cclutAssertI4Equal(CURREF, "cnvtjsontorec stat a", stat, 1)
    call cclutAssertVcEqual(CURREF, "cclut_drop_mock_table_def status a", cclut_drop_mock_table_def_reply->status, "S")
    call cclutAssertVcEqual(CURREF, "cclut_drop_mock_table_def message a", cclut_drop_mock_table_def_reply->message,
        concat("Table ", mockTable, " was dropped"))
end ;test_dropTable_happy_with

/**
    Tests that cclut_drop_mock_table_def will drop the definition for a CCL Unit mock without error 
    when _memory_reply_string is not declared.
*/
subroutine test_dropTable_happy_without(null)
    declare mockTable = vc with protect, noconstant("")
    call dropTable_happy(null)
end ;test_dropTable_happy_with

/** 
    Common code for test_dropTable_happy_with and test_dropTable_happy_without.
*/
subroutine dropTable_happy(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)

    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")

    ;this select should cause no problems as the table was just created.
    select into "nl:" from (value(mockTable) m) with nocounter
    set errorCode = error(errorMessage, 0)
    call cclutAssertI4Equal(CURREF, "select success", errorCode, 0)

    call cclutAddMockImplementation("reply", "cclut_drop_mock_table_def_reply")
    call cclutExecuteProgramWithMocks("cclut_drop_mock_table_def", concat("^MINE^,^", mockTable, "^"), "test_dropTable_happy")

    ;the same select should now produce an error if the table was actally dropped by cclut_drop_mock_table_def.
    select into "nl:" from (value(mockTable) m) with nocounter
    set errorCode = error(errorMessage, 0)
    call cclutAssertVcOperator(CURREF, "test_dropTable_happy 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-18-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{\}Unable to add range, ",
            "definition for table \(CUST_CCLUT_[^)]*\) not found in dictionary."))
end ;test_dropTable_happy


/**
    Tests that cclut_drop_mock_table_def will not drop the definition for a table that does not look like a CCL Unit mock table 
    and that it returns status information in _memory_reply_string if declared.
*/
subroutine test_dropTable_non_mock_with(null)
    declare _memory_reply_string = vc with protect, noconstant("")
    
    call dropTable_non_mock(null)

    set stat = cnvtjsontorec(_memory_reply_string)

    call cclutAssertI4Equal(CURREF, "cnvtjsontorec stat b", stat, 1)
    call cclutAssertVcEqual(CURREF, "cclut_drop_mock_table_def status b", cclut_drop_mock_table_def_reply->status, "F")
    call cclutAssertVcEqual(CURREF, "cclut_drop_mock_table_def message b", cclut_drop_mock_table_def_reply->message,
        "Only CCL Unit mock tables can be dropped with this program")
end ;test_dropTable_non_mock_with


/**
    Tests that cclut_drop_mock_table_def will not drop the definition for a table that does not look like a CCL Unit mock table
    and does not create errors when _memory_reply_string is not declared.
*/
subroutine test_dropTable_non_mock_without(null)
    call dropTable_non_mock(null)
end ;test_dropTable_non_mock_without

/**
    Common code for test_dropTable_non_mock_with and test_dropTable_non_mock_without.
    Creates a custom table that does not look like a CCL Unit mock table and tries to drop it using cclut_drop_mock_table_def.
    Confirms that cclut_drop_mock_table_def does not drop the table and then removes the custom table directly.
*/
subroutine dropTable_non_mock(null)
    declare mockTable = vc with protect, noconstant("")
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)

    select into table CUST__CCLUT_CUSTOM_TABLE custom_column = type("vc") with organization="P"

    ;this select should cause no problems as the table was just created.
    select into "nl:" from CUST__CCLUT_CUSTOM_TABLE with nocounter
    set errorCode = error(errorMessage, 0)
    call cclutAsserti4Equal(CURREF, "test_dropTable_non_mock 001", errorCode, 0)

    execute cclut_drop_mock_table_def ^MINE^, ^CUST__CCLUT_CUSTOM_TABLE^ with
        replace("REPLY", cclut_drop_mock_table_def_reply)

    
    ;the same select should still cause no problems if cclut_drop_mock_table_def did not drop it.
    select into "nl:" from CUST__CCLUT_CUSTOM_TABLE with nocounter
    set errorCode = error(errorMessage, 0)
    call cclutAsserti4Equal(CURREF, "test_dropTable_non_mock 002", errorCode, 0)

    drop table CUST__CCLUT_CUSTOM_TABLE
    call parser("rdb drop table CUST__CCLUT_CUSTOM_TABLE go")
end ;dropTable_non_mock

/**
    Verifies that cclut_drop_mock_table_def returns information about generated errors in _memory_reply_string if declared.
*/
subroutine test_dropTable_error_with(null)
    declare _memory_reply_string = vc with protect, noconstant("")
    call dropTable_error(null)

    set stat = cnvtjsontorec(_memory_reply_string)

    call cclutAssertI4Equal(CURREF, "cnvtjsontorec stat c", stat, 1)
    call cclutAssertVcEqual(CURREF, "cclut_drop_mock_table_def status c", cclut_drop_mock_table_def_reply->status, "F")
    call cclutAssertStartsWith(CURREF, "cclut_drop_mock_table_def message c",
                "Failed to drop table", cclut_drop_mock_table_def_reply->message)
end ;test_dropTable_error_with

/**
    Verifies that additional errors are not generated in cclut_drop_mock_table_def because _memory_reply_string is not declared
    when errors are generated in cclut_drop_mock_table_def.
*/
subroutine test_dropTable_error_without(null)
   call dropTable_error(null)
end ;test_dropTable_error_without

subroutine dropTable_error(null)
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)

    execute cclut_drop_mock_table_def ^MINE^, ^CUST_CCLUT I_AM_NOT_A_VALID_TABLE_NAME^ with 
        replace("REPLY", cclut_drop_mock_table_def_reply)
    
    set errorCode = error(errorMessage, 0)
    call cclutAsserti4Equal(CURREF, "dropTable_error 001", errorCode, 0)
end ;dropTable_error


/**
    Validates that the main subroutine calls the dropTable subroutine passing the input tableName
*/
subroutine test_main_happy(null)
    declare dropTableCalled = i4 with protect, noconstant(0)

    execute cclut_drop_mock_table_def ^MINE^,^test_table^ with  
        replace("REPLY", cclut_drop_mock_table_def_reply),
        curnamespace = "test_main_happy"

    call cclutAsserti4Equal(CURREF, "test_main_happy 002", dropTableCalled, 1)
end ;test_main_happy
subroutine test_main_happy::dropTable(tableName)
    set dropTableCalled = 1
    call cclutAssertvcEqual(CURREF, "test_main_happy 001", tableName, "test_table")
end ;;dropTable
