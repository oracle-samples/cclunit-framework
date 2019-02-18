# CCL Unit Tests
Unit tests for CCL written in CCL.

## Contents
[Anatomy of CCL Unit](#anatomy-of-ccl-unit)  
[Mocking Framework](#mocking-framework)  
[Manual Execution](#manual-execution)  
[Maven Execution](#maven-execution)  
[Simple Example](#simple-example)
+ [Program](#program)
- [Test Case](#test-case)

## Anatomy of CCL Unit
A ***unit test*** is a null parameter subroutine whose name begins with "test".  
A ***test case*** is an include file with a ***.inc*** extension which contains unit tests and optionally setup and teardown routines.  

CCL Unit generates a \[temporary\] program from a test case file and then executes the unit tests defined within that file.

A ***setupOnce*** is a null parameter subroutine named setupOnce.  
&nbsp; &bull; If a test case contains a setupOnce, CCL Unit will execute it before executing any of the unit tests in the test case.   
&nbsp; &bull; Use a setupOnce to perform any preparation steps that need performed one time before executing all the tests.  
A ***setup*** is a null parameter subroutine named setup.  
&nbsp; &bull; If a test case contains a setup, CCL Unit will execute it just before the execution of each unit test in the test case.  
&nbsp; &bull; Use a setup to perform any preparation steps that need to be freshly repeated before each test is executed.  
A ***teardown*** is a null parameter subroutine named teardown.  
&nbsp; &bull; If a test case contains a teardown, CCL Unit will execute it just after the execution of each unit test in the test case.  
&nbsp; &bull; Use a teardown to perform any cleanup steps (such as rollback) that need to be repeated after each test completes.  
A ***teardownOnce*** is a null parameter subroutine named teardownOnce.  
&nbsp; &bull; If a test case contains a teardownOnce, CCL Unit will execute it after all of the tests in the test case have finished executing.  
&nbsp; &bull; Use a teardownOnce to perform any final cleanup steps that need performed one time after all the tests have completed.  
An ***assert*** is a statement asserting a particular condition is true.  
&nbsp; &bull; [**Look here**][cclutAsserts] for details about asserts and a list of all the avaiable asserts.  
&nbsp; &bull; Example:  `call cclutAssertVcEqual(CURREF, "expected hello world", some_str, "hello world")`  
A ***timer*** asserts the elapse between two execution points does not exceed a specified threshold.  
&nbsp; &bull; Start a timer named "timer name" by invoking `call cclutStartTimer("timer name", CURREF)`  
&nbsp; &nbsp; &nbsp; &bull;  The second parameter is optional and will default to CURREF.  
&nbsp; &nbsp; &nbsp; &bull;  An error will occur if there already is a timer with the same name.  
&nbsp; &bull; To complete the timer invoke `call cclutEndTimer("timer name", threshold)`  

A unit test will fail if it has an assert that fails or a timer that does not meet its threshold or if a CCL error occurs while the test is executing. It will pass otherwise.  

## Mocking Framework
See [CCL Unit Mocking][ccl-unit-mocking] for instructions on mocking database data and other things for CCL Unit Tests.



## Manual Execution
During devlopment it is desirable to perform the test phase of the modify/test/repeat cycle as swiftly as possible. 
Manually executing a single test provides for this and avoids the time overhead of the maven execution described farther down.

---
- Transfer your test case file to the back end.
- excute cclut_ff or cclut.
- Examine the output.
- Repeat.
 - If the test case file is modified, transfer it again.
 - If the testing target (script) is modified, recompile the script.

cclut_ff and cclut are prompt programs which generate a temporary program from a test case and execute its tests.  

The cclut_ff program fails fast, i.e., stops executing tests as soon as it encounters an assert failure or a runtime error. 
It indicates whether the tests executed successfully and if not, indicates the assert failure or error that was encountered.  
In contrast cclut continues executing tests even after it encounters assert failures or errors. After all tests complete, 
it generates a report listing the status of every executed test along with a list of all failed asserts and runtime errors 
that were encountered. Both cclut_ff and cclut have the following parameters:
 * outputLocation - location to write the output, "MINE" by default.
 * testCaseDirectory - logical name for the directory containing the test case file, "cclsource" by default.
 * testCaseName - (required) the name of the test case file with or without the .inc extension
 * testNamePattern - a regular expression to limit which tests are executed by matching the test name, ".*" by  default.
 * optimizerMode - the optimizer mode (CBO or RBO) to set when running the tests, the current system value by default.
 * deprecatedFlag - the severity level (E,W,I,L,D) if deprecated constructs are encounter when testing, E (error) by default.

Example:

`cclut_ff "MINE", "ccluserdir", "ut_my_script", "testOne" go`

`cclut "MINE", "ccluserdir", "ut_my_script", "", "CBO" go`

## Maven Execution
Maven is a build system that can transfer all of the source and test files to the back end, compile them, execute all of the test
cases, and collect the test results and code coverage data. 

See [ccl-maven-plugin][ccl-maven-plugin] for instructions.

Maven plugins can be configure to generate HTML reports displaying the test 
results and code coverage data which is stored in not particularly legible xml files as well as various other reports. 
See [ccl-testing][ccl-testing] to learn about the reporting plugins.


## Simple Example
### Program
```javascript
drop program ex_cclut_simple_math go
create program ex_cclut_simple_math
/**
    This is a CCL program which performs basic math operations on integers.
    It is used as the subject under test (SUT) for a rudimentary example of CCL Unit.
*/

/*
    @request The expected request structure.
        @field valueOne The first integer in the operation.
        @field valueTwo The second integer in the operation.
        @field operation The operation to perform (case insensitive).
            @value ADD Perform an addition.
            @value SUBTRACT Perform a subtraction.
            @value MULTIPLY Perform a multiplication.
            @value DIVIDE Perform a division.
    record request(
      1 valueOne = i4
      1 valueTwo = i4
      1 operation = vc
    )
*/
 
/**
    @reply The expected reply structure.
        @field result The result of the operation.        
*/ 
    if(not(validate(reply)))
        record reply
        (
        1 result = i4
%i cclsource:status_block.inc
        )
    endif
 

/**
    Adds two integers.
    @param pNumOne The base value.
    @param pNumTwo The value to be added to the base value.
    @returns The result of the addition.
*/
subroutine (public::addValues(pNumOne = i4, pNumTwo = i4) = i4)
    return(pNumOne + pNumTwo)
end
 
/**
    Subtracts two integers.
    @param pNumOne The base value.
    @param pNumTwo The value to be subtracted from the base value.
    @returns The result of the subtraction.
*/
subroutine (public::subtractValues(pNumOne = i4, pNumTwo = i4) = i4)
    return(pNumOne - pNumTwo)
end
 
/**
    Multiplies two integers.
    @param pNumOne The base value.
    @param pNumTwo The value to multiply the base value by.
    @returns The result of the multiplication.
*/
subroutine (public::multiplyValues(pNumOne = i4, pNumTwo = i4) = i4)
    return(pNumOne * pNumTwo)
end
 
/**
    Divides two integers.
    @param pNumOne The base value.
    @param pNumTwo The value to divide the base value by.
    @returns The result of the division.
*/
subroutine (public::divideValues(pNumOne = i4, pNumTwo = i4) = i4)
    return(pNumOne / pNumTwo)
end

/**
    The main logic of the program.
*/
subroutine (public::main(null) = null)
    set reply->status_data.status = 'S'
    case (cnvtupper(request->operation))
    of "ADD":
        set reply->result = addValues(request->valueOne, request->valueTwo)
    of "SUBTRACT":
        set reply->result = subtractValues(request->valueOne, request->valueTwo)
    of "MULTIPLY":
        set reply->result = multiplyValues(request->valueOne, request->valueTwo)
    of "DIVIDE":
        if (request->valueTwo = 0)
            set reply->status_data.status = 'F'
            set reply->status_data.subeventstatus[1].OperationName = "DIVIDE"
            set reply->status_data.subeventstatus[1].OperationStatus = "F"
            set reply->status_data.subeventstatus[1].targetObjectName = "DIVIDE"
            set reply->status_data.subeventstatus[1].targetObjectValue = "Division by Zero"
            go to exit_script
        endif
        set reply->result = divideValues(request->valueOne, request->valueTwo)
    else
        set reply->status_data.status = 'F'
        set reply->status_data.subeventstatus[1].targetObjectName = request->operation
        set reply->status_data.subeventstatus[1].targetObjectValue = "Unrecognized Operation"
    endcase
end

    call main(null) 

#exit_script

end go
```
### Test Case
```javascript
/**
    ut_ex_cclut_simple_math.inc
    An example test case for testing ex_cclut_simple_math using CCL Unit.
*/

record simpleMathRequest(
    1 valueOne = i4
    1 valueTwo = i4
    1 operation = vc
)

record simpleMathReply(
    1 result = i4
%i cclsource:status_block.inc
)


/**
    initialization performed before each test executes.
*/
subroutine (setup(null) = null)
    declare stat = i4 with protect, noconstant(0)
    set stat = initrec(simpleMathRequest)
    set stat = initrec(simpleMathReply)
end ;setup


/**
    clean performed after each test completes. 
    the rollback is gratuitous in this example, but is generally a good idea to undo any updates made by a unit test.
*/
subroutine (teardown(null) = null)
    rollback
end ;teardown
 
 
/**
    Verifies add works as expected.
*/
subroutine (testAdd(null) = null)
    set simpleMathRequest->valueOne = 13
    set simpleMathRequest->valueTwo = 29
    set simpleMathRequest->operation = "ADD"
    
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    
    call cclutAssertI4Equal(CURREF, "testAdd", simpleMathReply->result, 42)
end ;testAdd
 

/**
    Verifies subtract works as expected.
*/ 
subroutine (testSubtract(null) = null)
    set simpleMathRequest->valueOne = 71
    set simpleMathRequest->valueTwo = 29
    set simpleMathRequest->operation = "SUBTRACT"
    
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    
    call cclutAssertI4Equal(CURREF, "testSubtract", simpleMathReply->result, 42)
end ;testSubtract


/**
    Verifies multiply works as expected.
*/ 
subroutine (testMultiply(null) = null)
    set simpleMathRequest->valueOne = 7
    set simpleMathRequest->valueTwo = 6
    set simpleMathRequest->operation = "MULTIPLY"
    
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    
    call cclutAssertI4Equal(CURREF, "testMultiply", simpleMathReply->result, 42)
end ;testMultiply
 
/**
    Verifies divide works as expected.
*/ 
subroutine (testDivide(null) = null)
    set simpleMathRequest->valueOne = 300
    set simpleMathRequest->valueTwo = 7
    set simpleMathRequest->operation = "DIVIDE"
    
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    
    call cclutAssertI4Equal(CURREF, "testDivide", simpleMathReply->result, 42)
end ;testDivide


/**
    Verifies divide by zero works as expected.
*/ 
subroutine (testDivideByZero(null) = null)
    set simpleMathRequest->valueOne = 42
    set simpleMathRequest->valueTwo = 0
    set simpleMathRequest->operation = "DIVIDE"
    
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    
    call cclutAssertVcEqual(CURREF, "/0 status", simpleMathReply->status_data.status, "F")
    call cclutAssertVcEqual(CURREF, "/0 operationName", simpleMathReply->subeventstatus.operationName, "DIVIDE")
    call cclutAssertVcEqual(CURREF, "/0 operationStatus", simpleMathReply->subeventstatus.operationStatus, "F")
    call cclutAssertVcEqual(CURREF, "/0 targetObjectName", simpleMathReply->subeventstatus.targetObjectName, "DIVIDE")
    call cclutAssertVcEqual(CURREF, "/0 targetObjectValue", simpleMathReply->subeventstatus.targetObjectValue, "Division by Zero")
end ;testDivideByZero


/**
    Verifies a failed status is returned when the operation name is unrecognized. 
*/
subroutine (testUnknowOperation(null) = null)
    set simpleMathRequest->valueOne = 42
    set simpleMathRequest->valueTwo = 0
    set simpleMathRequest->operation = "Evade"
    
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    
    call cclutAssertVcEqual(CURREF, "BadOp status", simpleMathReply->status_data.status, "F")
    call cclutAssertVcEqual(CURREF, "BadOp operationName", simpleMathReply->subeventstatus.operationName, "")
    call cclutAssertVcEqual(CURREF, "BadOp operationStatus", simpleMathReply->subeventstatus.operationStatus, "")
    call cclutAssertVcEqual(CURREF, "BadOp targetObjectName", simpleMathReply->subeventstatus.targetObjectName, "Evade")
    call cclutAssertVcEqual(CURREF, "BadOp targetObjectValue", 
            simpleMathReply->subeventstatus.targetObjectValue, "Unrecognized Operation")
end ;testUnknowOperation


/**
    Verifies the operation is case-insensitive.
*/
subroutine (testCharacterCase(null) = null)
    set simpleMathRequest->valueOne = 31
    set simpleMathRequest->valueTwo = 11
    set simpleMathRequest->operation = "add"
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    call cclutAssertI4Equal(CURREF, "add character case", simpleMathReply->result, 42)

    set simpleMathRequest->valueOne = 53
    set simpleMathRequest->operation = "SuBtRaCt"
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    call cclutAssertI4Equal(CURREF, "subtract character case", simpleMathReply->result, 42)
    
    set simpleMathRequest->valueOne = 3
    set simpleMathRequest->valueTwo = 14
    set simpleMathRequest->operation = "mulIply"
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    call cclutAssertI4Equal(CURREF, "subtract character case", simpleMathReply->result, 42)
    
    set simpleMathRequest->valueOne = 128
    set simpleMathRequest->valueTwo = 3
    set simpleMathRequest->operation = "diVide"
    execute ex_cclut_simple_math with replace("REQUEST", SIMPLEMATHREQUEST), replace("REPLY", SIMPLEMATHREPLY)
    call cclutAssertI4Equal(CURREF, "subtract character case", simpleMathReply->result, 42)
end ;testCaseSensitivity
```


[framework-installation]: FRAMEWORKINSTALL.md
[configure-maven]: https://github.com/cerner/ccl-testing/tree/master/doc/CONFIGUREMAVEN.md
[cclutAsserts]: CCLUTASSERTS.md
[ccl-unit-mocking]: CCLUTMOCKING.md
[ccl-maven-plugin]: https://github.com/cerner/ccl-testing/blob/master/ccl-maven-plugin/README.md
[ccl-testing]: https://github.com/cerner/ccl-testing