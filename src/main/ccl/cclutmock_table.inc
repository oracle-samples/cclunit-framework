%#ifndef CCLUTMOCK_TABLE_INC
%#def CCLUTMOCK_TABLE_INC
;**********************************************************************************************************************************
;
; Provides support functions that can be leveraged by consumers to construct mock tables and add data to them to be used in the
; script under test.
;
;**********************************************************************************************************************************

declare CCLUT_BACKSLASH = vc with protect, constant("\")
declare CCLUT_ESCAPED_BACKSLASH = vc with protect, constant("\\")
declare CCLUT_PIPE = vc with protect, constant("|")
declare CCLUT_ESCAPED_PIPE = vc with protect, constant("\|")
declare CCLUT_ESCAPED_NULL = vc with protect, constant("\null")

declare cclutDefineMockTable(cclutTableName = vc(val), cclutFieldNames = vc(val),
    cclutFieldTypes = vc(val)) = vc with protect
declare cclutAddMockIndex(cclutTableName = vc(val), cclutColumnNames = vc(val),
    cclutIsUnique = i4(val)) = null with protect
declare cclutCreateMockTable(cclutTableName = vc(val)) = null with protect
declare cclutRemoveMockTable(cclutTableName = vc(val)) = null with protect
declare cclutRemoveAllMockTables(null) = null with protect
declare cclutAddMockData(cclutTableName = vc(val), cclutRowData = vc(val)) = null with protect
declare cclutClearMockData(cclutTableName = vc(val)) = null with protect

declare cclut::cclutGetMockTableIndex(cclutTableName = vc(val)) = i4 with protect
declare cclut::cclutGetMockColumnIndex(cclutTableName = vc(val), cclutColumnName = vc(val)) = i4 with protect
declare cclut::cclutValidateMockTableIsDefined(cclutTableName = vc(val)) = i4 with protect
declare cclut::cclutValidateMockTableIsCreated(cclutTableName = vc(val)) = i4 with protect
declare cclut::cclutValidateMockTableIsNotCreated(cclutTableName = vc(val)) = i4 with protect
declare cclut::cclutValidateParameterIsNotEmpty(cclutParameter = vc(val),
    cclutParameterName = vc(val)) = i4 with protect
declare cclut::cclutValidateEscapeSequences(cclutMockValues = vc(val)) = i4 with protect
declare cclut::cclutValidateNoNulls(cclutMockValue = vc(val)) = i4 with protect
declare cclut::cclutNormalizeEscapes(cclutEscapedData = vc(val)) = vc with protect
declare cclut::cclutConstructDBSetStatement(cclutTableIndex = i4(val), cclutColumnIndex = i4(val),
    cclutDataBuffer = vc(val)) = vc with protect
declare cclut::cclutNextPipeValue(cclutPipeString = vc(ref)) = vc with protect
declare cclut::cclutGetCurrentGroup(null) = i4 with protect

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
) with protect

set cclut_mockTables->markerTime = cnvtint(curtime3)

/**
Defines a mock table structure that can be created for use within a program.  This is the first function to be called in
the process of mocking a table.  It must be called before cclutAddMockIndex() or cclutCreateMockTable() can be called.
The table will not be mocked in cclutExecuteProgramWithMocks() unless finalized by calling cclutCreateMockTable().
tableName, columnNames, and columnTypes are required.  columnNames and columnTypes are expected to be pipe-delimited
strings.  The columnTypes should have the same count as columnNames and be in the same order.

@param tableName
    The table to be mocked.
@param columnNames
    A pipe-delimited list of columns to be mocked on the table
@param columnTypes
    A pipe-delimited list of types for each column
@returns
    The name of the mock table (This can be used to select data for testing)

Example:
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8")
**/
subroutine cclutDefineMockTable(cclutTableName, cclutColumnNames, cclutColumnTypes)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare CCLUT_COLUMN_NAMES = vc with protect, constant(cnvtupper(cclutColumnNames))
    declare CCLUT_COLUMN_TYPES = vc with protect, constant(cnvtupper(cclutColumnTypes))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutColumnCount = i4 with protect, noconstant(0)
    declare cclutColumnIndex = i4 with protect, noconstant(0)
    declare cclutTypeCount = i4 with protect, noconstant(0)
    declare cclutTypeIndex = i4 with protect, noconstant(0)
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
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return("")
    endif
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_COLUMN_NAMES, "columnNames") = FALSE)
        return("")
    endif
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_COLUMN_TYPES, "columnTypes") = FALSE)
        return("")
    endif

    ;Validate the column count matches the type count
    set cclutColumnCount =
        arraysplit(cclutColumnRec->qual[cclutColumnIndex].str, cclutColumnIndex, CCLUT_COLUMN_NAMES, CCLUT_PIPE)
    set cclutTypeCount =
        arraysplit(cclutTypeRec->qual[cclutTypeIndex].str, cclutTypeIndex, CCLUT_COLUMN_TYPES, CCLUT_PIPE)

    if (cclutColumnCount != cclutTypeCount)
        call cclexception(100, "E", concat("cclutDefineMockTable(", CCLUT_TABLE_NAME, ") - Size mismatch. ",
            trim(cnvtstring(cclutColumnCount, 25)), " columnNames vs. ", trim(cnvtstring(cclutTypeCount)), " columnTypes"))
        return("")
    endif

    ;Validate that every column and type is populated and that there are no duplicate columns
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclutColumnName = cclutColumnRec->qual[cclutColumnIndex].str
        if (cclut::cclutIsEmpty(cclutColumnName))
            call cclexception(100, "E", concat("cclutDefineMockTable(", CCLUT_TABLE_NAME,
                ") - Empty columnName found in columnNames. columnNames: ", CCLUT_COLUMN_NAMES))
            return("")
        endif
        if (cclut::cclutIsEmpty(cclutTypeRec->qual[cclutColumnIndex].str))
            call cclexception(100, "E", concat("cclutDefineMockTable(", CCLUT_TABLE_NAME,
                ") - Empty columnType found in columnTypes. columnTypes: ", CCLUT_COLUMN_TYPES))
            return("")
        endif
        if (findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 0) !=
            findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 1))
                    call cclexception(100, "E", concat("cclutDefineMockTable(", CCLUT_TABLE_NAME,
                        ") - Duplicate columnName found. columnName: ", cclutColumnName))
                    return("")
        endif
    endfor

    ;Remove table if it exists, and add a new element to the end of the array
    call cclutRemoveMockTable(CCLUT_TABLE_NAME)
    set cclutMockTablePos = size(cclut_mockTables->tables, 5) + 1
    set cclutStat = alterlist(cclut_mockTables->tables, cclutMockTablePos)

    ;Create the mock name and fill out the columns
    set cclut_mockTables->tables[cclutMockTablePos].originalName = CCLUT_TABLE_NAME
    set cclut_mockTables->tables[cclutMockTablePos].replaceName = cnvtupper(trim(substring(1, 30, concat("CUST_CCLUT_",
        trim(cnvtstring(cclutMockTablePos, 25), 3), trim(cnvtstring(cclut_mockTables->markerTime, 25), 3), curuser)), 3))
    set cclut_mockTables->tables[cclutMockTablePos].isFinalized = FALSE

    set cclutStat = alterlist(cclut_mockTables->tables[cclutMockTablePos].columns, cclutColumnCount)
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclut_mockTables->tables[cclutMockTablePos].columns[cclutColumnIndex].columnName =
            cclutColumnRec->qual[cclutColumnIndex].str
        set cclut_mockTables->tables[cclutMockTablePos].columns[cclutColumnIndex].columnType =
            cclutTypeRec->qual[cclutColumnIndex].str
    endfor

    return(cclut_mockTables->tables[cclutMockTablePos].replaceName)
end ;cclutDefineMockTable

/**
Helper function to validate if the supplied cclutParameter is not empty.  cclutParameter is the vc value to be checked.
cclutParameterName is the name of the parameter to be used in any error handling.  The subroutine returns TRUE if the
parameter is not empty or FALSE if it is empty.  The subroutine creates an exception if it returns FALSE.
**/
subroutine cclut::cclutValidateParameterIsNotEmpty(cclutParameter, cclutParameterName)
    if (cclut::cclutIsEmpty(cclutParameter))
        call cclexception(100, "E", concat("cclutValidateParameterIsNotEmpty() - ", cclutParameterName,
            " is a required field"))
        return(FALSE)
    endif
    return(TRUE)
end ;cclut::cclutValidateParameterIsNotEmpty

/**
Helper function to validate if the supplied cclutTableName has been defined as a mock in cclut_mockTables.
cclutTableName is the name of the table to validate.  The subroutine returns the index within the array if it is defined
or 0 if it is not defined.
**/
subroutine cclut::cclutValidateMockTableIsDefined(cclutTableName)
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    set cclutMockTablePos = cclut::cclutGetMockTableIndex(cclutTableName)
    if (cclutMockTablePos < 1)
        call cclexception(100, "E", concat("cclutValidateMockTableIsDefined(", cclutTableName,
            ") - Table has not been mocked."))
        return(0)
    endif
    return(cclutMockTablePos)
end ;cclut::cclutValidateMockTableIsDefined

/**
Helper function to validate if the supplied cclutTableName has been created as a mock in the RDBMS.  cclutTableName is
the name of the table to validate.  The subroutine returns TRUE if it has been created or FALSE if it has not been
created.  The subroutine creates an exception if it returns FALSE.
**/
subroutine cclut::cclutValidateMockTableIsCreated(cclutTableName)
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    set cclutMockTablePos = cclut::cclutGetMockTableIndex(cclutTableName)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE)
            return(TRUE)
        endif
    endif
    call cclexception(100, "E", concat("cclutValidateMockTableIsCreated(", cclutTableName,
        ") - Table has not been created."))
    return(FALSE)
end ;cclut::cclutValidateMockTableIsCreated

/**
Helper function to validate if the supplied cclutTableName has not been created as a mock in the RDBMS.  cclutTableName
is the name of the table to validate.  The subroutine returns TRUE if it has been not been created or FALSE if it has
been created.  The subroutine creates an exception if it returns FALSE.
**/
subroutine cclut::cclutValidateMockTableIsNotCreated(cclutTableName)
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    set cclutMockTablePos = cclut::cclutGetMockTableIndex(cclutTableName)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE)
            call cclexception(100, "E", concat("cclutValidateMockTableIsNotCreated(", cclutTableName,
                ") - Table has already been created."))
            return(FALSE)
        endif
    endif
    return(TRUE)
end ;cclut::cclutValidateMockTableIsNotCreated

/**
Helper function to retrieve the index of the supplied cclutTableName within the cclut_mockTables->tables array.
cclutTableName is the name of the table for which to search.  The subroutine returns the index as an integer if it is
found or 0 if it is not found.
**/
subroutine cclut::cclutGetMockTableIndex(cclutTableName)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)

    return(locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), cclutTableName,
        cclut_mockTables->tables[cclutMockTableIndex].originalName))
end ;cclut::cclutGetMockTableIndex

/**
Helper function to retrieve the index of the supplied cclutColumnName for the supplied cclutTableName within the
cclut_mockTables record structure.  cclutTableName is the name of the table for which to search.  cclutColumnName is the
name of the column for which to search.  The subroutine returns the index as an integer if it is found or 0 if it is not
found.
**/
subroutine cclut::cclutGetMockColumnIndex(cclutTableName, cclutColumnName)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)

    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), cclutTableName,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    return(locateval(cclutMockColumnIndex, 1,
        size(cclut_mockTables->tables[cclutMockTablePos].columns, 5), cclutColumnName,
        cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnName))
end ;cclut::cclutGetMockColumnIndex

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
**/
subroutine cclutAddMockIndex(cclutTableName, cclutColumnNames, cclutIsUnique)
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
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_COLUMN_NAMES, "columnNames") = FALSE)
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = cclut::cclutValidateMockTableIsDefined(CCLUT_TABLE_NAME)
    if (cclutMockTablePos > 0 and cclut::cclutValidateMockTableIsNotCreated(CCLUT_TABLE_NAME))
        ;Validate that each column in the index is valid and that there are no duplicates
        set cclutColumnCount =
                arraysplit(cclutColumnRec->qual[cclutColumnIndex].str, cclutColumnIndex, CCLUT_COLUMN_NAMES, CCLUT_PIPE)
        for (cclutColumnIndex = 1 to cclutColumnCount)
            set cclutColumnName = cclutColumnRec->qual[cclutColumnIndex].str
            set cclutMockColumnPos = cclut::cclutGetMockColumnIndex(CCLUT_TABLE_NAME, cclutColumnName)
            if (cclutMockColumnPos < 1)
                call cclexception(100, "E", concat("cclutAddMockIndex(", CCLUT_TABLE_NAME,
                    ") - ", cclutColumnName, " is not mocked on table."))
                return
            endif
            if (findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                    concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 0) !=
                findstring(concat(CCLUT_PIPE, cclutColumnName, CCLUT_PIPE),
                    concat(CCLUT_PIPE, CCLUT_COLUMN_NAMES, CCLUT_PIPE), 1, 1))
                        call cclexception(100, "E", concat("cclutAddMockIndex(", CCLUT_TABLE_NAME,
                            ") - Duplicate columnName found. columnName: ", cclutColumnName))
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
**/
subroutine cclutCreateMockTable(cclutTableName)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnCount = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)
    declare cclutMockIndexIndex = i4 with protect, noconstant(0)
    declare cclutParserText = vc with protect, noconstant("")
    declare cclutColumnName = vc with protect, noconstant("")

    ;Validate inbound parameter
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = cclut::cclutValidateMockTableIsDefined(CCLUT_TABLE_NAME)
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
Helper function to return the current group for the user.  This is wrapped in a helper function so that it can be easily
mocked.
**/
subroutine cclut::cclutGetCurrentGroup(null)
    return(curgroup)
end ;cclut::cclutGetCurrentGroup

/**
Removes a mock table.  If the table was already created, it will also be dropped.  If the table is not currently
mocked, it will return silently.  tableName is required.

@param tableName
    The name of the source table that is mocked.

Example:
call cclutRemoveMockTable("person")
**/
subroutine cclutRemoveMockTable(cclutTableName)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare cclutMockTableCount = i4 with protect, noconstant(size(cclut_mockTables->tables, 5))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableName = vc with protect, noconstant("")
    declare cclutParserText = vc with protect, noconstant("")
    declare cclutStat = i4 with protect, noconstant(0)

    ;Validate inbound parameter
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    set cclutMockTablePos = cclut::cclutGetMockTableIndex(CCLUT_TABLE_NAME)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTablePos].isFinalized)
            set cclutMockTableName = cclut_mockTables->tables[cclutMockTablePos].replaceName
            ;Drop the table if it was already added
            ;For group1 users, they are unable to drop Discern definitions for tables (even ones they created).  A
            ;prompt program will execute to perform the drop for them.  RDB drops have no such restrictions, so that can
            ;be done directly
            if (cclut::cclutGetCurrentGroup(null) = 0)
                set cclutParserText = concat(" drop table ", cclutMockTableName, " go")
                call parser(cclutParserText)
            else
                free record cclut_drop_table_request
                record cclut_drop_table_request (
                  1 program_name = vc
                  1 query_command = vc
                  1 output_device = vc
                  1 Is_printer = c1
                  1 Is_Odbc = c1
                  1 IsBlob = c1
                  1 params = vc
                  1 qual [*]
                    2 parameter = vc
                    2 data_type = c1
                  1 blob_in = gvc
                )
                set cclut_drop_table_request->program_name = "cclut_drop_table_definition"
                set cclut_drop_table_request->params = concat("^MINE^,^", cclutMockTableName, "^")
                set stat = tdbexecute(3070000, 3070001, 3050002, "REC", cclut_drop_table_request, "REC",
                    cclut_drop_table_reply)
                if (stat = 1 or cclut_drop_table_reply->status_data.status = "F")
                    call cclexception(100, "E", concat("cclutRemoveMockTable(", CCLUT_TABLE_NAME,
                        ") - Unable to drop table.  Manual cleanup may be required.  Mock table name is ",
                        cclutMockTableName))
                endif
            endif
            set cclutParserText =
                concat(" rdb drop table ", cclutMockTableName, " end go")
            call parser(cclutParserText)
        endif
        ;Remove it from the mocked list
        set cclutStat = alterlist(cclut_mockTables->tables, cclutMockTableCount - 1, cclutMockTablePos - 1)
    endif
end ;cclutRemoveMockTable

/**
Removes all mock tables.  Any tables that have already been created will also be dropped.

Example:
call cclutRemoveAllMockTables(null)
**/
subroutine cclutRemoveAllMockTables(null)
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
**/
subroutine cclutAddMockData(cclutTableName, cclutRowData)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    ;Appending a single pipe on the end so that the string is empty once all values are popped.
    declare cclutMockValues = vc with protect, noconstant(concat(notrim(cclutRowData), CCLUT_PIPE))
    declare cclutMockValue = vc with protect, noconstant("")
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockColumnLength = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)
    declare cclutNeedsAComma = i2 with protect, noconstant(FALSE)

    ;Validate inbound parameters
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    ;Validate that the table exists and has been created
    set cclutMockTablePos = cclut::cclutValidateMockTableIsDefined(CCLUT_TABLE_NAME)
    if (cclutMockTablePos < 1)
        return
    endif
    if (cclut::cclutValidateMockTableIsCreated(CCLUT_TABLE_NAME) = FALSE)
        return
    endif
    set cclutMockColumnLength = size(cclut_mockTables->tables[cclutMockTablePos].columns, 5)

    ;Validate that there are no unsupported escape sequences
    if (cclut::cclutValidateEscapeSequences(cclutMockValues) = FALSE)
        return
    endif

    ;Initialize insert statement
    call parser(concat(" insert into ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " set"))

    while (textlen(cclutMockValues) > 0)
        set cclutMockColumnIndex = cclutMockColumnIndex + 1
        set cclutMockValue = notrim(cclut::cclutNextPipeValue(cclutMockValues))
        if (cclutMockValue != CCLUT_ESCAPED_NULL)
            if (cclut::cclutValidateNoNulls(cclutMockValue) = FALSE)
                return
            endif
            set cclutMockValue = notrim(cclut::cclutNormalizeEscapes(notrim(cclutMockValue)))
            set cclutMockValue = notrim(cclut::cclutConstructDBSetStatement(cclutMockTablePos, cclutMockColumnIndex,
                notrim(cclutMockValue)))
            if (cclut::cclutIsEmpty(cclutMockValue) = FALSE)
                call parser(concat(evaluate(cclutNeedsAComma, TRUE, ", ", " "), cclutMockValue))
                set cclutNeedsAComma = TRUE
            else
                ;Error occurred in cclutConstructDBSetStatement, return
                return
            endif
        endif
    endwhile

    if (cclutMockColumnIndex < cclutMockColumnLength)
        call cclexception(100, "E", concat("cclutAddMockData(", CCLUT_TABLE_NAME,
            ") - Too few columns of data. rowData: ", cclutRowData))
        call parser(" reset")
        call parser(" generate null go")
        return
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
Helper function to replace escaped character sequences with their unescaped value.  This function expects
cclutEscapedData to not have any pipes besides escaped ones (i.e. cclutEscapedData should represent a single value to be
added to a single column for cclutAddMockData).  The subroutine returns TRUE if all escape sequences are valid or FALSE
if there are any invalid escape sequences.  The subroutine creates an exception if it returns FALSE.
**/
subroutine cclut::cclutValidateEscapeSequences(cclutMockValues)
    if (findstring(CCLUT_BACKSLASH, replace(replace(replace(cclutMockValues, CCLUT_ESCAPED_BACKSLASH, ""),
        CCLUT_ESCAPED_PIPE, ""), CCLUT_ESCAPED_NULL, "")) > 0)
            call cclexception(100, "E",
                concat("cclutValidateEscapeSequences() - Invalid escape sequence. rowData: ", cclutMockValues))
            return(FALSE)
    endif
    return(TRUE)
end ;cclut::cclutValidateEscapeSequences

/**
Helper function to validate that there are no escaped nulls in a mocked value.  Escaped nulls should be the only
characters in a given column as it indicates that the column should be null.  The subroutine returns TRUE if there are
no escaped nulls or FALSE if there are any escaped nulls.  The subroutine creates an exception if it returns FALSE.
**/
subroutine cclut::cclutValidateNoNulls(cclutMockValue)
    if (findstring(CCLUT_ESCAPED_NULL, replace(replace(cclutMockValue, CCLUT_ESCAPED_BACKSLASH, ""),
        CCLUT_ESCAPED_PIPE, "")) > 0)
            call cclexception(100, "E",
                concat("cclutValidateNotNulls() - \null column must have no other characters. rowData: ",
                    cclutMockValue))
            call parser(" reset")
            call parser(" generate null go")
            return(FALSE)
    endif
    return(TRUE)
end ;cclut::cclutValidateNoNulls

/**
Helper function to replace escaped character sequences with their unescaped value.  This function expects
cclutEscapedData to not have any pipes besides escaped ones (i.e. cclutEscapedData should represent a single value to be
added to a single column for cclutAddMockData).
**/
subroutine cclut::cclutNormalizeEscapes(cclutEscapedData)
    return (replace(replace(cclutEscapedData, CCLUT_ESCAPED_PIPE, CCLUT_PIPE),
        CCLUT_ESCAPED_BACKSLASH, CCLUT_BACKSLASH))
end ;cclut::cclutNormalizeEscapes

/**
Helper function to pop the next value out of a piped string for the cclutAddMockData function.  The cclutPipeString
value is passed by reference and will be modified to remove the next value similar to popping an array.  The returned
value will be the value that was popped.
**/
subroutine cclut::cclutNextPipeValue(cclutPipeString)
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
end ;cclut::cclutNextPipeValue


/**
Helper function to construct a column for a mock insertion.  cclutTableIndex is the index of the table within the
cclut_mockTables->tables array.  cclutColumnIndex is the index of the column within the
cclut_mockTables->tables->columns array.  cclutDataBuffer is the data that will be inserted into the column.  The
subroutine will return a string in the form of "<column> = <data>" with appropriate wrapping of data for certain data
types.
**/
subroutine cclut::cclutConstructDBSetStatement(cclutTableIndex, cclutColumnIndex, cclutDataBuffer)
    declare CCLUT_COLUMN_COUNT = i4 with protect, constant(size(cclut_mockTables->tables[cclutTableIndex].columns, 5))
    declare cclutMockColumnType = vc with protect, noconstant("")
    declare cclutMockColumnName = vc with protect, noconstant("")

    if (cclutColumnIndex > CCLUT_COLUMN_COUNT)
        call cclexception(100, "E", concat("cclutConstructDBSetStatement(",
            cclut_mockTables->tables[cclutTableIndex].originalName, ") - Too many columns of data added to table."))
        call parser(" reset")
        call parser(" generate null go")
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
end ;cclut::cclutConstructDBSetStatement

/**
Clears all data from a specified mock table.  This is functionally similar to a truncate.  tableName is required.  The
mock table must have been created through cclutCreateMockTable() or else an error will be thrown.

@param tableName
    The name of the source table for the mock table to be cleared.

Example:
call cclutClearMockData("person")
**/
subroutine cclutClearMockData(cclutTableName)
    declare CCLUT_TABLE_NAME = vc with protect, constant(cnvtupper(cclutTableName))
    declare cclutMockTablePos = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (cclut::cclutValidateParameterIsNotEmpty(CCLUT_TABLE_NAME, "tableName") = FALSE)
        return
    endif

    ;Validate that the table exists and has been created
    set cclutMockTablePos = cclut::cclutValidateMockTableIsDefined(CCLUT_TABLE_NAME)
    if (cclutMockTablePos < 1)
        return
    endif
    if (cclut::cclutValidateMockTableIsCreated(CCLUT_TABLE_NAME) = FALSE)
        return
    endif

    call parser(concat(" delete from ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " where seq = 1 go"))
end ;cclutClearMockData

%#endif CCLUTMOCK_TABLE_INC
