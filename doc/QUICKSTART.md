# CCL Unit Quick Start Guide

## Prerequisites
The CCL Unit framework must be installed in the HNAM environment where the testing will be performed. For information on 
checking this, [look here][framework-installation]. 

## Anatomy of CCL Unit
A ***unit test*** is a null parameter subroutine whose name begins with "test".  
A ***test case*** is an include file with a ***.inc*** extension which contains unit tests.  

CCL Unit generates a \[temporary\] program from a test case and then executes the unit tests defined within that test case file.

A ***setupOnce*** is a null parameter subroutine named setupOnce.  
&nbsp; &bull; If a test case contains a setupOnce, CCL Unit will execute it before executing any of the unit tests in the test case.   
&nbsp; &bull; Use a setupOnce to perform any preparation steps that need performed one time before executing all the tests.  
A ***setup*** is a null parameter subroutine named setup.  
&nbsp; &bull; If a test case contains a setup, CCL Unit will execute it just before each unit test in the test case is executed.  
&nbsp; &bull; Use a setup to perform any preparation steps that need to be freshly repeated before each test in a case is executed.  
A ***teardown*** is a null parameter subroutine named teardown.  
&nbsp; &bull; If a test case contains a teardown, CCL Unit will execute it just after each unit test in the case finishes executing.  
&nbsp; &bull; Use a teardown to perform any cleanup steps (such as rollback) that need to be repeated after each test completes.  
A ***teardownOnce*** is a null parameter subroutine named teardownOnce.  
&nbsp; &bull; If a test case contains a teardownOnce, CCL Unit will execute it after all of the tests in the test case have finished executing.  
&nbsp; &bull; Use a teardownOnce to perform any final cleanup steps after all the tests have completed.  
An ***assert*** is a statement asserting a particular condition is true.  
&nbsp; &bull; Example:  `call cclutAssertVcEqual(CURREF,"expected hello world", some_str,"hello world")`  
&nbsp; &bull; For details about asserts and a list of all the avaiable asserts [look here][cclutAsserts]  
A ***timer*** asserts the elapse between two execution points does not exceed a specified threshold.  
&nbsp; &bull; Start a timer named "timer name" by invoking `call cclutStartTimer("timer name", CURREF)`  
&nbsp; &nbsp; &nbsp; &bull;  An error will occur if there already is a timer with the same name.  
&nbsp; &bull; To complete the timer invoke `call cclutEndTimer("timer name", threshold)`  

A unit test will fail if it has an assert that fails or a timer that does not meet its threshold or if a CCL error occurs while the test is executing. It will pass otherwise.  


## Manual Execution
During devlopment it is desirable to perform the test phase of the modify/test/repeat cycle as swiftly as possible. 
Manually executing a single test provides for this and avoids the time overhead of the maven execution described farther down.

---
- Transfer your test case file to the back end.
- excute cclut_ff or cclut
- Examine the output
- Repeat
 - If the unit test is modified, transfer the test case again.
 - If the testing target (script) is modified, recompile the script.

cclut_ff and cclut are prompt programs which generate a temporary program from a test case and execute its tests.  

The cclut_ff program fails fast, i.e., stops executing tests as soon as it encounters an assert failure or a runtime error. 
It indicates whether the tests executed successfully and if not, indicates the assert failure or error that was encountered.  
In contrast cclut continues executing tests even after it encounters assert failures or errors. It generates a report listing the status of all executed tests 
along with the list of all failed asserts and runtime errors that were encountered. Both cclut_ff and cclut have the following parameters:
 * outputLocation - location to write the output, "MINE" by default.
 * testCaseDirectory - logical name for the directory containing the test case file, "cclsource" by default.
 * testCaseName - (required) the name of the test case file with or without the .inc extension
 * testNamePattern - a regular expression to limit which tests are executed by matching the test name, ".*" by  default.
 * optimizerMode - the optimizer mode (CBO or RBO) to set when running the tests, the system's default value by default.
 * deprecatedFlag - the severity level (E,W,I,L,D) if deprecated constructs are encounter when testing, E (error) by default.

Example:

`cclut_ff "MINE", "ccluserdir", "ut_my_script", "testOne" go

`cclut "MINE", "ccluserdir", "ut_my_script", "", "CBO" go

## Maven Execution
[Install and configure maven][configure-maven].

- Execute `mvn archetype:generate -Dfilter=com.cerner.ccl.archetype:cclunit-archetype` to generate an empty project
- Put the source for your CCL program(s) in the src/main/ccl folder.
- Put your test case files in the src/test/ccl folder.
- Execute `mvn clean test -P<profile-name>`


[framework-installation]:https://github.com/cerner/ccl-testing/blob/master/doc/FRAMEWORK.md
[configure-maven]:CONFIGUREMAVEN.md
[cclutAsserts]:CCLUTASSERTS.md
