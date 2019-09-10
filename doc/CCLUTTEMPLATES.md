
# CCL Unit Templates
The CCL Unit Test framework's templating program is available for consumers to quickly generate a test case template of all subroutines in a script-under-test, stubbing in mocks where appropriate.  This makes it easier for a developer to fill in the mock implementations they want to use and write their tests.

## Contents
[API](#api)  
[Implementation Notes](#implementation-notes)  
[Example](#example)

## API

**execute cclut_generate_test_case \<outputDestination\>, \<objectName\>, \<sourceFileLocation\>, \<includeFiles\> go**

outputDestination - This is the location to which the generated test case will be output.  If it is "MINE", the test case will be output to the appropriate output device (e.g. the terminal in interactive CCL or the Discern Output Viewer in Discern Visual Developer).  The default value is "MINE".

objectName - This is the name of the compiled CCL object for the script-under-test.  If the object cannot be found in the CCL dictionary, an error message will be displayed.  If an object name is not provided, usage instructions will be sent to the outputDestination.

sourceFileLocation - This is the location of the source file for the script-under-test.  If it is not provided, the default location is cclsource:\<objectName\>.prg.  If the provided (or default) file does not exist, a test case will still be generated, but it will include test subroutines for subroutines in all include files.  In order to generate a test case that does not include test subroutines for all include files, a valid sourceFileLocation must be provided (if the default would not be correct).  If the located source file is not the file that was used to create the program, unexpected results may occur.

includeFiles - By default, all include files from the sourceFileLocation are excluded from having test subroutines generated.  If an include file from the source file _should_ be included for testing, it should be added to this parameter.  This parameter is a pipe-delimited string of all include files that should be included for testing.  The listed files are assumed to reside in cclsource: and have a .inc extension unless otherwise specified.

Examples:
```
execute cclut_generate_test_case "MINE", "my_ccl_program", "", "my_ccl_include" go
execute cclut_generate_test_case "ccluserdir:ut_another_program.inc", "another_program", "ccluserdir:my_other_ccl_program.prg", "first_include|second_include.dat|ccluserdir:third_include.inc" go
execute cclut_generate_test_case "MINE", "abc_run_report" go
```

## Implementation Notes

The CCL Unit test case that is generated follows best practices for unit testing and expects certain patterns to be followed when constructing a CCL program as outlined in the other documentation for this project.

It is assumed that the script-under-test is structured with testable logic in subroutines and a "main" subroutine that calls all the other subroutines.  If there is no "main" subroutine, a test case will not be generated and an error will be displayed stating that a "main" subroutine is necessary.

Every test case will be generated with setupOnce, setup, tearDown, and tearDownOnce subroutines.  These can be implemented or removed as needed, but it is recommended to keep the tearDown subroutine since it follows best practice of removing mocks and rolling back any uncommitted changes made to the database.

Below is a simple example of what a generated test case might look like for a subroutine called getPerson in a script called my_program:

```
subroutine testGETPERSON(null)
    declare testGETPERSON__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")
    call cclutExecuteProgramWithMocks("my_program", "", "1_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETPERSON auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETPERSON(null)
    call GETPERSON(null)
    set testGETPERSON__MainWasCalled = TRUE
end
```

The test subroutine that is generated will have a name of "test\<subroutineName\>" (in this case, testGETPERSON).

The first line of the test subroutine creates a variable called test\<subroutineName\>__MainWasCalled that is initialized to FALSE.  The purpose of this variable is to validate that the subroutine you are testing is being called.  CCL Unit best practice is to have testable logic inside subroutines with a "main" subroutine.  The CCL Unit test then overrides the "main" subroutine to call only that one subroutine and nothing else.  This allows each test to test its subroutine in isolation.

The overriding "main" subroutine is the second subroutine in the example above.  Its name is MAINtest\<subroutineName\> and its implementation is to call the subroutine (in this case GETPERSON(null)), and set the testGETPERSON__MainWasCalled variable to TRUE.  By default, no parameters are sent to the subroutine being tested, but if the subroutine takes parameters, those can be added here.  Additionally, if the subroutine returns a value, that value can be captured and asserted within the mock main.

In order for the mock main to be used instead of the real main, a mock implementation is added:  
`call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")`  
Other mocks can be added here as well, and in some cases the templating program will assist in generating those if there is enough information.

The next line executes the program with mocks:  
`call cclutExecuteProgramWithMocks("my_program", "", "1_GETPERSON")`  
By default, no parameters are sent to the program, but if the program takes parameters, those can be added here.  A namespace is also added with a default of "1_\<subroutineName\>".  This can be useful for mocking subroutines defined by your program that are called from the subroutine being tested.  More examples are provided later in this document.

Following program execution, two asserts are added:
```
call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
call cclutAsserti2Equal(CURREF, "testGETPERSON auto-generate test.  Fail by default.", TRUE, FALSE)
```
The first assert validates that the mock main was called.  The second assert is an intentionally failing test.  If the test case is generated fresh, the first assertion is expected to pass (assuming the subroutine and script can be called without parameters and generate no errors), but the second assertion is expected to fail.  This allows a consumer to see which tests they have already implemented if all tests are run together.

### Mock Subroutines

For unit tests, it is considered best practice to mock as much external logic as possible and only test the "unit" of work for the given test.  In CCL, subroutines are the unit.  As such, if your subroutine calls other subroutines in your program, those should generally be mocked.  The templating program will identify subroutines called by the subroutine being tested and create stub mocks to be used.  Let us assume that the getPerson subroutine from the earlier example calls two other subroutines - getId and getName.  Below is an example of how that would be generated:

```
subroutine testGETPERSON(null)
    declare testGETPERSON__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")
    call cclutExecuteProgramWithMocks("my_program", "", "1_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETPERSON auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETPERSON(null)
    call GETPERSON(null)
    set testGETPERSON__MainWasCalled = TRUE
end
; TODO: All calls to subroutines declared in the script-under-test have been prepped for mocking using namespaces such as below.
; If the inbound parameters need to be captured or the return type specified, each mock should be updated accordingly.
subroutine (1_GETPERSON::GETID(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
subroutine (1_GETPERSON::GETNAME(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
```

The test subroutine and mock main are identical to before.  The new lines are:
```
; TODO: All calls to subroutines declared in the script-under-test have been prepped for mocking using namespaces such as below.
; If the inbound parameters need to be captured or the return type specified, each mock should be updated accordingly.
subroutine (1_GETPERSON::GETID(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
subroutine (1_GETPERSON::GETNAME(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
```

The first TODO comment is a notice to the user that these mocks have been generated throughout the whole test case and that they can be modified if necessary to accept parameters or return values.  This TODO comment will only appear for the first instance of a mock subroutine in the generated test case.  The other TODOs will be present for every mock subroutine stub so that the consumer can take an action (either by implementing a mock or removing the implementation).  The default implementation is that it takes no parameters, returns no value, and performs no operation.

Notice that these mocks have the namespace prepended to them that was generated from the original test subroutine.  Since the program is executed with the 1_GETPERSON namespace, these mocks will be used instead of the real implementation.

The number at the start of the namespace is for creating multiple tests of the same subroutine.  Let us say you have a conditional in the getPerson subroutine.  In order to test both scenarios (where the condition is true and where the condition is false), you would need two tests.  The consumer can copy the test created by the templating program and update the namespace to be 2_GETPERSON (and can continue incrementing for as many tests as needed).  The name of the test should also be changed to avoid conflict with the first test.  Below is an example of what such a test might look like:
```
subroutine testGETPERSON_condition_false(null)
    declare testGETPERSON_condition_false__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON_condition_false")
    call cclutExecuteProgramWithMocks("my_program", "", "2_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON_condition_false 001", testGETPERSON_condition_false__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETPERSON_condition_false auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETPERSON_condition_false(null)
    call GETPERSON(null)
    set testGETPERSON_condition_false__MainWasCalled = TRUE
end
; TODO: All calls to subroutines declared in the script-under-test have been prepped for mocking using namespaces such as below.
; If the inbound parameters need to be captured or the return type specified, each mock should be updated accordingly.
subroutine (2_GETPERSON::GETID(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
subroutine (2_GETPERSON::GETNAME(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
```

### Mock Scripts

External scripts that are called by the subroutine being tested are another good candidate for being mocked to give you control over the return values and test that the subroutine can handle both good and bad responses.

Going back to the original example, let us say that the getPerson subroutine calls an external script called "search_persons".  The generated test case would look like this:
```
subroutine testGETPERSON(null)
    declare testGETPERSON__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    ; TODO: Script calls in the script-under-test have been prepped for mocking in this file using cclutAddMockImplementation.
    ; Each instance of cclutAddMockImplementation in this file must be updated with the mock script name in the second parameter.
    call cclutAddMockImplementation("SEARCH_PERSONS", "TODO: delete line or add mock script name")
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")
    call cclutExecuteProgramWithMocks("my_program", "", "1_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETPERSON auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETPERSON(null)
    call GETPERSON(null)
    set testGETPERSON__MainWasCalled = TRUE
end
```

With the new lines that were added in the test subroutine being:

```
; TODO: Script calls in the script-under-test have been prepped for mocking in this file using cclutAddMockImplementation.
; Each instance of cclutAddMockImplementation in this file must be updated with the mock script name in the second parameter.
call cclutAddMockImplementation("SEARCH_PERSONS", "TODO: delete line or add mock script name")
```

The first TODO comment is a notice to the user that these mocks have been generated throughout the whole test case and that they must be updated with the appropriate mock script.  This TODO comment will only appear for the first instance of a mock script in the generated test case.  The second TODO will be present for every mock script stub so that the consumer can take an action (either be adding the mock script or removing the line).

Once the consumer has created a mock program that has been compiled for this test, the line can be updated.  Assuming the mock program is called GET_PERSON_TEST_SEARCH_PERSONS, the line would be as follows:
`call cclutAddMockImplementation("SEARCH_PERSONS", "GET_PERSON_TEST_SEARCH_PERSONS")`

### Mock Tables And Data

Prior to the CCL Unit Mocking framework, it could be quite difficult to simulate data on Millennium tables.  Either the consumer would have to find pre-existing data to use (if it was static, then hoping that it did not get deleted; if it was dynamic, then hoping that there were results), or inject the data themselves with insert commands which may come with other issues if there are foreign keys that need to be satisfied or non-null fields that are not relevant to the functionality being tested.

Now with mock tables, only those columns that are necessary for the subroutine need to be mocked and it will not impact the real table.  The templating program will identify any tables and columns being used by the subroutine being tested and prepare a mock version in the test.  Going back to the original example, let us say that the getPerson leverages the PERSON table (and columns PERSON_ID, NAME_LAST, and NAME_FIRST) and the ENCOUNTER table (and columns ENCNTR_ID, PERSON_ID, and LOCATION_CD).  Below is how it would be generated:
```
subroutine testGETPERSON(null)
    declare testGETPERSON__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    ; TODO: Table calls in the script-under-test have been prepped for mocking in this file using cclutDefineMockTable.  Each
    ; instance of cclutDefineMockTable in this file must be updated with the column types in the second parameter.
    call cclutDefineMockTable("PERSON", "PERSON_ID|NAME_LAST|NAME_FIRST", "TODO: delete line or add parameter types for mock tabl\
e columns") 
    call cclutCreateMockTable("PERSON")
    call cclutAddMockData("PERSON", "TODO: delete line or add mock data")
    call cclutDefineMockTable("ENCOUNTER", "ENCNTR_ID|PERSON_ID|LOCATION_CD", "TODO: delete line or add parameter types for mock \
table columns") 
    call cclutCreateMockTable("ENCOUNTER")
    call cclutAddMockData("ENCOUNTER", "TODO: delete line or add mock data")
            
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")
    call cclutExecuteProgramWithMocks("my_program", "", "1_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETPERSON auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETPERSON(null)
    call GETPERSON(null)
    set testGETPERSON__MainWasCalled = TRUE
end
```   

With the new lines that were generated in the test subroutine being:
```
    ; TODO: Table calls in the script-under-test have been prepped for mocking in this file using cclutDefineMockTable.  Each
    ; instance of cclutDefineMockTable in this file must be updated with the column types in the second parameter.
    call cclutDefineMockTable("PERSON", "PERSON_ID|NAME_LAST|NAME_FIRST", "TODO: delete line or add parameter types for mock tabl\
e columns") 
    call cclutCreateMockTable("PERSON")
    ; TODO: All mock tables have the below line to aid in adding mock data.  Each instance of cclutAddMockData in this file must
    ; be updated with the mock data in the second parameter.  The line can be copied multiple times for multiple rows of data.
    call cclutAddMockData("PERSON", "TODO: delete line or add mock data")
    call cclutDefineMockTable("ENCOUNTER", "ENCNTR_ID|PERSON_ID|LOCATION_CD", "TODO: delete line or add parameter types for mock \
table columns") 
    call cclutCreateMockTable("ENCOUNTER")
    call cclutAddMockData("ENCOUNTER", "TODO: delete line or add mock data")
```

The first TODO comment and the TODO before the first cclutAddMockData call are notices to the user that mock tables and data have been generated throughout the whole test case and that they must be updated with the correct parameters and values.  These TODO comments will only appear for the first instance of a mock table in the generated test case.

The TODO comment in the second parameter of cclutDefineMockTable is where the types for the columns should be filled out (e.g. vc500, i4, f8, etc.).  The TODO comment in the second parameter of cclutAddMockData is where the test data to be added should be filled out.  Both of these are pipe-delimited strings and the cclutAddMockData line can be copied for as many rows of data as needed.  More information about the API for mocks can be found at [CCLUTMOCKING.md](./CCLUTMOCKING.md).

An example of how this might be implemented is below:
```
call cclutDefineMockTable("PERSON", "PERSON_ID|NAME_LAST|NAME_FIRST", "f8|vc100|vc100") 
call cclutCreateMockTable("PERSON")
call cclutAddMockData("PERSON", "1.0|Washington|George")
call cclutAddMockData("PERSON", "2.0|Adams|John")
call cclutAddMockData("PERSON", "3.0|Jefferson|Thomas")
call cclutDefineMockTable("ENCOUNTER", "ENCNTR_ID|PERSON_ID|LOCATION_CD", "f8|f8|f8") 
call cclutCreateMockTable("ENCOUNTER")
call cclutAddMockData("ENCOUNTER", "10.0|2.0|123.0")
call cclutAddMockData("ENCOUNTER", "11.0|3.0|123.0")
call cclutAddMockData("ENCOUNTER", "12.0|3.0|124.0")
``` 

## Example

Taking all of these concepts together, let us look at a full example of what the templating program would generate and a sample test implementation.  Here is the script-under-test:

```
/* Testing will be dependent on how the script is intended to be used.  For this one, we will assume that this script is designed 
to be called from other CCL scripts, it will populate a personRequest record structure with the person_id, and they will create 
a record structure to replace the personReply to get the information back (personReply will be declared with public scope).*/
drop program abc_get_person:dba go
create program abc_get_person:dba

/*
Expected record structure
record personRequest (
	1 person_id = f8
)*/

record personReply (
	1 person_id = f8
	1 person_name = vc
	1 encounter_details[1]
		2 encounter_id = f8
		2 location_cd = f8
)
 
subroutine (PUBLIC::getPerson(null) = null with protect)
	set personReply->person_id = personRequest->person_id
	set personReply->person_name = getName(personReply->person_id)
	call getEncounterDetails(personReply)
end

subroutine (PUBLIC::getName(personId = f8) = vc with protect)
	; Assume that abc_get_person_name behaves similarly to this script.  A request and reply are sent in to get the data out.
	record personNameRequest (
		1 person_id = f8
	)
	record personNameReply (
		1 person_name = vc
	)
	set personNameRequest->person_id = personId
	execute abc_get_person_name
	return (personNameReply->person_name)
end

subroutine (PUBLIC::getEncounterDetails(personReplyRec = vc(ref)) = null with protect)
	select into "nl:"
	from encounter e
	where e.person_id = personReplyRec->person_id
	head report
		personReplyRec->encounter_details[1].encounter_id = e.encntr_id
		personReplyRec->encounter_details[1].location_cd = e.location_cd
	with nocounter
end
 
subroutine (PUBLIC::main(null) = null)
	call getPerson(null)
end

call main(null)
 
end
go
```

Here is what the initial generated file would look like:

```
subroutine (setupOnce(null) = null)
    ; Place any test case-level setup here
    null
end
subroutine (setup(null) = null)
    ; Place any test-level setup here
    null
end
subroutine (tearDown(null) = null)
    ; Place any test-level teardown here
    call cclutRemoveAllMocks(null)
    rollback
end
subroutine (tearDownOnce(null) = null)
    ; Place any test case-level teardown here
    null
end
 
subroutine testGETPERSON(null)
    declare testGETPERSON__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETPERSON auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETPERSON(null)
    call GETPERSON(null)
    set testGETPERSON__MainWasCalled = TRUE
end
; TODO: All calls to subroutines declared in the script-under-test have been prepped for mocking using namespaces such as below.
; If the inbound parameters need to be captured or the return type specified, each mock should be updated accordingly.
subroutine (1_GETPERSON::GETNAME(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
subroutine (1_GETPERSON::GETENCOUNTERDETAILS(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
 
subroutine testGETNAME(null)
    declare testGETNAME__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    ; TODO: Script calls in the script-under-test have been prepped for mocking in this file using cclutAddMockImplementation.
    ; Each instance of cclutAddMockImplementation in this file must be updated with the mock script name in the second parameter.
    call cclutAddMockImplementation("ABC_GET_PERSON_NAME", "TODO: delete line or add mock script name")
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETNAME")
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_GETNAME")
 
    call cclutAsserti2Equal(CURREF, "testGETNAME 001", testGETNAME__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETNAME auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETNAME(null)
    call GETNAME(null)
    set testGETNAME__MainWasCalled = TRUE
end
 
subroutine testGETENCOUNTERDETAILS(null)
    declare testGETENCOUNTERDETAILS__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    ; TODO: Table calls in the script-under-test have been prepped for mocking in this file using cclutDefineMockTable.  Each
    ; instance of cclutDefineMockTable in this file must be updated with the column types in the second parameter.
    call cclutDefineMockTable("ENCOUNTER", "PERSON_ID|ENCNTR_ID|LOCATION_CD", "TODO: delete line or add parameter types for mock \
table columns")
    call cclutCreateMockTable("ENCOUNTER")
    ; TODO: All mock tables have the below line to aid in adding mock data.  Each instance of cclutAddMockData in this file must
    ; be updated with the mock data in the second parameter.  The line can be copied multiple times for multiple rows of data.
    call cclutAddMockData("ENCOUNTER", "TODO: delete line or add mock data")
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETENCOUNTERDETAILS")
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_GETENCOUNTERDETAILS")
 
    call cclutAsserti2Equal(CURREF, "testGETENCOUNTERDETAILS 001", testGETENCOUNTERDETAILS__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETENCOUNTERDETAILS auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine MAINtestGETENCOUNTERDETAILS(null)
    call GETENCOUNTERDETAILS(null)
    set testGETENCOUNTERDETAILS__MainWasCalled = TRUE
end
 
subroutine testMAIN(null)
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_MAIN")
 
    call cclutAsserti2Equal(CURREF, "testMAIN auto-generate test.  Fail by default.", TRUE, FALSE)
end
subroutine (1_MAIN::GETPERSON(null) = null)
    ; TODO: delete line or add mock subroutine implementation
    null
end
```

If we fill in these implementations to make actual tests, we might end up with something like below:

```
subroutine (setupOnce(null) = null)
    ; Place any test case-level setup here
    null
end
subroutine (setup(null) = null)
    ; Place any test-level setup here
    null
end
subroutine (tearDown(null) = null)
    ; Place any test-level teardown here
    call cclutRemoveAllMocks(null)
    rollback
end
subroutine (tearDownOnce(null) = null)
    ; Place any test case-level teardown here
    null
end
 
/*
For getPerson, we want to confirm that it is calling the other subroutines and setting those values on to personReply.
*/
subroutine testGETPERSON(null)
    declare testGETPERSON__MainWasCalled = i2 with protect, noconstant(FALSE)
 
 	record getPersonPersonRequest (
 		1 person_id = f8
 	)
 	record getPersonPersonReply (
		1 person_id = f8
		1 person_name = vc
		1 encounter_details[1]
			2 encounter_id = f8
			2 location_cd = f8
	)
	call cclutAddMockImplementation("PERSONREQUEST", "GETPERSONPERSONREQUEST")
	call cclutAddMockImplementation("PERSONREPLY", "GETPERSONPERSONREPLY")
	set getPersonPersonRequest->person_id = 123.0
	
    call cclutAddMockImplementation("MAIN", "MAINtestGETPERSON")
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_GETPERSON")
 
    call cclutAsserti2Equal(CURREF, "testGETPERSON 001", testGETPERSON__MainWasCalled, TRUE)
    call cclutAssertf8Equal(CURREF, "testGETPERSON 002", getPersonPersonReply->person_id, 123.0)
    call cclutAssertvcEqual(CURREF, "testGETPERSON 003", getPersonPersonReply->person_name, "James Madison")
    call cclutAssertf8Equal(CURREF, "testGETPERSON 004", getPersonPersonReply->encounter_details[1].encounter_id, 1000.0)
    call cclutAssertf8Equal(CURREF, "testGETPERSON 005", getPersonPersonReply->encounter_details[1].location_cd, 2000.0)
end
subroutine MAINtestGETPERSON(null)
    call GETPERSON(null)
    set testGETPERSON__MainWasCalled = TRUE
end
subroutine (1_GETPERSON::GETNAME(person_id = f8) = vc)
    call cclutAssertf8Equal(CURREF, "testGETPERSON 006", person_id, 123.0)
    return ("James Madison")
end
subroutine (1_GETPERSON::GETENCOUNTERDETAILS(reply = vc(ref)) = null)
    set reply->encounter_details[1].encounter_id = 1000.0
    set reply->encounter_details[1].location_cd = 2000.0
end
 
/*
For getName, we want to confirm that it is returning the value it receives from calling abc_get_person_name.  This will require a 
mock script that we will call abc_mock_get_person_name.
*/
subroutine testGETNAME(null)
    declare testGETNAME__MainWasCalled = i2 with protect, noconstant(FALSE)
    
    ; This variable will be set in our mock script so that we know getName is sending the correct value to it.
    declare inbound_person_id = f8 with protect, noconstant(0.0)
 
    call cclutAddMockImplementation("ABC_GET_PERSON_NAME", "ABC_MOCK_GET_PERSON_NAME")
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETNAME")
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_GETNAME")
 
    call cclutAsserti2Equal(CURREF, "testGETNAME 001", testGETNAME__MainWasCalled, TRUE)
    call cclutAssertf8Equal(CURREF, "testGETNAME 002", inbound_person_id, 456.0)
end
subroutine MAINtestGETNAME(null)
    declare personName = vc with protect, noconstant("")
    set personName = GETNAME(456.0)
    set testGETNAME__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGETNAME 003", personName, "James Monroe")
end
 
/*
For getEncounterDetails, we want to confirm that the values populated on the record structure are coming from the encounter table,
so we will mock it with our test data.
*/
subroutine testGETENCOUNTERDETAILS(null)
    declare testGETENCOUNTERDETAILS__MainWasCalled = i2 with protect, noconstant(FALSE)
 
    call cclutDefineMockTable("ENCOUNTER", "PERSON_ID|ENCNTR_ID|LOCATION_CD", "f8|f8|f8")
    call cclutCreateMockTable("ENCOUNTER")
    call cclutAddMockData("ENCOUNTER", "789.0|1001.0|2001.0")
 
    call cclutAddMockImplementation("MAIN", "MAINtestGETENCOUNTERDETAILS")
    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_GETENCOUNTERDETAILS")
 
    call cclutAsserti2Equal(CURREF, "testGETENCOUNTERDETAILS 001", testGETENCOUNTERDETAILS__MainWasCalled, TRUE)
end
subroutine MAINtestGETENCOUNTERDETAILS(null)
	record testReply (
		1 person_id = f8
		1 encounter_details[1]
			2 encounter_id = f8
			2 location_cd = f8
	)
	set testReply->person_id = 789.0
    call GETENCOUNTERDETAILS(testReply)
    set testGETENCOUNTERDETAILS__MainWasCalled = TRUE
    call cclutAssertf8Equal(CURREF, "testGETENCOUNTERDETAILS 002", testReply->encounter_details[1].encounter_id, 1001.0)
    call cclutAssertf8Equal(CURREF, "testGETENCOUNTERDETAILS 003", testReply->encounter_details[1].location_cd, 2001.0)
end
 
/*
For main, we want to confirm that it calls getPerson.
*/
subroutine testMAIN(null)
	declare getPersonCalled = i2 with protect, noconstant(FALSE)

    call cclutExecuteProgramWithMocks("abc_get_person", "", "1_MAIN")
 
    call cclutAsserti2Equal(CURREF, "testMAIN 001", getPersonCalled, TRUE)
end
subroutine (1_MAIN::GETPERSON(null) = null)
    set getPersonCalled = TRUE
end
```

And here is the mock program that was used for the testGETNAME test:
```
/* This is the mock program used by testGETNAME */
drop program abc_mock_get_person_name:dba go
create program abc_mock_get_person_name:dba

; Storing the inbound person id to a test-level variable to confirm that getName is setting it correctly.
set inbound_person_id = personNameRequest->person_id

; Returning a test value that will be asserted in the getName test.
set personNameReply->person_name = "James Monroe"

end
go
```