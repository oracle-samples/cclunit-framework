%#ifndef CCLUTMOCK_TABLE_INC
%#def CCLUTMOCK_TABLE_INC
;**********************************************************************************************************************************
;
; Provides support functions that can be leveraged by consumers to construct mock tables and add data to them to be used in the
; script under test.
;
;**********************************************************************************************************************************

%i cclsource:cclut_utils.inc

declare CCLUT_BACKSLASH = vc with protect, constant("\")
declare CCLUT_ESCAPED_BACKSLASH = vc with protect, constant("\\")
declare CCLUT_PIPE = vc with protect, constant("|")
declare CCLUT_ESCAPED_PIPE = vc with protect, constant("\|")
declare CCLUT_ESCAPED_NULL = vc with protect, constant("\null")

if (validate(cclutMockTableNumber) = FALSE)
    declare cclutMockTableNumber = i4 with persist, noconstant(0)
endif
declare public::cclutDefineMockTable(cclutTableName = vc(val), cclutFieldNames = vc(val),
    cclutFieldTypes = vc(val), cclutIsAccessedViaRdbCommands= i2(val, FALSE)) = vc with protect
declare public::cclutAddMockIndex(cclutTableName = vc(val), cclutColumnNames = vc(val),
    cclutIsUnique = i4(val)) = null with protect
declare public::cclutCreateMockTable(cclutTableName = vc(val)) = null with protect
declare public::cclutRemoveMockTable(cclutTableName = vc(val)) = null with protect
declare public::cclutRemoveAllMockTables(null) = null with protect
declare public::cclutAddMockData(cclutTableName = vc(val), cclutRowData = vc(val)) = null with protect
declare public::cclutClearMockData(cclutTableName = vc(val)) = null with protect

declare public::cclutGetMockTableIndex(cclutTableName = vc(val)) = i4 with protect
declare public::cclutGetMockColumnIndex(cclutTableName = vc(val), cclutColumnName = vc(val)) = i4 with protect
declare public::cclutAssertMockDefinedForTable(cclutTableName = vc(val)) = i4 with protect
declare public::cclutAssertMockTableIsCreated(cclutTableName = vc(val)) = i4 with protect
declare public::cclutAssertMockTableIsNotCreated(cclutTableName = vc(val)) = i4 with protect
declare public::cclutAssertStringIsNonEmpty(cclutValue = vc(val),
    cclutValueName = vc(val)) = i4 with protect
declare public::cclutAssertNoInvalidEscapes(cclutValue = vc(val)) = i4 with protect
declare public::cclutAssertNoEscapedNulls(cclutValue = vc(val)) = i4 with protect
declare public::cclutNormalizeEscapes(cclutEscapedData = vc(val)) = vc with protect
declare public::cclutConstructDBSetStatement(cclutTableIndex = i4(val), cclutColumnIndex = i4(val),
    cclutDataBuffer = vc(val)) = vc with protect
declare public::cclutNextPipeValue(cclutPipeString = vc(ref)) = vc with protect

record cclut_mockTables (
    1 markerTime = i4
    1 tables[*]
        2 originalName = vc
        2 replaceName = vc
        2 columns[*]
            3 columnName = vc
            3 columnType = vc
        2 indexes[*]
            3 indexColumns = vc
            3 isUnique = i2
        2 isFinalized = i2
        2 isAccessedViaRdbCommands = i2
) with protect

set cclut_mockTables->markerTime = cnvtint(curtime3)

/**
Defines a mock table structure that can be created for use within a program.  This is the first function to be called in
the process of mocking a table.  It must be called before cclutAddMockIndex() or cclutCreateMockTable() can be called.
The table will not be mocked in cclutExecuteProgramWithMocks() unless finalized by calling cclutCreateMockTable().
tableName, columnNames, and columnTypes are required.  columnNames and columnTypes are expected to be pipe-delimited
strings.  The columnTypes should have the same count as columnNames and be in the same order.
isAccessedViaRdbCommands is optional and should be TRUE only if the script under test accesses the table using rdb commands.
Table mocking will fail if there are both rdb and non-rdb queries against the table.

@param tableName
    The table to be mocked.
@param columnNames
    A pipe-delimited list of columns to be mocked on the table
@param columnTypes
    A pipe-delimited list of types for each column
@param isAccessedViaRdbCommands
    A boolean flag indicating that the mocked table will be accessed using an rdb command
@returns
    The name of the mock table (This can be used to select data for testing)

Example:
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8")
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8", TRUE) ;person rdb-query exists
*/
subroutine public::cclutDefineMockTable(cclutTableName, cclutColumnNames, cclutColumnTypes, cclutIsAccessedViaRdbCommands)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare CCLUT_COLUMN_NAMES = vc with protect, constant(cnvtupper(cclutColumnNames))
    declare CCLUT_COLUMN_TYPES = vc with protect, constant(cnvtupper(cclutColumnTypes))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutColumnCount = i4 with protect, noconstant(0)
    declare cclutColumnIndex = i4 with protect, noconstant(0)
    declare cclutTypeCount = i4 with protect, noconstant(0)
    declare cclutColumnName = vc with protect, noconstant("")
    declare cclutStat = i4 with protect, noconstant(0)

    record cclutColumnRec(
        1 qual[*]
            2 str = vc
    ) with protect
    record cclutTypeRec(
        1 qual[*]
            2 str = vc
    ) with protect

    ;Validate inbound parameters
    if (cclutAssertStringIsNonEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return("")
    endif
    if (cclutAssertStringIsNonEmpty(CCLUT_COLUMN_NAMES, "columnNames") = FALSE)
        return("")
    endif
    if (cclutAssertStringIsNonEmpty(CCLUT_COLUMN_TYPES, "columnTypes") = FALSE)
        return("")
    endif

    ;Populate cclutColumnRec and cclutTypeRec and then validate that the column count matches the type count
    set cclutColumnCount =
        cclutDoArraySplit(cclutColumnRec, CCLUT_COLUMN_NAMES, CCLUT_PIPE)
    set cclutTypeCount =
        cclutDoArraySplit(cclutTypeRec, CCLUT_COLUMN_TYPES, CCLUT_PIPE)

    if (cclutColumnCount != cclutTypeCount)
        call cclut::addStashedError(392,
            concat("cclutDefineMockTable(", CCLUT_TABLE_NAME, ") - Size mismatch. ", trim(cnvtstring(cclutColumnCount, 25)),
                " columnNames vs. ", trim(cnvtstring(cclutTypeCount)), " columnTypes."), CURREF)
        return("")
    endif

    ;Validate that every column and type is populated and that there are no duplicate columns
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclutColumnName = cclutColumnRec->qual[cclutColumnIndex].str
        if (cclutIsEmpty(cclutColumnName))
            call cclut::addStashedError(392,
                concat("cclutDefineMockTable(",
                    CCLUT_TABLE_NAME, ") - Empty columnName found in columnNames. columnNames: ", CCLUT_COLUMN_NAMES, "."), CURREF)
            return("")
        endif
        if (cclutIsEmpty(cclutTypeRec->qual[cclutColumnIndex].str))
            call cclut::addStashedError(392,
                concat("cclutDefineMockTable(", CCLUT_TABLE_NAME,
                    ") - Empty columnType found in columnTypes. columnTypes: ", CCLUT_COLUMN_TYPES, "."), CURREF)
            return("")
        endif
        if (findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 0) !=
            findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 1))
                    call cclut::addStashedError(392,
                        concat("cclutDefineMockTable(", CCLUT_TABLE_NAME,
                            ") - Duplicate columnName found. columnName: ", cclutColumnName, "."), CURREF)
                    return("")
        endif
    endfor

    ;Remove table if it exists, and add a new element to the end of the array
    call cclutRemoveMockTable(CCLUT_TABLE_NAME)
    set cclutMockTablePos = size(cclut_mockTables->tables, 5) + 1
    set cclutStat = alterlist(cclut_mockTables->tables, cclutMockTablePos)

    ;Create the mock name and fill out the columns
    if (mod(cclutMockTableNumber, 10000) = 0)
        set cclutMockTableNumber = 1
    else
        set cclutMockTableNumber = cclutMockTableNumber + 1
    endif

    set cclut_mockTables->tables[cclutMockTablePos].originalName = CCLUT_TABLE_NAME
    set cclut_mockTables->tables[cclutMockTablePos].replaceName = cnvtupper(trim(substring(1, 30, concat("CUST_CCLUT_",
        trim(cnvtstring(cclutMockTableNumber, 25), 3), trim(cnvtstring(cclut_mockTables->markerTime, 25), 3), curuser)), 3))
    set cclut_mockTables->tables[cclutMockTablePos].isFinalized = FALSE

    set cclutStat = alterlist(cclut_mockTables->tables[cclutMockTablePos].columns, cclutColumnCount)
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclut_mockTables->tables[cclutMockTablePos].columns[cclutColumnIndex].columnName =
            cclutColumnRec->qual[cclutColumnIndex].str
        set cclut_mockTables->tables[cclutMockTablePos].columns[cclutColumnIndex].columnType =
            cclutTypeRec->qual[cclutColumnIndex].str
    endfor

    set cclut_mockTables->tables[cclutMockTablePos].isAccessedViaRdbCommands = cclutIsAccessedViaRdbCommands
    return(cclut_mockTables->tables[cclutMockTablePos].replaceName)
end ;cclutDefineMockTable

/**
Asserts that a specified string variable is non-empty, raising an error if it is.

@param cclutValue
    The variable to be checked.
@param cclutValueName
    The name for the variable to be displayed in the error if raised.
@return
    A boolean flag indicating whether the variable is non-empty, TRUE if and only if the variable is non-empty.
*/
subroutine public::cclutAssertStringIsNonEmpty(cclutValue, cclutValueName)
    if (cclutIsEmpty(cclutValue))
        call cclut::addStashedError(392,
            concat("cclutAssertStringIsNonEmpty() - ", cclutValueName, " is a required field."), CURREF)
        return(FALSE)
    endif
    return(TRUE)
end ;cclutAssertStringIsNonEmpty

/**
Asserts that a specified table name has a defined mock in cclut_mockTables, raising an error if not.

@param cclutTableName
    The name of the table.
@return
    The index in cclut_mockTables for the table's defined mock or 0 if there is none.
*/
subroutine public::cclutAssertMockDefinedForTable(cclutTableName)
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    set cclutMockTablePos = cclutGetMockTableIndex(cclutTableName)
    if (cclutMockTablePos < 1)
        call cclut::addStashedError(392,
            concat("cclutAssertMockDefinedForTable(", cclutTableName,") - Table has not been mocked."), CURREF)
        return(0)
    endif
    return(cclutMockTablePos)
end ;cclutAssertMockDefinedForTable

/**
Asserts that a mock for a specified table has been created in the RDBMS, raising an error otherwise.

@param cclutTableName
    The name of the table.
@return
    A boolean flag indicating that a mock for the table has been created.
*/
subroutine public::cclutAssertMockTableIsCreated(cclutTableName)
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    set cclutMockTablePos = cclutGetMockTableIndex(cclutTableName)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE)
            return(TRUE)
        endif
    endif
    call cclut::addStashedError(392,
        concat("cclutAssertMockTableIsCreated(", cclutTableName, ") - Table has not been created."), CURREF)
    return(FALSE)
end ;cclutAssertMockTableIsCreated

/**
Asserts that a mock for a specified table has not been created in the RDBMS, raising an error otherwise.

@param cclutTableName
    The name of the table.
@return
    A boolean flag indicating that a mock for the table has NOT been created.
*/
subroutine public::cclutAssertMockTableIsNotCreated(cclutTableName)
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    set cclutMockTablePos = cclutGetMockTableIndex(cclutTableName)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE)
            call cclut::addStashedError(392,
                concat("cclutAssertMockTableIsNotCreated(", cclutTableName, ") - Table has already been created."), CURREF)
            return(FALSE)
        endif
    endif
    return(TRUE)
end ;cclutAssertMockTableIsNotCreated

/**
Retrieves the index within cclut_mockTables->tables corresponding to a specified table name.

@param cclutTableName
    The name of the table.
@return
    The table's index within cclut_mockTables->tables or 0 if there is none.
*/
subroutine public::cclutGetMockTableIndex(cclutTableName)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)

    return(locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), cclutTableName,
        cclut_mockTables->tables[cclutMockTableIndex].originalName))
end ;cclutGetMockTableIndex

/**
Retrieves the index in cclut_mockTables->tables->columns corresponding to a specified table name and column name.
An error will occur if the table does not have a defined mock.

@param cclutTableName
    The table name.
@param cclutColumnName
    The column name.
@return
    The column's index within the table's list of defined columns or 0 if the column is not defined for the table.
*/
subroutine public::cclutGetMockColumnIndex(cclutTableName, cclutColumnName)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)

    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), cclutTableName,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    return(locateval(cclutMockColumnIndex, 1,
        size(cclut_mockTables->tables[cclutMockTablePos].columns, 5), cclutColumnName,
        cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnName))
end ;cclutGetMockColumnIndex

/**
Adds an index to a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error
will be thrown.  This function may not be called after cclutCreateMockTable().  tableName, columnNames, and isUnique are
required.  columnNames may be a single column name or a pipe-delimited list of columns for a composite index (the order
of the columns will be the order of the index).  If isUnique is TRUE, then a unique index will be created.  If isUnique
is FALSE, then a non-unique index will be created.

@param tableName
    The name of the source table for the mock table to which the index will be added.
@param columnNames
    A pipe-delimited string of column names for the index.
@param isUnique
    TRUE to create a unique index; FALSE to create a non-unique index

Example:
call cclutAddMockIndex("person", "person_id", TRUE)
call cclutAddMockIndex("person", "name_last|name_first", FALSE)
*/
subroutine public::cclutAddMockIndex(cclutTableName, cclutColumnNames, cclutIsUnique)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare CCLUT_COLUMN_NAMES = vc with protect, constant(cnvtupper(cclutColumnNames))
    declare CCLUT_IS_UNIQUE = i4 with protect, constant(cclutIsUnique)
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnPos = i4 with protect, noconstant(0)
    declare cclutMockIndexPos = i4 with protect, noconstant(0)
    declare cclutMockIndexIndex = i4 with protect, noconstant(0)
    declare cclutMockIndexLength = i4 with protect, noconstant(0)
    declare cclutColumnCount = i4 with protect, noconstant(0)
    declare cclutColumnIndex = i4 with protect, noconstant(0)
    declare cclutColumnName = vc with protect, noconstant("")
    declare cclutStat = i4 with protect, noconstant(0)

    record cclutColumnRec(
        1 qual[*]
            2 str = vc
    ) with protect

    ;Validate inbound parameters
    if (cclutAssertStringIsNonEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif
    if (cclutAssertStringIsNonEmpty(CCLUT_COLUMN_NAMES, "columnNames") = FALSE)
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = cclutAssertMockDefinedForTable(CCLUT_TABLE_NAME)
    if (cclutMockTablePos > 0 and cclutAssertMockTableIsNotCreated(CCLUT_TABLE_NAME))
        ;Validate that each column in the index is valid and that there are no duplicates
        set cclutColumnCount =
                cclutDoArraySplit(cclutColumnRec, CCLUT_COLUMN_NAMES, CCLUT_PIPE)
        for (cclutColumnIndex = 1 to cclutColumnCount)
            set cclutColumnName = cclutColumnRec->qual[cclutColumnIndex].str
            set cclutMockColumnPos = cclutGetMockColumnIndex(CCLUT_TABLE_NAME, cclutColumnName)
            if (cclutMockColumnPos < 1)
                call cclut::addStashedError(392,
                    concat("cclutAddMockIndex(", CCLUT_TABLE_NAME, ") - ", cclutColumnName, " is not mocked on table."), CURREF)
                return
            endif
            if (findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                    concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 0) !=
                findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                    concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 1))
                        call cclut::addStashedError(392,
                            concat("cclutAddMockIndex(", CCLUT_TABLE_NAME,
                                ") - Duplicate columnName found. columnName: ", cclutColumnName, "."), CURREF)
                        return
            endif
        endfor

        ;Set the index
        set cclutMockIndexLength = size(cclut_mockTables->tables[cclutMockTablePos].indexes, 5)
        set cclutMockIndexPos = locateval(cclutMockIndexIndex, 1, cclutMockIndexLength,
            CCLUT_COLUMN_NAMES, cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexIndex].indexColumns)
        if (cclutMockIndexPos < 1)
            set cclutMockIndexPos = cclutMockIndexLength + 1
            set cclutStat = alterlist(cclut_mockTables->tables[cclutMockTablePos].indexes, cclutMockIndexPos)
            set cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexPos].indexColumns = CCLUT_COLUMN_NAMES
        endif
        set cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexPos].isUnique =
            evaluate(CCLUT_IS_UNIQUE, FALSE, FALSE, TRUE)
    endif
end ;cclutAddMockIndex

/**
Creates a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be
thrown.  If the table has already been created, the function will return silently.  tableName is required.

@param tableName
    The name of the source table to be mocked.

Example:
call cclutCreateMockTable("person")
*/
subroutine public::cclutCreateMockTable(cclutTableName)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnCount = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)
    declare cclutMockIndexIndex = i4 with protect, noconstant(0)
    declare cclutParserText = vc with protect, noconstant("")
    declare cclutColumnName = vc with protect, noconstant("")

    ;Verify uesr is DBA or the session is authenticated
    if (cclutGetCurrentGroup(null) != 0 and reqinfo->updt_id = 0.0)
        call cclut::addStashedError(392, "The CCL session must be authenticated to execute this test", CURREF)
        go to exit_script
    endif

    ;Validate inbound parameter
    if (cclutAssertStringIsNonEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = cclutAssertMockDefinedForTable(CCLUT_TABLE_NAME)
    if (cclutMockTablePos < 1)
        return
    endif
    if (cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE)
        return
    endif

    ;Create the table
    set cclutParserText = concat(" select into table ", cclut_mockTables->tables[cclutMockTablePos].replaceName)
    for (cclutMockColumnIndex = 1 to size(cclut_mockTables->tables[cclutMockTablePos].columns, 5))
        if (cclutMockColumnIndex > 1)
            set cclutParserText = concat(cclutParserText, " ,")
        endif
        set cclutColumnName = cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnName
        set cclutParserText =
            concat(cclutParserText, ^ ^, cclutColumnName,
            ^ = type("^, cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnType, ^")^)
    endfor
    set cclutParserText = concat(cclutParserText, ^ with organization="P"^)
    for (cclutMockIndexIndex = 1 to size(cclut_mockTables->tables[cclutMockTablePos].indexes, 5))
        set cclutParserText = concat(cclutParserText,
            evaluate(cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexIndex].isUnique, TRUE,
            " , indexunique(", " , index("),
            replace(cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexIndex].indexColumns, CCLUT_PIPE,
                ","), ")")
    endfor

    set cclutParserText = concat(cclutParserText, " go")
    call parser(cclutParserText)

    set cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE
end ;cclutCreateMockTable

/**
Removes a mock table.  If the table was already created, it will also be dropped.  If the table is not currently
mocked, it will return silently.  tableName is required.

@param tableName
    The name of the source table that is mocked.

Example:
call cclutRemoveMockTable("person")
*/
subroutine public::cclutRemoveMockTable(cclutTableName)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare cclutMockTableCount = i4 with protect, noconstant(size(cclut_mockTables->tables, 5))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableName = vc with protect, noconstant("")
    declare cclutParserText = vc with protect, noconstant("")
    declare cclutStat = i4 with protect, noconstant(0)

    ;Validate inbound parameter
    if (cclutAssertStringIsNonEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return (null)
    endif

    set cclutMockTablePos = cclutGetMockTableIndex(CCLUT_TABLE_NAME)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTablePos].isFinalized)
            set cclutMockTableName = cclut_mockTables->tables[cclutMockTablePos].replaceName
            ;only DBA users can drop a CCL table definition
            ; so cclut_drop_mock_table_def is executed via tdbexecute to handle this for non-DBA users.
            if (cclutGetCurrentGroup(null) = 0)
                set cclutParserText = concat(" drop table ", cclutMockTableName, " go")
                call parser(cclutParserText)
            else
                declare dropSuccess = i2 with protect, noconstant(FALSE)
                free record cclut_drop_table_request
                record cclut_drop_table_request (
                  1 program_name = vc
                  1 output_device = vc
                  1 isBlob = i1
                  1 params = vc
                )
                set cclut_drop_table_request->program_name = "cclut_drop_mock_table_def"
                set cclut_drop_table_request->output_device = "MINE"
                set cclut_drop_table_request->isBlob = 1
                set cclut_drop_table_request->params = concat("^MINE^,^", cclutMockTableName, "^")
                set cclutStat =
                    tdbexecute(3202004, 3202004, 3050002, "REC", cclut_drop_table_request, "REC", cclut_drop_table_reply)
                if (cclutStat = 0)
                    if (cclut_drop_table_reply->status_data.status = "F")
                        call cclut::addStashedError(392, "VCCL_RUN_PROGRAM failure.", CURREF)
                    else
                        set cclutStat = cnvtjsontorec(cclut_drop_table_reply->document)
                        if(cclutDropMockTableReply->status = "S")
                            set dropSuccess = TRUE
                        else
                            call cclut::addStashedError(392, cclutDropMockTableReply->message, CURREF)
                        endif
                    endif
                else
                    call cclut::addStashedError(392, concat("TDBEXECUTE error (status = ", build(cclutStat), ")."), CURREF)
                endif
                if (dropSuccess = FALSE)
                    call cclut::addStashedError(392,
                        concat("cclutRemoveMockTable(", CCLUT_TABLE_NAME,
                            ") - Unable to drop table.  Manual cleanup may be required.  Mock table name is ",
                            cclutMockTableName, "."), CURREF)
                endif
            endif
            set cclutParserText = concat(" rdb drop table ", cclutMockTableName, " end go")
            call parser(" reset")
            call parser("execute null go")
            call parser(cclutParserText)
        endif
        set cclutStat = alterlist(cclut_mockTables->tables, cclutMockTableCount - 1, cclutMockTablePos - 1)
    endif
end ;cclutRemoveMockTable

/**
Removes all mock tables.  Any tables that have already been created will also be dropped.

Example:
call cclutRemoveAllMockTables(null)
*/
subroutine public::cclutRemoveAllMockTables(null)
    while (size(cclut_mockTables->tables, 5) > 0)
        call cclutRemoveMockTable(cclut_mockTables->tables[1].originalName)
    endwhile
end ;cclutRemoveAllMockTables

/**
Adds a row of mock data to a table.  tableName and rowData are required.  tableName must have already been created
through cclutCreateMockTable() or an error will be thrown.  rowData is a pipe-delimited string for each column in the
same order that was used in cclutDefineMockTable().  For character fields, the backslash (\) will serve as an escape
character.  For date fields, the value in rowData will be supplied to the cnvtdatetime() function.  All other values
will be passed as-is.

Supported escape values
\| = | (to represent a pipe in character fields)
\\ = \ (to represent a backslash in character fields)
\null = null (no value will be inserted into the column)

@param tableName
    The name of the source table for the mock table to which the data will be added.
@param rowData
    A pipe-delimited string of data to be inserted into the mock table.

Example:
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8")
call cclutCreateMockTable("person")
call cclutAddMockData("person", "1.0|Washington|George|01-JAN-1970 00:00") ;Will add George Washington
call cclutAddMockData("person", "2.0|A\|d\\ams|John|02-FEB-1971 11:11") ;Will add John A|d\ams
call cclutAddMockData("person", "3.0|Jefferson|\null|03-MAR-1972 22:22") ;Will add Jefferson (no first name)
call cclutAddMockData("person", "4.0|Madison||04-APR-1973 10:33") ;Will add Madison (empty string for first name)
*/
subroutine public::cclutAddMockData(cclutTableName, cclutRowData)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    ;Appending a single pipe on the end so that the string is empty once all values are popped.
    declare cclutMockValues = vc with protect, noconstant(concat(notrim(cclutRowData), CCLUT_PIPE))
    declare cclutMockValue = vc with protect, noconstant("")
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnLength = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)
    declare cclutNeedsAComma = i2 with protect, noconstant(FALSE)

    ;Validate inbound parameters
    if (cclutAssertStringIsNonEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    ;Validate that the table exists and has been created
    set cclutMockTablePos = cclutAssertMockDefinedForTable(CCLUT_TABLE_NAME)
    if (cclutMockTablePos < 1)
        return
    endif
    if (cclutAssertMockTableIsCreated(CCLUT_TABLE_NAME) = FALSE)
        return
    endif
    set cclutMockColumnLength = size(cclut_mockTables->tables[cclutMockTablePos].columns, 5)

    ;Validate that there are no unsupported escape sequences
    if (cclutAssertNoInvalidEscapes(cclutMockValues) = FALSE)
        return
    endif

    ;Initialize insert statement
    call parser(concat(" insert into ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " set"))

    while (textlen(cclutMockValues) > 0)
        set cclutMockColumnIndex = cclutMockColumnIndex + 1
        set cclutMockValue = notrim(cclutNextPipeValue(cclutMockValues))
        if (cclutMockValue != CCLUT_ESCAPED_NULL)
            if (cclutAssertNoEscapedNulls(cclutMockValue) = FALSE)
                return
            endif
            set cclutMockValue = notrim(cclutNormalizeEscapes(notrim(cclutMockValue)))
            set cclutMockValue = notrim(cclutConstructDBSetStatement(cclutMockTablePos, cclutMockColumnIndex,
                notrim(cclutMockValue)))
            if (cclutIsEmpty(cclutMockValue) = FALSE)
                call parser(concat(evaluate(cclutNeedsAComma, TRUE, ", ", " "), cclutMockValue))
                set cclutNeedsAComma = TRUE
            else
                ;Error occurred in cclutConstructDBSetStatement, return
                return
            endif
        endif
    endwhile

    if (cclutMockColumnIndex < cclutMockColumnLength)
        call cclut::addStashedError(392,
            concat("cclutAddMockData(", CCLUT_TABLE_NAME,") - Too few columns of data. rowData: ", cclutRowData, "."), CURREF)
        call parser(" reset")
        call parser("execute null go")
        return(null)
    endif

    ;No columns have been added meaning all are null
    if (cclutNeedsAComma = FALSE)
        call parser(" reset")
        call parser(concat(" rdb insert into ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " values ("))
        for (cclutMockColumnIndex = 1 to cclutMockColumnLength)
            call parser(evaluate(cclutMockColumnIndex, 1, "NULL", ",NULL"))
        endfor
        call parser(") end")
    endif

    call parser(" go")
end ;cclutAddMockData

/**
Asserts that a string value does not contain any invalid escape sequences, raising an error if it does. An escape sequence
is any substring beginning with a backslash ('\') and the only valid escape sequences are '\\', '\|' and '\null'.

@param cclutValue
    The string value to inspect.
@return
    A boolean flag indicating that the string does not contain any invalid escapes.
*/
subroutine public::cclutAssertNoInvalidEscapes(cclutValue)
    if (findstring(CCLUT_BACKSLASH, replace(replace(replace(cclutValue, CCLUT_ESCAPED_BACKSLASH, ""),
        CCLUT_ESCAPED_PIPE, ""), CCLUT_ESCAPED_NULL, "")) > 0)
            call cclut::addStashedError(392,
                concat("cclutAssertNoInvalidEscapes() - Invalid escape sequence. rowData: ", cclutValue, "."), CURREF)
            return(FALSE)
    endif
    return(TRUE)
end ;cclutAssertNoInvalidEscapes

/**
Asserts that a string value contains no escaped nulls, raising an error if it does. An escaped null is the sequence '\null' where
the preceding backslash is not escaped to represent an actual backslash character.

@param cclutValue
    The string value to inspect.
@return
    A boolean flag indicating that the string does not contain any escaped nulls.
*/
subroutine public::cclutAssertNoEscapedNulls(cclutValue)
    if (findstring(CCLUT_ESCAPED_NULL, replace(replace(cclutValue, CCLUT_ESCAPED_BACKSLASH, ""), CCLUT_ESCAPED_PIPE, "")) > 0)
        call cclut::addStashedError(392,
            concat("cclutValidateNotNulls() - \null column must have no other characters. rowData: ", cclutValue, "."), CURREF)
        call parser(" reset")
        call parser("execute null go")
        return(FALSE)
    endif
    return(TRUE)
end ;cclutAssertNoEscapedNulls

/**
Replaces escaped character sequences in a string value with their corresponding values.

@param cclutEscapedData
    The string value to normalize.
@return
    The result of replacing all escape sequences in cclutEscapedData with their corresponding values.
*/
subroutine public::cclutNormalizeEscapes(cclutEscapedData)
    return (replace(replace(cclutEscapedData, CCLUT_ESCAPED_PIPE, CCLUT_PIPE),
        CCLUT_ESCAPED_BACKSLASH, CCLUT_BACKSLASH))
end ;cclutNormalizeEscapes

/**
Pops the next value out from a pipe-delimited string.  The input string will be modified by removing
the first pipe-delimited value similar to popping from a stack.

@param cclutPipeString
    The pipe-delimited string
@return
    The first pipe-delimited value in cclutPipeString.
*/
subroutine public::cclutNextPipeValue(cclutPipeString)
    declare cclutPipePos = i4 with protect, noconstant(findString(CCLUT_PIPE, cclutPipeString))
    declare cclutPipeValue = vc with protect, noconstant("")
    while (cclutPipePos > 0)
        set cclutPipeValue = notrim(substring(1, cclutPipePos - 1, cclutPipeString))
        ;Check if the pipe is a delimiter pipe or an escaped pipe.  An escaped pipe will have an odd number of
        ;backslashes at the end (to account for situations like "\\" representing an escaped backslash).
        if(operator(cclutPipeValue,"regexplike","(^|[^\\])(\\\\)*$"))
            ;Delimiter pipe found, so update cclutPipeString to pop off the next value and return cclutPipeValue.
            set cclutPipeString =
                notrim(substring(cclutPipePos + 1, textlen(cclutPipeString) - cclutPipePos, cclutPipeString))
            return (cclutPipeValue)
        endif
        ;Escaped pipe, keep searching
        set cclutPipePos = findstring(CCLUT_PIPE, cclutPipeString, cclutPipePos + 1)
    endwhile
    ;Found no delimiter pipes, so the string is complete
    set cclutPipeValue = notrim(cclutPipeString)
    set cclutPipeString = trim("")
    return (cclutPipeValue)
end ;cclutNextPipeValue


/**
Constructs a statement for setting the value of a column in a database query on a mock table.

@param cclutTableIndex
    The index of the mock table within cclut_mockTables->tables.
@param cclutColumnIndex
    The index of the column within the table's columns from cclut_mockTables->tables->columns.
@param cclutDataBuffer
    The value to set on the column.
@return
    A string in the form of "<column> = <data>" with appropriate wrapping (cnvtdatetime, trim, etc.)
    depending on tne column's data type.
*/
subroutine public::cclutConstructDBSetStatement(cclutTableIndex, cclutColumnIndex, cclutDataBuffer)
    declare CCLUT_COLUMN_COUNT = i4 with protect, constant(size(cclut_mockTables->tables[cclutTableIndex].columns, 5))
    declare cclutMockColumnType = vc with protect, noconstant("")
    declare cclutMockColumnName = vc with protect, noconstant("")

    if (cclutColumnIndex > CCLUT_COLUMN_COUNT)
        call parser(" reset")
        call parser("execute null go")
        call cclut::addStashedError(392,
            concat("cclutConstructDBSetStatement(", cclut_mockTables->tables[cclutTableIndex].originalName,
                ") - Too many columns of data added to table."), CURREF)
        return("")
    endif

    set cclutMockColumnType = cclut_mockTables->tables[cclutTableIndex].columns[cclutColumnIndex].columnType
    set cclutMockColumnName = cclut_mockTables->tables[cclutTableIndex].columns[cclutColumnIndex].columnName

    ;Use the @ delimiter for string columns
    if (findstring("C", cclutMockColumnType) > 0)
        if (textlen(cclutDataBuffer) = 0)
            return(concat(cclutMockColumnName, ' = trim("")'))
        endif
        return(concat(cclutMockColumnName, " = @",
            trim(cnvtstring(textlen(cclutDataBuffer), 25), 3), ":", cclutDataBuffer, "@"))
    ;Use cnvtdatetime for date columns
    elseif (findstring("D", cclutMockColumnType) > 0)
        return(concat(cclutMockColumnName, ^ = cnvtdatetime("^, cclutDataBuffer, ^")^))
    ;Everything else is set directly
    else
        return(concat(cclutMockColumnName, " = ", cclutDataBuffer))
    endif
end ;cclutConstructDBSetStatement

/**
Clears all data from a specified mock table.  This is functionally similar to a truncate.  tableName is required.  The
mock table must have been created through cclutCreateMockTable() or else an error will be thrown.

@param tableName
    The name of the source table for the mock table to be cleared.

Example:
call cclutClearMockData("person")
*/
subroutine public::cclutClearMockData(cclutTableName)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (cclutAssertStringIsNonEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    ;Validate that the table exists and has been created
    set cclutMockTablePos = cclutAssertMockDefinedForTable(CCLUT_TABLE_NAME)
    if (cclutMockTablePos < 1)
        return
    endif
    if (cclutAssertMockTableIsCreated(CCLUT_TABLE_NAME) = FALSE)
        return
    endif

    call parser(concat(" delete from ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " where seq = 1 go"))
end ;cclutClearMockData

%#endif CCLUTMOCK_TABLE_INC
