%#ifndef CCLUTMOCK_TABLE_INC
%#def CCLUTMOCK_TABLE_INC
;**********************************************************************************************************************************
;
; Provides support functions that can be leveraged by consumers to construct mock tables and add data to them to be used in the
; script under test.
;
;**********************************************************************************************************************************

declare cclutDefineMockTable(tableName = vc(val), fieldNames = vc(val), fieldTypes = vc(val)) = vc with protect
declare cclutAddMockConstraint(tableName = vc(val), columnName = vc(val),
    columnConstraint = vc(val)) = null with protect
declare cclutAddMockIndex(tableName = vc(val), columnNames = vc(val), isUnique = i4(val)) = null with protect
declare cclutCreateMockTable(tableName = vc(val)) = null with protect
declare cclutRemoveMockTable(tableName = vc(val)) = null with protect
declare cclutRemoveAllMockTables(null) = null with protect
declare cclutAddMockData(tableName = vc(val), rowData = vc(val)) = null with protect
declare cclutClearMockData(tableName = vc(val)) = null with protect

declare cclut::cclutParseColumn(tableIndex = i4(val), columnIndex = i4(val), dataBuffer = vc(val)) = vc with protect

record cclut_mockTables (
    1 startTime = i4
    1 tables[*]
		2 originalName = vc
		2 replaceName = vc
		2 columns[*]
		    3 columnName = vc
		    3 columnType = vc
		    3 columnConstraint = vc
		2 indexes[*]
		    3 indexColumns = vc
		    3 isUnique = i4
		2 isFinalized = i4
) with protect

set cclut_mockTables->startTime = cnvtint(curtime3)

/**
Defines a mock table structure that can be created for use within a program.  This is the first function to be called in
the process of mocking a table.  It must be called before cclutAddMockIndex(), cclutAddMockConstraint(), and
cclutCreateMockTable() can be called.  The table will not be mocked in cclutExecuteProgramWithMocks() unless
cclutCreateMockTable() is called.  tableName, columnNames, and columnTypes are required.  columnNames and columnTypes
are expected to be pipe-delimited strings.  The columnTypes should have the same count as columnNames and be in the same
order.

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
subroutine cclutDefineMockTable(tableName, columnNames, columnTypes)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare CCLUT_COLUMN_NAMES = vc with protect, constant(trim(cnvtupper(columnNames), 3))
    declare CCLUT_COLUMN_TYPES = vc with protect, constant(trim(cnvtupper(columnTypes), 3))
    declare cclutMockTableLength = i4 with protect, noconstant(size(cclut_mockTables->tables, 5))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutColumnCount = i4 with protect, noconstant(0)
    declare cclutColumnIndex = i4 with protect, noconstant(0)
    declare cclutTypeCount = i4 with protect, noconstant(0)
    declare cclutTypeIndex = i4 with protect, noconstant(0)
    declare cclutColumnName = vc with protect, noconstant("")
    record cclutColumnRec(
        1 qual[*]
            2 str = vc
    ) with protect
    record cclutTypeRec(
        1 qual[*]
            2 str = vc
    ) with protect

    ;Validate inbound parameters
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutDefineMockTable() - tableName is a required field")
        return("")
    endif
    if (CCLUT_COLUMN_NAMES <= " ")
        call cclexception(100, "E", "cclutDefineMockTable() - columnNames is a required field")
        return("")
    endif
    if (CCLUT_COLUMN_TYPES <= " ")
        call cclexception(100, "E", "cclutDefineMockTable() - columnTypes is a required field")
        return("")
    endif

    ;Validate the column count matches the type count
    set cclutColumnCount =
        arraysplit(cclutColumnRec->qual[cclutColumnIndex].str, cclutColumnIndex, CCLUT_COLUMN_NAMES, "|")
    set cclutTypeCount = arraysplit(cclutTypeRec->qual[cclutTypeIndex].str, cclutTypeIndex, CCLUT_COLUMN_TYPES, "|")

    if (cclutColumnCount != cclutTypeCount)
        call cclexception(100, "E", concat("cclutDefineMockTable() - columnTypes must match number of columnNames. ",
            "columnNames Count: ", cnvtstring(cclutColumnCount), " columnTypes Count: ", cnvtstring(cclutTypeCount)))
        return("")
    endif

    ;Validate that every column and type is populated and that there are no duplicate columns
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclutColumnName = cclutColumnRec->qual[cclutColumnIndex].str
        if (cclutColumnName <= " ")
            call cclexception(100, "E", concat("cclutDefineMockTable() - Empty columnName found in columnNames. ",
                "columnNames: ", CCLUT_COLUMN_NAMES))
            return("")
        endif
        if (cclutTypeRec->qual[cclutColumnIndex].str <= " ")
            call cclexception(100, "E", concat("cclutDefineMockTable() - Empty columnType found in columnTypes. ",
                "columnTypes: ", CCLUT_COLUMN_TYPES))
            return("")
        endif
        if (findstring(cclutColumnName, CCLUT_COLUMN_NAMES, 1, 0) !=
            findstring(cclutColumnName, CCLUT_COLUMN_NAMES, 1, 1))
                call cclexception(100, "E", concat("cclutDefineMockTable() - Duplicate columnName found. ",
                    "columnName: ", cclutColumnName))
                return("")
        endif
    endfor

    ;Check if table is already mocked
    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, cclutMockTableLength, CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)

    if (cclutMockTablePos > 0)
        ;If table was already defined, remove it and re-add it
        call cclutRemoveMockTable(CCLUT_TABLE_NAME)
        set stat = alterlist(cclut_mockTables->tables, cclutMockTableLength, cclutMockTablePos - 1)
    else
        ;If table is new, add a new element to the end of the array
        set cclutMockTablePos = cclutMockTableLength + 1
        set stat = alterlist(cclut_mockTables->tables, cclutMockTablePos)
    endif

    ;Create the mock name and fill out the columns
    set cclut_mockTables->tables[cclutMockTablePos].originalName = CCLUT_TABLE_NAME
    set cclut_mockTables->tables[cclutMockTablePos].replaceName = cnvtupper(trim(substring(1, 30, concat("CUST_CCLUT_",
        trim(cnvtstring(cclutMockTablePos), 3), trim(cnvtstring(cclut_mockTables->startTime), 3), curuser)), 3))
    set cclut_mockTables->tables[cclutMockTablePos].isFinalized = FALSE

    set stat = alterlist(cclut_mockTables->tables[cclutMockTablePos].columns, cclutColumnCount)
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclut_mockTables->tables[cclutMockTablePos].columns[cclutColumnIndex].columnName =
            cclutColumnRec->qual[cclutColumnIndex].str
        set cclut_mockTables->tables[cclutMockTablePos].columns[cclutColumnIndex].columnType =
            cclutTypeRec->qual[cclutColumnIndex].str
    endfor

    return(cclut_mockTables->tables[cclutMockTablePos].replaceName)
end ;cclutDefineMockTable

/**
Adds a constraint to a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error
will be thrown.  This function may not be called after cclutCreateMockTable().  tableName and columnName are required.
If the columnName is not valid for the table specified, an error will be thrown.  All constraints for a column should be
present in the columnConstraint field.  If a constraint already exists, this function will overwrite it with the new
value.  If columnConstraint is blank, the constraint will be removed for the column.  The supported constraints can be
seen here:
https://wiki.cerner.com/display/public/1101discernHP/SELECT+INTO+TABLE+Table_Name+Using+Discern+Explorer

@param tableName
    The table to which the constraint will be added.
@param columnName
    The column to which the constraint will be applied.
@param columnConstraint
    A string of all constraints to be applied to the column.

Example:
call cclutAddMockConstraint("person", "name_last", "not null unique")
**/
subroutine cclutAddMockConstraint(tableName, columnName, columnConstraint)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare CCLUT_COLUMN_NAME = vc with protect, constant(trim(cnvtupper(columnName), 3))
    declare CCLUT_COLUMN_CONSTRAINT = vc with protect, constant(trim(cnvtupper(columnConstraint), 3))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutMockColumnPos = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutAddMockConstraint() - tableName is a required field")
        return
    endif
    if (CCLUT_COLUMN_NAME <= " ")
        call cclexception(100, "E", "cclutAddMockConstraint() - columnName is a required field")
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    if (cclutMockTablePos < 1)
        call cclexception(100, "E", concat("cclutAddMockConstraint() - tableName has not been mocked. tableName: ",
            CCLUT_TABLE_NAME))
        return
    endif
    if (cclut_mockTables->tables[cclutMockTableIndex].isFinalized = TRUE)
        call cclexception(100, "E", concat("cclutAddMockConstraint() - tableName has already been created. ",
            "Constraints cannot be modified. tableName: ", CCLUT_TABLE_NAME))
        return
    endif

    ;Validate that the column exists on the table.
    set cclutMockColumnPos =
        locateval(cclutMockColumnIndex, 1, size(cclut_mockTables->tables[cclutMockTablePos].columns, 5),
        CCLUT_COLUMN_NAME, cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnName)
    if (cclutMockColumnPos < 1)
        call cclexception(100, "E", concat("cclutAddMockConstraint() - columnName is not mocked on table. ",
            "columnName: ", CCLUT_COLUMN_NAME, " tableName: ", CCLUT_TABLE_NAME))
    endif

    ;Set the constraint
    set cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnPos].columnConstraint =
        CCLUT_COLUMN_CONSTRAINT
end ;cclutAddMockConstraint

/**
Adds an index to a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error
will be thrown.  This function may not be called after cclutCreateMockTable().  tableName, columnNames, and isUnique are
required.  columnNames may be a single column name or a pipe-delimited list of columns for a composite index (the order
of the columns will be the order of the index).  If isUnique is 1, then a unique index will be created.  If isUnique is
0, then a non-unique index will be created.

@param tableName
    The table to which the index will be added.
@param columnNames
    A pipe-delimited string of column names for the index.
@param isUnique
    1 to create a unique index; 0 to create a non-unique index

Example:
call cclutAddMockIndex("person", "person_id", 1)
call cclutAddMockIndex("person", "name_last|name_first", 0)
**/
subroutine cclutAddMockIndex(tableName, columnNames, isUnique)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare CCLUT_COLUMN_NAMES = vc with protect, constant(trim(cnvtupper(columnNames), 3))
    declare CCLUT_IS_UNIQUE = i4 with protect, constant(isUnique)
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutMockColumnPos = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)
    declare cclutMockIndexPos = i4 with protect, noconstant(0)
    declare cclutMockIndexIndex = i4 with protect, noconstant(0)
    declare cclutMockIndexLength = i4 with protect, noconstant(0)
    declare cclutColumnCount = i4 with protect, noconstant(0)
    declare cclutColumnIndex = i4 with protect, noconstant(0)
    declare cclutColumnName = vc with protect, noconstant("")
    record cclutColumnRec(
        1 qual[*]
            2 str = vc
    ) with protect

    ;Validate inbound parameters
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutAddMockIndex() - tableName is a required field")
        return
    endif
    if (CCLUT_COLUMN_NAMES <= " ")
        call cclexception(100, "E", "cclutAddMockIndex() - columnNames is a required field")
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    if (cclutMockTablePos < 1)
        call cclexception(100, "E", concat("cclutAddMockIndex() - tableName has not been mocked. tableName: ",
            CCLUT_TABLE_NAME))
        return
    endif
    if (cclut_mockTables->tables[cclutMockTableIndex].isFinalized = TRUE)
        call cclexception(100, "E", concat("cclutAddMockIndex() - tableName has already been created. ",
            "Indexes cannot be modified. tableName: ", CCLUT_TABLE_NAME))
        return
    endif

    ;Validate that each column in the index is valid and that there are no duplicates
    set cclutColumnCount =
            arraysplit(cclutColumnRec->qual[cclutColumnIndex].str, cclutColumnIndex, CCLUT_COLUMN_NAMES, "|")
    for (cclutColumnIndex = 1 to cclutColumnCount)
        set cclutColumnName = cclutColumnRec->qual[cclutColumnIndex].str
        set cclutMockColumnPos = locateval(cclutMockColumnIndex, 1,
            size(cclut_mockTables->tables[cclutMockTablePos].columns, 5), cclutColumnName,
            cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnName)
        if (cclutMockColumnPos < 1)
            call cclexception(100, "E", concat("cclutAddMockIndex() - columnName is not mocked on table. columnName: ",
                cclutColumnName, " tableName: ", CCLUT_TABLE_NAME))
            return
        endif
        if (findstring(cclutColumnName, CCLUT_COLUMN_NAMES, 1, 0) !=
            findstring(cclutColumnName, CCLUT_COLUMN_NAMES, 1, 1))
                call cclexception(100, "E", concat("cclutAddMockIndex() - Duplicate columnName found. ",
                    "columnName: ", cclutColumnName))
                return
        endif
    endfor

    ;Set the index
    set cclutMockIndexLength = size(cclut_mockTables->tables[cclutMockTablePos].indexes, 5)
    set cclutMockIndexPos = locateval(cclutMockIndexIndex, 1, cclutMockIndexLength,
        CCLUT_COLUMN_NAMES, cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexIndex].indexColumns)
    if (cclutMockIndexPos < 1)
        set cclutMockIndexPos = cclutMockIndexLength + 1
        set stat = alterlist(cclut_mockTables->tables[cclutMockTablePos].indexes, cclutMockIndexPos)
        set cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexPos].indexColumns = CCLUT_COLUMN_NAMES
    endif
    set cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexPos].isUnique =
        evaluate(CCLUT_IS_UNIQUE, 0, 0, 1)
end ;cclutAddMockIndex

/**
Creates the mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be
thrown.  If the table has already been created, the function will return silently.  tableName is required.

@param tableName
    The table to be mocked.

Example:
call cclutCreateMockTable("person")
**/
subroutine cclutCreateMockTable(tableName)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutMockColumnCount = i4 with protect, noconstant(0)
    declare cclutMockColumnIndex = i4 with protect, noconstant(0)
    declare cclutMockIndexIndex = i4 with protect, noconstant(0)
    declare cclutParserText = vc with protect, noconstant("")
    declare cclutColumnName = vc with protect, noconstant("")
    declare cclutConstraint = vc with protect, noconstant("")
    declare cclutParserConstraint = vc with protect, noconstant(" ")

    ;Validate inbound parameter
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutCreateMockTable() - tableName is a required field")
        return
    endif

    ;Validate that the table exists and has not been created
    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    if (cclutMockTablePos < 1)
        call cclexception(100, "E", concat("cclutCreateMockTable() - tableName has not been mocked. tableName: ",
            CCLUT_TABLE_NAME))
        return
    endif
    if (cclut_mockTables->tables[cclutMockTableIndex].isFinalized = TRUE)
        return
    endif

    ;Create the table
    set cclutParserText = concat(" select into table ", cclut_mockTables->tables[cclutMockTablePos].replaceName)
    for (cclutMockColumnIndex = 1 to size(cclut_mockTables->tables[cclutMockTablePos].columns, 5))
        if (cclutMockColumnIndex > 1)
            set cclutParserText = concat(cclutParserText, " , ")
        endif
        set cclutColumnName = cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnName
        set cclutConstraint = cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnConstraint
        set cclutParserText =
            concat(cclutParserText, ^ ^, cclutColumnName,
            ^ = type("^, cclut_mockTables->tables[cclutMockTablePos].columns[cclutMockColumnIndex].columnType, ^")^)
        if (cclutConstraint > " ")
            set cclutParserConstraint =
                concat(cclutParserConstraint, ^ , constraint(^, cclutColumnName, ^,"^, cclutConstraint, ^")^)
        endif
    endfor
    set cclutParserText = concat(cclutParserText, ^ with organization="P" ^)
    if (cclutParserConstraint > " ")
        set cclutParserText = concat(cclutParserText, cclutParserConstraint)
    endif
    for (cclutMockIndexIndex = 1 to size(cclut_mockTables->tables[cclutMockTablePos].indexes, 5))
        set cclutParserText = concat(cclutParserText,
            evaluate(cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexIndex].isUnique, 1,
            " , indexunique(", " , index("),
            replace(cclut_mockTables->tables[cclutMockTablePos].indexes[cclutMockIndexIndex].indexColumns, "|", ","),
            ")")
    endfor

    set cclutParserText = concat(cclutParserText, " go ")
    call parser(cclutParserText)

    set cclut_mockTables->tables[cclutMockTablePos].isFinalized = TRUE
end ;cclutCreateMockTable

/**
Removes the mock table.  If the table was already created, it will also be dropped.  If the table is not currently
mocked, it will return silently.  tableName is required.

@param tableName
    The table that is mocked.

Example:
call cclutRemoveMockTable("person")
**/
subroutine cclutRemoveMockTable(tableName)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare cclutMockTableLength = i4 with protect, noconstant(size(cclut_mockTables->tables, 5))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutParserText = vc with protect, noconstant("")

    ;Validate inbound parameter
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutRemoveMockTable() - tableName is a required field")
        return
    endif

    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, cclutMockTableLength, CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    if (cclutMockTablePos > 0)
        if (cclut_mockTables->tables[cclutMockTableIndex].isFinalized)
            ;Drop the table if it was already added
            set cclutParserText =
                concat(" drop table ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " go ")
            call parser(cclutParserText)
            set cclutParserText =
                concat(" rdb drop table ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " end go ")
            call parser(cclutParserText)
        endif
        ;Remove it from the mocked list
        set stat = alterlist(cclut_mockTables->tables, cclutMockTableLength - 1, cclutMockTablePos - 1)
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
Add a row of mock data to a table.  tableName and rowData are required.  tableName must have already been created
through cclutCreateMockTable() or an error will be thrown.  rowData is a pipe-delimited string for each column in the
same order that was used in cclutDefineMockTable().  For character fields, the backslash (\) will serve as an escape
character.  For date fields, the value in rowData will be supplied to the cnvtdatetime() function.  All other values
will be passed as-is.

Supported escape values
\| = | (to represent a pipe in character fields)
\\ = \ (to represent a backslash in character fields)
\null = null (no value will be inserted into the column)

@param tableName
    The table to which the data will be added.
@param rowData
    A pipe-delimited string of data to be inserted into the table.

Example:
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8")
call cclutCreateMockTable("person")
call cclutAddMockData("person", "1.0|Washington|George|01-JAN-1970 00:00") ;Will add George Washington
call cclutAddMockData("person", "2.0|A\|d\\ams|John|02-FEB-1971 11:11") ;Will add John A|d\ams
call cclutAddMockData("person", "3.0|Jefferson|\null|03-MAR-1972 22:22") ;Will add Jefferson (no first name)
call cclutAddMockData("person", "4.0|Madison||04-APR-1973 10:33") ;Will add Madison (empty string for first name)
**/
subroutine cclutAddMockData(tableName, rowData)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare CCLUT_ROW_DATA = vc with protect, constant(rowData)
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)
    declare cclutMockDataLength = i4 with protect, noconstant(size(CCLUT_ROW_DATA))
    declare cclutMockDataIndex = i4 with protect, noconstant(0)
    declare cclutMockColumnLength = i4 with protect,
        noconstant(size(cclut_mockTables->tables[cclutMockTablePos].columns, 5))
    declare cclutMockColumnIndex = i4 with protect, noconstant(1)
    declare cclutHasComma = i4 with protect, noconstant(0)
    declare cclutNullColumn = i4 with protect, noconstant(0)
    declare cclutDataBuffer = vc with protect, noconstant(" ")
    declare cclutEscapeBuffer = vc with protect, noconstant("")
    declare cclutDataCharacter = vc with protect, noconstant("")
    declare cclutParserText = vc with protect, noconstant("")

    ;Validate inbound parameters
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutAddMockData() - tableName is a required field")
        return
    endif

    ;Validate that the table exists and has been created
    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    if (cclutMockTablePos < 1)
        call cclexception(100, "E", concat("cclutAddMockData() - tableName has not been mocked. tableName: ",
            CCLUT_TABLE_NAME))
        return
    elseif (cclut_mockTables->tables[cclutMockTablePos].isFinalized = FALSE)
        call cclexception(100, "E", concat("cclutAddMockData() - tableName has not been created. tableName: ",
            CCLUT_TABLE_NAME))
        return
    endif

    ;Initialize insert statement
    set cclutParserText = concat(" insert into ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " set ")
    call parser(cclutParserText)

    ;Special case if rowData is empty for single-column tables
    if (cclutMockDataLength = 0 or (cclutMockDataLength = 1 and ichar(substring(1, 1, CCLUT_ROW_DATA)) = 0))
        set cclutDataBuffer = cclut::cclutParseColumn(cclutMockTablePos, 1, " ")
        if (cclutDataBuffer > " ")
            call parser(cclutDataBuffer)
            set cclutMockColumnIndex = cclutMockColumnIndex + 1
        else
            ;Error occurred in cclutParseColumn, return
            return
        endif
    else
        ;Iterate over every character in rowData
        for (cclutMockDataIndex = 1 to cclutMockDataLength)
            set cclutParserText = " "
            ;Grab the next character
            set cclutDataCharacter = notrim(substring(cclutMockDataIndex, 1, CCLUT_ROW_DATA))
            ;If the character is a pipe or the end of the string
            if (cclutDataCharacter = "|" or cclutMockDataIndex = cclutMockDataLength)
                if (cclutNullColumn = FALSE)
                    ;Check if the comma is needed
                    if (cclutHasComma)
                        set cclutParserText = concat(cclutParserText, " , ")
                    endif

                    ;Add the last character if the end of the string is reached and it is not a pipe
                    if (cclutMockDataIndex = cclutMockDataLength and cclutDataCharacter != "|")
                        set cclutDataBuffer = notrim(concat(cclutDataBuffer, cclutDataCharacter))
                    endif

                    ;Construct the column insertion
                    set cclutDataBuffer =
                        cclut::cclutParseColumn(cclutMockTablePos, cclutMockColumnIndex, cclutDataBuffer)
                    if (cclutDataBuffer > " ")
                        set cclutParserText = concat(cclutParserText, " ", cclutDataBuffer)
                    else
                        ;Error occurred in cclutParseColumn, return
                        return
                    endif

                    ;Add one final empty string if the pipe is the last character
                    if (cclutMockDataIndex = cclutMockDataLength and cclutDataCharacter = "|")
                        set cclutDataBuffer = cclut::cclutParseColumn(cclutMockTablePos, cclutMockColumnIndex + 1, " ")
                        if (cclutDataBuffer > " ")
                            set cclutParserText = concat(cclutParserText, " , ", cclutDataBuffer)
                            set cclutMockColumnIndex = cclutMockColumnIndex + 1
                        else
                            ;Error occurred in cclutParseColumn, return
                            return
                        endif
                    endif

                    set cclutHasComma = 1
                    call parser(cclutParserText)
                    set cclutDataBuffer = " "
                    set cclutMockColumnIndex = cclutMockColumnIndex + 1
                else
                    set cclutNullColumn = FALSE
                endif
            ;If the character is an escape character
            elseif (cclutDataCharacter = "\")
                ;Evaluate escape conditions
                set cclutEscapeBuffer = substring(cclutMockDataIndex + 1, 1, CCLUT_ROW_DATA)
                if (cclutEscapeBuffer = "|" or cclutEscapeBuffer = "\")
                    set cclutDataBuffer = notrim(concat(cclutDataBuffer, cclutEscapeBuffer))
                    if (cclutMockDataLength = cclutMockDataIndex + 1)
                        ;End of string was reached, save off last column
                        if (cclutHasComma)
                            set cclutParserText = concat(cclutParserText, " , ")
                        endif
                        set cclutDataBuffer =
                            cclut::cclutParseColumn(cclutMockTablePos, cclutMockColumnIndex, cclutDataBuffer)
                        if (cclutDataBuffer > " ")
                            set cclutParserText = concat(cclutParserText, " ", cclutDataBuffer)
                        else
                            ;Error occurred in cclutParseColumn, return
                            return
                        endif
                        set cclutHasComma = 1
                        call parser(cclutParserText)
                        set cclutDataBuffer = " "
                        set cclutMockColumnIndex = cclutMockColumnIndex + 1
                    endif
                    set cclutMockDataIndex = cclutMockDataIndex + 1
                else
                    set cclutEscapeBuffer = substring(cclutMockDataIndex + 1, 4, CCLUT_ROW_DATA)
                    if (cclutEscapeBuffer = "null")
                        ;Validate that null is by itself in the column
                        if ((cclutMockDataIndex = 1 or
                            (substring(cclutMockDataIndex - 1, 1, CCLUT_ROW_DATA) = "|" and
                            (cclutMockDataIndex = 2 or substring(cclutMockDataIndex - 2, 1, CCLUT_ROW_DATA) != "\")))
                            and
                            (cclutMockDataIndex + 4 = cclutMockDataLength or
                            substring(cclutMockDataIndex + 5, 1, CCLUT_ROW_DATA) = "|"))
                                set cclutMockColumnIndex = cclutMockColumnIndex + 1
                                if (cclutMockDataLength != cclutMockDataIndex + 4)
                                    set cclutMockDataIndex = cclutMockDataIndex + 4
                                endif
                                set cclutNullColumn = TRUE
                        else
                            ;\null must be by itself
                            call cclexception(100, "E",
                                concat("cclutAddMockData() - \null column must have no other characters. rowData: ",
                                    CCLUT_ROW_DATA))
                            call parser(" reset ")
                            call parser(" set modify system go ")
                            return
                        endif
                    else
                        ;Invalid escape character
                        call cclexception(100, "E", concat("cclutAddMockData() - Invalid escape sequence. rowData: ",
                            CCLUT_ROW_DATA))
                        call parser(" reset ")
                        call parser(" set modify system go ")
                        return
                    endif
                endif
            ;All other characters
            else
                set cclutDataBuffer = notrim(concat(cclutDataBuffer, cclutDataCharacter))
            endif
        endfor
    endif

    if (cclutMockColumnIndex <= cclutMockColumnLength)
        call cclexception(100, "E", concat("cclutAddMockData() - Too few columns of data. rowData: ", CCLUT_ROW_DATA))
        call parser(" reset ")
        call parser(" set modify system go ")
        return
    endif

    ;No columns have been added meaning all are null
    if (cclutHasComma = 0)
        call parser(" reset ")
        call parser(concat(" rdb insert into ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " values ("))
        for (cclutMockColumnIndex = 1 to cclutMockColumnLength)
            call parser(evaluate(cclutMockColumnIndex, 1, "NULL", ",NULL"))
        endfor
        call parser(") end ")
    endif

    call parser(" go ")
end ;cclutAddMockData

/**
Helper function to parse out a column for a mock insertion.
**/
subroutine cclut::cclutParseColumn(tableIndex, columnIndex, dataBuffer)
    declare CCLUT_COLUMN_COUNT = i4 with protect, constant(size(cclut_mockTables->tables[tableIndex].columns, 5))
    declare cclutMockColumnType = vc with protect, noconstant("")
    declare cclutMockColumnName = vc with protect, noconstant("")

    if (columnIndex > CCLUT_COLUMN_COUNT)
        call cclexception(100, "E", concat("cclutParseColumn() - Too many columns of data added to table. tableName: ",
            cclut_mockTables->tables[tableIndex].originalName))
        call parser(" reset ")
        call parser(" set modify system go ")
        return("")
    endif

    set cclutMockColumnType = cclut_mockTables->tables[tableIndex].columns[columnIndex].columnType
    set cclutMockColumnName = cclut_mockTables->tables[tableIndex].columns[columnIndex].columnName

    ;Use the @ delimiter for string columns
    if (findstring("C", cclutMockColumnType) > 0)
        return(concat(cclutMockColumnName, " = @",
            trim(cnvtstring(size(dataBuffer)), 3), ":", dataBuffer, "@ "))
    ;Use cnvtdatetime for date columns
    elseif (findstring("D", cclutMockColumnType) > 0)
        return(concat(cclutMockColumnName, ^ = cnvtdatetime("^, dataBuffer, ^") ^))
    ;Everything else is set directly
    else
        return(concat(cclutMockColumnName, " = ", dataBuffer))
    endif
end ;cclut::cclutParseColumn

/**
Clears all data from the mock table.  This is functionally similar to a truncate.  tableName is required.  The table
must have been created through cclutCreateMockTable() or else an error will be thrown.

@param tableName
    The table to be cleared.

Example:
call cclutClearMockData("person")
**/
subroutine cclutClearMockData(tableName)
    ;Declare local variables
    declare CCLUT_TABLE_NAME = vc with protect, constant(trim(cnvtupper(tableName), 3))
    declare cclutMockTablePos = i4 with protect, noconstant(0)
    declare cclutMockTableIndex = i4 with protect, noconstant(0)

    ;Validate inbound parameters
    if (CCLUT_TABLE_NAME <= " ")
        call cclexception(100, "E", "cclutClearMockData() - tableName is a required field")
        return
    endif

    ;Validate that the table exists and has been created
    set cclutMockTablePos = locateval(cclutMockTableIndex, 1, size(cclut_mockTables->tables, 5), CCLUT_TABLE_NAME,
        cclut_mockTables->tables[cclutMockTableIndex].originalName)
    if (cclutMockTablePos < 1)
        call cclexception(100, "E", concat("cclutClearMockData() - tableName has not been mocked. tableName: ",
            CCLUT_TABLE_NAME))
        return
    elseif (cclut_mockTables->tables[cclutMockTablePos].isFinalized = FALSE)
        call cclexception(100, "E", concat("cclutClearMockData() - tableName has not been created. tableName: ",
            CCLUT_TABLE_NAME))
        return
    endif

    call parser(concat(" delete from ", cclut_mockTables->tables[cclutMockTablePos].replaceName, " where seq = 1 go "))
end ;cclutClearMockData

%#endif CCLUTMOCK_TABLE_INC