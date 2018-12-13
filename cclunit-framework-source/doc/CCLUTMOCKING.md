
# CCL Unit Mocking
**\*CAUTION\*** **The CCL Unit Mocking framework should only be used in non-production environments.  Table mocking creates new tables against an Oracle instance for the lifetime of the test.  Because the DDL is generated in a dynamic way, it is possible through inappropriate use of the framework to affect the actual table.  Please only use the documented API.**

## API

**cclutDefineMockTable(tableName = vc, fieldNames = vc, fieldTypes = vc)**

Defines a mock table structure that can be created for use within a program.  This is the first function to be called in the process of mocking a table.  It must be called before cclutAddMockIndex(), cclutAddMockConstraint(), and  
cclutCreateMockTable() can be called.  The table will not be mocked in cclutExecuteProgramWithMocks() unless  
cclutCreateMockTable() is called.  tableName, columnNames, and columnTypes are required.  columnNames and columnTypes are expected to be pipe-delimited strings.  The columnTypes should have the same count as columnNames and be in the same order.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to be mocked.  
@param columnNames  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited list of columns to be mocked on the table  
@param columnTypes  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited list of types for each column  
@returns  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the mock table (This can be used to select data for testing)  
  
Example:  
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8") 

**cclutAddMockConstraint(tableName = vc, columnName = vc, columnConstraint = vc)**

Adds a constraint to a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be thrown.  This function may not be called after cclutCreateMockTable().  tableName and columnName are required. If the columnName is not valid for the table specified, an error will be thrown.  All constraints for a column should be present in the columnConstraint field.  If a constraint already exists, this function will overwrite it with the new value.  If columnConstraint is blank, the constraint will be removed for the column.  The supported constraints can be seen here:  
https://wiki.cerner.com/display/public/1101discernHP/SELECT+INTO+TABLE+Table_Name+Using+Discern+Explorer  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to which the constraint will be added.  
@param columnName  
&nbsp;&nbsp;&nbsp;&nbsp;The column to which the constraint will be applied.  
@param columnConstraint  
&nbsp;&nbsp;&nbsp;&nbsp;A string of all constraints to be applied to the column.  
  
Example:  
call cclutAddMockConstraint("person", "name_last", "not null unique")

**cclutAddMockIndex(tableName = vc, columnNames = vc, isUnique = i4)**

Adds an index to a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be thrown.  This function may not be called after cclutCreateMockTable().  tableName, columnNames, and isUnique are required.  columnNames may be a single column name or a pipe-delimited list of columns for a composite index (the order of the columns will be the order of the index).  If isUnique is 1, then a unique index will be created.  If isUnique is 0, then a non-unique index will be created.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to which the index will be added.  
@param columnNames  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited string of column names for the index.  
@param isUnique  
&nbsp;&nbsp;&nbsp;&nbsp;1 to create a unique index; 0 to create a non-unique index  
  
Example:  
call cclutAddMockIndex("person", "person_id", 1)  
call cclutAddMockIndex("person", "name_last|name_first", 0)

**cclutCreateMockTable(tableName = vc)**

Creates the mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be thrown.  If the table has already been created, the function will return silently.  tableName is required.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to be mocked.  
  
Example:  
call cclutCreateMockTable("person")

**cclutRemoveMockTable(tableName = vc)**

Removes the mock table.  If the table was already created, it will also be dropped.  If the table is not currently  
mocked, it will return silently.  tableName is required.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table that is mocked.  
  
Example:  
call cclutRemoveMockTable("person")

**cclutRemoveAllMockTables(null)**

Removes all mock tables.  Any tables that have already been created will also be dropped.  
  
Example:  
call cclutRemoveAllMockTables(null)

**cclutAddMockData(tableName = vc, rowData = vc)**

Add a row of mock data to a table.  tableName and rowData are required.  tableName must have already been created through cclutCreateMockTable() or an error will be thrown.  rowData is a pipe-delimited string for each column in the same order that was used in cclutDefineMockTable().  For character fields, the backslash (\\) will serve as an escape character.  For date fields, the value in rowData will be supplied to the cnvtdatetime() function.  All other values will be passed as-is.  
  
Supported escape values  
\\| = | (to represent a pipe in character fields)  
\\\\ = \ (to represent a backslash in character fields)  
\null = null (no value will be inserted into the column)  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to which the data will be added.  
@param rowData  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited string of data to be inserted into the table.  
  
Example:  
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc|vc|dq8")  
call cclutCreateMockTable("person")  
call cclutAddMockData("person", "1.0|Washington|George|01-JAN-1970 00:00") ;Will add George Washington  
call cclutAddMockData("person", "2.0|A\\|d\\\\ams|John|02-FEB-1971 11:11") ;Will add John A|d\ams  
call cclutAddMockData("person", "3.0|Jefferson|\null|03-MAR-1972 22:22") ;Will add Jefferson (no first name)  
call cclutAddMockData("person", "4.0|Madison||04-APR-1973 10:33") ;Will add Madison (empty string for first name)

**cclutClearMockData(tableName = vc)**

Clears all data from the mock table.  This is functionally similar to a truncate.  tableName is required.  The table  
must have been created through cclutCreateMockTable() or else an error will be thrown.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to be cleared.  
  
Example:  
call cclutClearMockData("person")

**cclutAddMockImplementation(originalName = vc, replaceName = vc)**

Adds a mock implementation to be utilized by cclutExecuteProgramWithMocks.  This can include record structures, subroutines, or other programs.  originalName and replaceName are required.  If originalName is already being mocked, the mock will be overwritten with the new replaceName.  
  
@param originalName  
&nbsp;&nbsp;&nbsp;&nbsp;The object to be mocked.  
@param replaceName  
&nbsp;&nbsp;&nbsp;&nbsp;The mocked object.  
  
Example:  
call cclutAddMockImplementation("uar_get_code_by", "mock_uar_get_code_by")

**cclutRemoveMockImplementation(originalName = vc)**

Removes a mock implementation.  
  
@param originalName  
&nbsp;&nbsp;&nbsp;&nbsp;The object that is mocked.  
  
Example:  
call cclutRemoveMockImplementation("uar_get_code_by")

**cclutRemoveAllMockImplementations(null)**

Removes all mock implementations.  
  
Example:  
call cclutRemoveAllMockImplementations(null)

**cclutExecuteProgramWithMocks(programName = vc, params = vc, namespace = vc)**

Executes a program with all mocks currently added through cclutAddMockImplementation() and cclutCreateMockTable(). programName is required.  params is a string parameter to be sent directly to the program, so all commas and string delimiters must be specified.  If namespace is omitted, it will default to the PUBLIC namespace.  
  
Example:  
call cclutExecuteProgramWithMocks("ccl_my_program", "\^MINE^, 1.0, ^string parameter^", "MYNAMESPACE")

**cclutRemoveAllMocks**

Removes all mock implementations and mock tables that have been added through the cclutAddMockImplementation() and cclutCreateMockTable() APIs.  This should be called at the completion of a test suite to clean up all mocks.  
  
Example:  
call cclutRemoveAllMocks(null)

## Implementation Notes
1. cclutRemoveAllMocks should be called as part of the teardown for all tests.  The framework will attempt to clean up any outstanding mocks, but it is good practice to explicitly remove any mocks to ensure that no mocked tables remain in the Oracle instance.

2. The mocked items created through cclutCreateMockTable() and cclutAddMockImplementation will not be applied to children script called from the script-under-test.  Some alternatives would be to mock the child script to return the appropriate data or to mock the child script to execute the real script applying the mocked tables and implementations.

3. The mocked items created through cclutCreateMockTable() and cclutAddMockImplementation will not be applied to statements executed through "call parser()" commands.  An alternative would be to mock the parser() call to validate the correct information is supplied, then perform the appropriate mock versions of the actions the statement would normally perform.

4. The mocking API calls cannot be used from reportwriter sections.  Alternatives would be dependent on the use-case, but take, for example, a dummyt used with a record structure and a call to cclutAddMockData within the detail section in order to add data based on a record structure.  Instead, a FOR loop construct could be leveraged outside the context of a reportwriter section to iterate over the record structure.

5. Mocking record structures with a call to tdbexecute is unsupported under certain conditions, specifically if a call to free the record structure is made just prior to calling tdbexecute.  If the scenario is truly necessary for a test, the best alternative is to separate the freeing of the record structure and the call to tdbexecute in different subroutines and test the subroutines independently of each other.
  
## Example
Below is an example of some of the APIs available in the CCL Unit Mocking framework along with some simple notes.

Script-under-test:

    drop program cclut_get_persons:dba go
    create program cclut_get_persons:dba    
    
    declare newSize = i4 with protect, noconstant(0)
    
    select into "nl:"
    from person p
    plan p
	order by p.person_id
	detail
		newSize = newSize + 1
		stat = alterlist(reply->persons, newSize)
		reply->persons[newSize].person_id = p.person_id
		reply->persons[newSize].name_last = p.name_last
		reply->persons[newSize].name_first = p.name_first
		reply->persons[newSize].birth_dt_tm = p.birth_dt_tm
	with nocounter
    
    end
    go

Test Code:

    declare mockTable = vc with protect, noconstant("")
    
    ; Defining a mock person table.  The return value is the name of the mockTable.  
    ; This can be useful to perform a select on the table after the script-under-test is complete
    ; to verify (among other things) that an insert or a delete worked correctly.
    set mockTable = cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm",
	    "f8|vc|vc|dq8")
	
	; Add a constraint that the person_id cannot be null
	call cclutAddMockConstraint("person", "person_id", "not null")
	
	; Add a non-unique index to name_last
	call cclutAddMockIndex("person", "name_last", 0)
	
	; Creates the mock table.  After this, it is available for DML statements.
	call cclutCreateMockTable("person")
	
	; Create data for the table.
	call cclutAddMockData("person", "1.0|Washington|George|01-JAN-1970 00:00") ;Will add George Washington 
	call cclutAddMockData("person", "2.0|Adams|John|02-FEB-1971 11:11") ;Will add John Adams 
	call cclutAddMockData("person", "3.0|Jefferson|\null|03-MAR-1972 22:22") ;Will add Jefferson (no first name) 
	call cclutAddMockData("person", "4.0|Madison||04-APR-1973 10:33") ;Will add Madison (empty string for first name)
	
	record mock_reply (
		1 persons[*]
			2 person_id = f8
			2 name_last = vc
			2 name_first = vc
			2 birth_dt_tm = dq8
	) with protect
	
	; Replace the reply references with mock_reply
	call cclutAddMockImplementation("REPLY", "MOCK_REPLY")
	
	; Execute the script-under-test
	call cclutExecuteProgramWithMocks("cclut_get_persons", "")
	
	; Do validation
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[1].person_id, 1.0)  
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[1].name_last,
		"Washington") 
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[1].name_first,
		"George") 
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[1].birth_dt_tm,
		cnvtdatetime("01-JAN-1970 00:00"))
	
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[2].person_id, 2.0)  
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[2].name_last,
		"Adams") 
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[2].name_first,
		"John") 
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[2].birth_dt_tm,
		cnvtdatetime("02-FEB-1971 11:11"))
		
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[3].person_id, 3.0)  
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[3].name_last,
		"Jefferson") 
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[3].name_first,
		"") 
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[3].birth_dt_tm,
		cnvtdatetime("03-MAR-1972 22:22"))
	
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[4].person_id, 4.0)  
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[4].name_last,
		"Madison") 
	call cclutAssertvcEqual(CURREF, "test_get_people_happy", mock_reply->persons[4].name_first,
		"") 
	call cclutAssertf8Equal(CURREF, "test_get_people_happy", mock_reply->persons[4].birth_dt_tm,
		cnvtdatetime("04-APR-1973 10:33"))
	
	call cclutRemoveAllMocks(null)