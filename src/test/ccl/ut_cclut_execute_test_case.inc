/**
  Unit Tests for cclut_execute_test_case.
  
  Developers. 
  Please note that cclut_execute_test_case is just a pass through to cclut_execute_test_case_file.
  The only things that need to be tested are that 
    - cclut_execute_test_case input values are properly set on the cclut_execute_test_case_file structure.
    - cclut_execute_test_case_file output values are properly set on the cclut_execute_test_case ouput structure.
  There are some additional tests only to improve code coverage.
*/

%i cclsource:cclut_xml_functions.inc

%i cclsource:ut_cclut_execute_tc_comm.inc
    
record ucetc_request (
  1 testINCName = vc
  1 programs[*]
    2 programName = vc
    2 compile = i2
  1 optimizerMode = vc
  1 enforcePredeclare = i2
  1 deprecatedFlag = vc
  1 testSubroutineName = vc
) with protect

record ucetc_reply (
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

declare testing::sut = vc with protect, constant("cclut_execute_test_case")
declare cclut::debug = i2 with protect, constant(true)
 