drop program cclut_execute_test_case:dba go
create program cclut_execute_test_case:dba
/**
  Legacy entry point for the CCL Testing Framework refactored to wrap cclut_execute_test_case_file. 
  Consumers are expected to supply the cclutRequest and cclutReply structures.
*/

declare cclut::stat = i4 with protect, noconstant(0)
declare cclut::index = i4 with protect, noconstant(0)
declare cclut::count = i4 with protect, noconstant(0)
 
/**
  The primary input.
  
  @request
  @field testINCName
    The name of the test include file to be executed.
  @field programs
    A list of programs whose code coverage is to be recorded after each test execution.
    @field programName
      The name of the program whose code coverage data is to be retrieved.
    @field compile
      A boolean flag indicating whether the program should be compiled in debug mode.
      This is mostly useful when performing multiple tests against the same program. In such situations, 
      set this to 1 on the first execution and then 0 in subsequent executions.
      @value 0 Do not compile the program in debug mode.
      @value 1 Compile the program in debug mode.
  @field optimizerMode
    The Oracle optimizer mode under which the tests should be executed.
  @field enforcePredeclare
    A boolean indicating whether <tt>SET MODIFY PREDECLARE</tt> should be issued prior to executing the test case.
    @value 0 
      No predeclaration enforcement will be used.
    @value 1 
      All variables encountered during test execution are required to be declared prior to use.
  @field testSubroutineName
    An optional filter to limit which tests are executed by case insensitive regular expression matching on the testname.

  record cclutRequest (
    1 testINCName = vc
    1 programs[*]
      2 programName = vc
      2 compile = i2
    1 optimizerMode = vc
    1 enforcePredeclare = i2
    1 deprecatedFlag = vc
    1 testSubroutineName = vc
  ) with protect
*/

/**
  The primary output.

  @reply
  @field environmentXml
    An XML string containing information about the environment in which the test was executed.
  @field testINCListingXml
    The listing output - the result of compilation - of the provided test include file.
  @field testINCCoverageXml
    Xml of the coverage of the coverage of the test.
  @field testINCResultsXml
    An XML document containing the results of the test include file execution.
  @field programs
    A list of the programs that were requested to be checked for code coverage by the test execution.
    @field programName
      The name of the program.
    @field listingXml
      An XML document containing the listing output - the result of compilation - of the program.
    @field coverageXml
      An XML document containing the code coverage data for this program.

  record cclutReply (
    1 environmentXml = vc
    1 testINCListingXml = vc
    1 testINCCoverageXml = vc
    1 testINCResultsXml = vc
    1 programs[*]
      2 programName = vc
      2 listingXml = vc
      2 coverageXml = vc
  %i cclsource:status_block.inc
  ) with protect
*/


/**
  The primary input for cclut_execte_test_case_file. See cclut_execte_test_case_file for documentation.
*/
record cclut_request (
  1 testCaseDirectory = vc
  1 testCaseFileName = vc
  1 testNamePattern = vc
  1 programs[*]
    2 programName = vc
    2 compile = i2
  1 optimizerMode = vc
  1 enforcePredeclare = i2
  1 deprecatedFlag = vc
  1 legacyResultsFormat = i2
) with protect

/**
  The primary output for cclut_execte_test_case_file. See cclut_execte_test_case_file for documentation.
*/
record cclut_reply (
  1 environmentXml = vc
  1 listingXml = vc
  1 coverageXml = vc
  1 resultsXml = vc
  1 programs[*]
    2 programName = vc
    2 listingXml = vc
    2 coverageXml = vc
%i cclsource:status_block.inc
) with protect


;set the codecover trace here because CCL will error when the framework sets it if the session or top-level script did not.
if (curimage = "CCL")
  set trace codecover 0; summary
endif
set modify maxvarlen 10000000

set cclut_request->testCaseDirectory = "CCLSOURCE"
set cclut_request->testCaseFileName = validate(cclutRequest->testIncName, "")
set cclut_request->testNamePattern = validate(cclutRequest->testSubroutineName, "")
set cclut_request->optimizerMode = validate(cclutRequest->optimizerMode, "")
set cclut_request->enforcePredeclare = validate(cclutRequest->enforcePredeclare, TRUE)
set cclut_request->deprecatedFlag = validate(cclutRequest->deprecatedFlag, "E")
set cclut_request->legacyResultsFormat = TRUE
set cclut::count = size(cclutRequest->programs, 5)
set cclut::stat = alterList(cclut_request->programs, cclut::count)
for (cclut::index = 1 to cclut::count)
  set cclut_request->programs[cclut::index].programName = cclutRequest->programs[cclut::index].programName
  set cclut_request->programs[cclut::index].compile = cclutRequest->programs[cclut::index].compile
endfor

execute cclut_execute_test_case_file with 
    replace("CCLUTREQUEST", cclut_request), 
    replace("CCLUTREPLY", cclut_reply),
    replace("CCLUTTESTCASERESULTS", cclutTestCaseResults)

set cclutReply->environmentXml = cclut_reply->environmentXml
set cclutReply->testIncListingXml = cclut_reply->listingXml
set cclutReply->testIncCoverageXml = cclut_reply->coverageXml
set cclutReply->testIncResultsXml = cclut_reply->resultsXml
set cclut::count = size(cclut_reply->programs, 5)
set cclut::stat = alterList(cclutReply->programs, cclut::count)
for (cclut::index = 1 to cclut::count)
  set cclutReply->programs[cclut::index].programName = cclut_reply->programs[cclut::index].programName
  set cclutReply->programs[cclut::index].listingXml = cclut_reply->programs[cclut::index].listingXml
  set cclutReply->programs[cclut::index].coverageXml = cclut_reply->programs[cclut::index].coverageXml
endfor
set cclutReply->status_data.status = cclut_reply->status_data.status
set cclutReply->status_data.subeventstatus[1].operationName = cclut_reply->status_data.subeventstatus[1].operationName
set cclutReply->status_data.subeventstatus[1].operationStatus = cclut_reply->status_data.subeventstatus[1].operationStatus
set cclutReply->status_data.subeventstatus[1].targetObjectName = cclut_reply->status_data.subeventstatus[1].targetObjectName
set cclutReply->status_data.subeventstatus[1].targetObjectValue = cclut_reply->status_data.subeventstatus[1].targetObjectValue

if (validate(cclut::debug, FALSE) = TRUE)
  call echorecord(cclutReply)
endif
 
end go
