declare tearDown(dummyVar = i2) = null

subroutine tearDown(dummyVar)
    call cclutRemoveAllMocks(null)
    rollback
end ;tearDown

;**********************************************************************************************************************************
;** dropTable
;**********************************************************************************************************************************
/* test_dropTable_happy *************************************************************************************
*  Scenario: Drops a CCL table definition (the program is intended to be used by group1 users)              *
************************************************************************************************************/
subroutine test_dropTable_happy(null)
    declare mockTable = vc with protect, noconstant("")
    set mockTable =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    select into "nl:"
    from (value(mockTable) m)
    with nocounter

    declare test_dropTable_happy::main(null) = null with protect
    subroutine test_dropTable_happy::main(null)
        call dropTable(mockTable)
    end

    call cclutExecuteProgramWithMocks("cclut_drop_table_definition", concat("^MINE^,^", mockTable, "^"),
        "test_dropTable_happy")

    select into "nl:"
    from (value(mockTable) m)
    with nocounter

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set errorCode = error(errorMessage, 0)

    call cclutAssertVcOperator(CURREF, "test_dropTable_happy 001", trim(errorMessage, 3),
        "regexplike", concat("%CCL-E-18-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{\}Unable to add range, ",
            "definition for table \(CUST_CCLUT_[^)]*\) not found in dictionary."))
end ;test_dropTable_happy

/* test_dropTable_invalid_table *****************************************************************************
*  Scenario: Does not drop the CCL table definition if the table is not prepended with CUST_CCLUT           *
************************************************************************************************************/
subroutine test_dropTable_invalid_table(null)
    select into table CUST__CCLUT_INVALID_TABLE
    invalid_column = type("vc")
    with organization="P"

    declare test_dropTable_invalid_table::main(null) = null with protect
    subroutine test_dropTable_invalid_table::main(null)
        call dropTable("CUST__CCLUT_INVALID_TABLE")
    end

    call cclutExecuteProgramWithMocks("cclut_drop_table_definition", concat("^MINE^,^CUST__CCLUT_INVALID_TABLE^"),
        "test_dropTable_invalid_table")

    select into "nl:"
    from CUST__CCLUT_INVALID_TABLE
    with nocounter

    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    set errorCode = error(errorMessage, 0)

    call cclutAsserti4Equal(CURREF, "test_dropTable_invalid_table 001", errorCode, 0)

    drop table CUST__CCLUT_INVALID_TABLE
    rdb drop table CUST__CCLUT_INVALID_TABLE end
end ;test_dropTable_invalid_table

;**********************************************************************************************************************************
;** main
;**********************************************************************************************************************************
/* test_main_happy ******************************************************************************************
*  Scenario: Validates that the main function calls the dropTable with the tableName                        *
************************************************************************************************************/
subroutine test_main_happy(null)
    declare dropTableCalled = i4 with protect, noconstant(0)

    declare test_main_happy::dropTable(tableName = vc) = null with protect
    subroutine test_main_happy::dropTable(tableName)
        set dropTableCalled = 1
        call cclutAssertvcEqual(CURREF, "test_main_happy 001", tableName, "test_table")
    end

    call cclutExecuteProgramWithMocks("cclut_drop_table_definition", concat("^MINE^,^test_table^"),
        "test_main_happy")

    call cclutAsserti4Equal(CURREF, "test_main_happy 002", dropTableCalled, 1)
end ;test_main_happy
