# CCL Unit Guidance

## Crafting Unit Tests

Before there can be unit testing, there must be units. The ultimate goal is to have small, testable, modular units and to test each of them individually. The goal of this guidance is to describe a way to structure a CCL script that supports testing its units individually. It will not discuss how to create units as there are many other sources that go into that detail.
 
Suppose the body of the script under test has been decomposed into a series of calls to testable subroutines (e.g. "call subroutineA(null)", "call subroutineB(null)", etc.).
 
First off, enclose the body of the script under test in a separate subroutine, say "main(null)".  Next, namespace every subroutine declaration/implementation with the PUBLIC namespace.
 
Let's say we want to test subroutineB in isolation.  Then we will create and execute a unit test named something like "testSubroutineBForScenarioX".  We want our test to execute subroutineB and the descendants of subroutineB. We do not want it to execute any other functions in our script. Here is how to go about it.
 
Use a unique name to define an override for the main subroutine within the test suite, say mainForSubroutineBForScenarioX(null) = null, and have the override first set up the conditions for scenarioX and then have it call subroutineB directly.  Note that the main function also deserves to be tested with all of the functions it calls mocked to ensure that the real implementation for main behaves correctly.
 
The key point here is that the script does not contain a lot of loose code that gets executed every time the script executes. It all gets bundled into the main subroutine which is what allows a unit test to dictate exactly how much of the script's code will be executed.

An illustration of the concept can be seen [here](./examples/basic_example.inc). 

## Mocking Things

In general, there are two ways to mock objects in CCL unit tests.  Generally speaking, use "with replace" to mock things that are defined outside the script or called directly by the script (CCL subroutines, UARs, other scripts), and use "with curnamespace" to mock subroutines executed by the subroutine being tested.  When using "with curnamespace", add the PUBLIC namespace to the real thing and use an alternate namespace to define an override.  Execute the script using the option `with curnamespace = "<alternate namespace>".`  In practice, it is convenient to use the name of the test for the alternate namespace.
    
The CCL Unit Testing framework provides an abstraction for creating mocks.  The purpose is to make it easier to define mock tables and other mock objects to be used when executing a script.  Details on the API can be found at [CCL Unit Mocking][CCL Unit Mocking].

[These unit tests](./examples/mocking_api.inc) demonstrate how to accomplish a basic "with replace" while using the [CCL Unit Mocking Framework][CCL Unit Mocking].  They leverage a script named "mock_other_script" to mock the behavior of "other_script" and test "the_script" in scenarios where "other_script" returns 0 items, more than 5 items and a failed ("F") status.

There are other variations on this.  For example, you could put asserts within mock_other_script itself.  Additionally, other_script might generate its own reply structure, so you would want to do the same in mock_other_script.

[Here](./examples/validation_subroutine.inc) is an example where the mock script calls a validation subroutine defined from the testing subroutine.  It tests that other_script is called exactly 3 times with the correct parameters each time.

Finally, a [namespace example](./examples/using_namespaces.inc) where the getPersonName subroutine is mocked to always return the same value.

## Commit and Rollback

`commit` and `rollback` are keywords within CCL that apply the respective commands to the RDBMS.  This can be particularly annoying when dealing with real tables, especially if one is testing insert/update/delete functionality.  The table mocking API helps mitigate this by using separate custom tables, but it may still be advantageous to separate any usages of `commit/rollback` into their own subroutines which can be mocked with no-op subroutines.

[CCL Unit Mocking]:./CCLUTMOCKING.md