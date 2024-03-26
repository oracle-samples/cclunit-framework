# cclunit-framework Change Log
## 3.5 - ????

* (non-passive) Use lax string logic only if an i2 variable cclut::useLaxStringLogic with value true is declared by the test case or if that variable (which is 
declare by theframe framework with value false is not declared by the test case) is set equal to true. 


## 3.4 - 2019.03.21

* Fix [#20](https://github.com/cerner/cclunit-framework/issues/20): Fix error handling in the mocking framework.
* Provide the ablity to compile a test case object independent of executing it.
* Create cclut_generate_test_case providing the ability to generate a test case from a compiled program.

## 3.3 - 2019.03.21
* Fix [#17](https://github.com/cerner/cclunit-framework/issues/17): Table mocking fails if some field shares the same name as the table being mocked.
* Fix [#18](https://github.com/cerner/cclunit-framework/issues/18): The CCL definitions for mock tables do not get dropped if the test user does not have DBA privileges.

## 3.2 - 2019.02.25
* Add first class fail fast support in the framework and the cclut prompt program.

## 3.1 - 2019.02.15
* Incorporate the mocking framework.

## 3.0.1 - 2019.02.10
* Fix [#8](https://github.com/cerner/cclunit-framework/issues/8): cclut_find_unit_tests fails to find any unit tests if the OS is aix64.

## 3.0 - 2018.07.09
* Requires CCL version 8.12.0 or higher.

### Enhancements
* Update the framework to be 64-bit compatible.

## 2.0 - 2018.07.09
* Requires CCL version 8.9.3 or higher.

### Additions
* Add support for "operator asserts" using CCL's operator function.
* Add support for setupOnce and teardownOnce functions.
* Create the prompt programs cclut and cclut_ff for executing CCL unit tests directly from CCL or Discern Visual Developer.
* Create CCL Unit unit tests for the CCL Unit Framework to test itself.

### Corrections
* Prevent the framework from failing to log the asserts called from a unit test that calls go to exit_script.
* Escape CDATA tags within CDATA tags allowing the framework to test source code which contains a 
 CDATA tag in a comment or a literal string. This also opens the door for the framework to test itself.
* Update the framework to be able to identify unit test subroutines defined using CCL's in-line subroutine declaration syntax.
* Prefix all framework subroutine parameter names with "cclut" in order to prevent conflicts with testing targets and unit tests.
* Update the framework to use namespaces to better insulate testing targets from variables and subroutines defined by the framework.

### Enhancements
* Update the framework to support disabling code coverage for specific lines of code.
* Update the framework to support specifying the directory where the unit tests are located.
* Update the framework to support changing the deprecated severity which is E (error) by default.
* Update the framework to support changing the default behavior of producing an error if undeclared variables are used.
* Update cclut_get_framework_state to display the framework version and required minimum CCL version if executed directly from CCL or Discern Visual Devleoper.
* Refactor the framework to make it self-testable.

