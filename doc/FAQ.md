### Frequently Asked Questions
1. [There are errors about entities and functions that are not referenced by my program or its tests. What's up?](#there-are-errors-about-entities-and-functions-that-are-not-referenced-by-my-program-or-its-tests-whats-up)  
1. [My tests used to work. Now there are numerous errors. What's going on?](#my-tests-used-to-work-now-there-are-numerous-errors-whats-going-on)  
    - [Version 3.5](#version_3_5)
    - [Authentication](#authentication)
1. [Where do all these %CCL-E-151 errors come from and how do I avoid them? <br/>(variable not previously defined)](#where-do-all-these-ccl-e-151-errors-come-from-and-how-do-i-avoid-them-brvariable-not-previously-defined)  


### There are errors about entities and functions that are not referenced by my program or my tests. What's up?
Mistakes in a test can manifest as errors deeper in the testing framework. The quickest approach to a 
resolution is probably divide and conquer. Work on only one test at a time. Remove the last half of its code....

### My tests used to work. Now there are numerous errors. What's going on?
- #### <a name="version_3_5"></a>Was the framework recently upgraded to version 3.5?
    - Prior to version 3.5 strings with completly different content would be considered equal but that is no longer the case, 
    most notably `""` vs. `" "` and `""` vs. `trim("")` but also `"hello"` vs. `"hello "`, for example. 
    It did that because CCL votes that way.
    - We recommend fixing your string assert calls to be honest, but we recognize there may be just too many of them to
    deal with right now. If there is a Boolean variable (i2) named cclut::useCclStringLogic that is in scope with value TRUE, then 
    the string asserts will apply CCL's string logic. 
- #### <a name="authentication"></a>Do your tests require an authenticated CCL session to work correctly?
    - The answer is yes if the program or the tests call TDBEXECUTE or make UAR calls.
    - If the answer is yes, confirm that the CCL session is authenticated. 
        - This should not be an issue when running tests via Discern Visual Developer
        - It could be a problem when running in a back-end CCL session or via maven if current, valid domain credentials are not configured.
    - Consider using [mock implementations](./CCLUTGUIDANCE.md#mocking-things) to remove the need for an authenticated session
    - Consider adding an [authentication check](./CCLUTGUIDANCE.md#authentication) to avoid confusing error messages if the need cannot be removed.


### Where do all these %CCL-E-151 errors come from and how do I avoid them? <br/>(variable not previously defined)

CCL Unit flags undeclared variables by default whether the infraction occurs in a test, in the program being tested or in 
a child program called by the program being tested. CCL Unit has no way to distinguish between the three cases.

Note that this is merely an early warning system. CCL Unit will fail to find undeclared variable usages masked 
by variables delared in the unit tests or their test cases. As such, static analyses should still be performed 
to thoroghly check for undeclared variable usages. 


The best course of action is to declare all variables in both the unit tests, which should be treated with the same quality 
mindset as production code, and in the program being tested. This might not be an option for child programs, 
and issue volume may prohibit addressing all problems in a legacy program. 
Note that the errors can be completely avoided for child programs by using [mock programs](./CCLUTGUIDANCE.md#mocking-things). 

The next best course of action is to declare each variable as close as possible to the point it gets used. If necessary this can be done
in the test or its test case but it would be best to do so in the program under test.

It is not advised to do so but the check for undeclared variables can be turned off via the deprecatedFlag. 
See [CCL Unit Tests](https://github.com/cerner/cclunit-framework/blob/master/doc/CCLUNITTESTS.md#manual-execution) 
and [ccl-maven-plugin options](https://github.com/cerner/ccl-testing/blob/master/ccl-maven-plugin/doc/CONFIGURATIONOPTIONS.md#deprecatedFlag). 
The trouble with this approach is that it completely disables undeclared variable checking and it also prevents CCL's built in checks for bad coding styles.







