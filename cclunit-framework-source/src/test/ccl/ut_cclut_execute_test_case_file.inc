/**
  Unit Tests for cclut_execute_test_case_file.
*/

%i cclsource:cclut_xml_functions.inc
%i cclsource:cclut_code_coverage.inc


declare setup(null) = null with protect
declare validateEnvironmentXml(xml = vc) = null with protect
declare validateListingXml(xml = vc) = null with protect
declare validateResultsXml(xml = vc, legacyFormat = i2) = null with protect
declare validateCoverageXml(coverage = vc) = null with protect
declare validateNoIssuesResponse(response = vc(ref)) = null with protect
declare testNoIssues(null) = null with protect
declare testNoIssuesLegacy(null) = null with protect
declare testNoIssuesCBO(null) = null with protect
declare testNoIssuesRBO(null) = null with protect
declare testNoIssuesInvalidOM(null) = null with protect
declare testErrorsEverywhere(null) = null with protect
declare testFailsAndErrorsEverywhere(null) = null with protect
declare testFailsEverywhere(null) = null with protect
declare testFailsTestsOnly(null) = null with protect
declare testSetupErrors(null) = null with protect
declare testLoadingErrors(null) = null with protect
declare testTestCaseFileDoesNotExist(null) = null with protect
declare testCclMinimumNotMet(null) = null with protect
declare cclut::getOutdatedCclVersion(null) = vc with protect
declare testCompileProgramFailure(null) = null with protect
declare cclut::compileProgramFailure(fileDir = vc, fileName = vc, listingDir = vc, listingName = vc, errMsg = vc(ref)) = i2
declare testGenerateTestCaseProgramFailure(null) = null with protect
declare cclut::generateTestCaseProgramFailure(fileDir = vc, fileName = vc, listingDir = vc, listingName = vc, 
    objName = vc, errorMessage = vc(ref)) = i2
declare testCheckPrgFailure(null) = null with protect
declare checkPrgFailure(prgName = vc) = i4 with protect
declare testPostExecutionFailure(null) = null with protect
declare mockRemove(fileLocation = vc) = i4 with protect
declare testErrorThenExitScript(null) = null with protect
declare testCoverageExcludes(null) = null with protect


record ucets_request (
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

record ucets_reply (
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

record ucets_testCaseResults (
  1 resultInd = i2
  1 tests[*]
    2 name = vc
    2 asserts[*]
      3 lineNumber = i4
      3 context = vc
      3 resultInd = i2 ;TRUE = PASS, FALSE = FAIL
      3 condition = vc
    2 errors[*]
      3 lineNumber = i4
      3 errorText = vc
%i cclsource:status_block.inc
) with protect

declare testing::errorSetupOnce = i2 with protect, noconstant(FALSE)
declare testing::errorSetup = i2 with protect, noconstant(FALSE)
declare testing::errorTeardown = i2 with protect, noconstant(FALSE)
declare testing::errorTest = i2 with protect, noconstant(FALSE)
declare testing::errorTeardownOnce = i2 with protect, noconstant(FALSE)

declare testing::failSetupOnce = i2 with protect, noconstant(FALSE)
declare testing::failSetup = i2 with protect, noconstant(FALSE)
declare testing::failTeardown = i2 with protect, noconstant(FALSE)
declare testing::failTest = i2 with protect, noconstant(FALSE)
declare testing::failTeardownOnce = i2 with protect, noconstant(FALSE)


/**
  per-test initialization
*/
subroutine setup(null)
  declare stat = i4 with protect, noconstant(0)
  set stat = initRec(ucets_request)
  set stat = initRec(ucets_reply)
  set stat = initRec(ucets_testCaseResults)

  set ucets_request->testCaseDirectory = "cclsource"
  set ucets_request->testCaseFileName = "ut_cclut_suo_su_td_tdo"

  set testing::errorSetupOnce = FALSE
  set testing::errorSetup = FALSE
  set testing::errorTeardown = FALSE
  set testing::errorTest = FALSE
  set testing::errorTeardownOnce = FALSE

  set testing::failSetupOnce = FALSE
  set testing::failSetup = FALSE
  set testing::failTeardown = FALSE
  set testing::failTest = FALSE
  set testing::failTeardownOnce = FALSE
end ;;;setup


/**
  Validates that an environmentXml matches the expected enviromentXml. 
*/
subroutine validateEnvironmentXml(xml)
  declare xmlItem = vc with protect, noconstant("")
  declare itemFound = i2 with protect, noconstant(FALSE)

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDB", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact currdb", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdb", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDBNAME", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact currdbname", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdbname", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDBUSER", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact currdbuser", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdbuser", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDBSYS", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact currdbsys", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdbsys", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CCLVER", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact cclver", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate cclver", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURSYS", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact cursys", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate cursys", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURLOCALE", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curlocale", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curlocale", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURUSER", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curuser", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curuser", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURUTC", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curutc", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curutc", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURUTCDIFF", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curutcdiff", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curutcdiff", xmlItem, "")
  
  set xmlItem = cclut::retrieveXmlContent(xml, "CURTIMEZONE", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curtimezone", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curtimezone", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURTIMEZONEAPP", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curtimezoneapp", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curtimezoneapp", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURTIMEZONESYS", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curtimezonesys", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curtimezonesys", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURREVAFD", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact currevafd", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currevafd", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURGROUP", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact curgroup", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curgroup", xmlItem, "")
  
  set xmlItem = cclut::retrieveXmlContent(xml, "DBVERSION", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact dbversion", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate dbversion", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "DBOPTMODE", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extact dboptmode", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate dboptmode", xmlItem, "")
end ;;;validateEnvironmentXml

/**
  Validates that a listingXml matches the expected listing. 
  
  Note: The behavior of this function depends on the specific contents of various framework files 
  so this function must be updated whenever those files are modified.
*/
subroutine validateListingXml(xml)
  declare matches = i2 with protect, noconstant(FALSE)

  call cclutAssertStartsWith(CURREF, "validate listingXml", 
  ~<LISTING><LISTING_NAME>UT_CCLUT_SUO_SU_TD_TDO</LISTING_NAME><COMPILE_DATE>~, xml)

  ;21-NOV-2017 11:06:09.00
  set matches = operator(substring(75, 23, xml), "regexplike", 
    "[0-9]{2}-(JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC)-[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]{2}")
  
  call cclutAssertI4Equal(CURREF, "validate listingXml", matches, TRUE) 

;  call echo(build2("listingXml = ", xml))
  call cclutAssertEndsWith(CURREF, "validate listingXml", 
~</COMPILE_DATE><LINES><LINE><NBR>916</NBR><TEXT><![CDATA[/**]]></TEXT></LINE><LINE><NBR>917</NBR><TEXT><![CDATA[  A test cas\
e file for testing the CCL Unit framework.]]></TEXT></LINE><LINE><NBR>918</NBR><TEXT><![CDATA[  This test case file contains \
all of the testing support subroutines, setupOnce, setup, teardown and teardownOnce and three test]]></TEXT></LINE><LINE><NBR\
>919</NBR><TEXT><![CDATA[  subroutines. Each subroutine fires a successful assert to indicate it executed and based on config\
uration will then fire]]></TEXT></LINE><LINE><NBR>920</NBR><TEXT><![CDATA[  a failed assert and/or cause an error based on th\
e value of the boolean variable]]></TEXT></LINE><LINE><NBR>921</NBR><TEXT><![CDATA[  testing::{fail|error}{SetupOnce|Setup|Te\
st|Teardown|TeardownOnce} which needs to be provided by the invoking test.]]></TEXT></LINE><LINE><NBR>922</NBR><TEXT><![CDATA\
[*/]]></TEXT></LINE><LINE><NBR>923</NBR><TEXT><![CDATA[declare setupOnce(null) = null]]></TEXT></LINE><LINE><NBR>924</NBR><TE\
XT><![CDATA[declare teardownOnce(null) = null]]></TEXT></LINE><LINE><NBR>925</NBR><TEXT><![CDATA[declare setup(null) = null]]\
></TEXT></LINE><LINE><NBR>926</NBR><TEXT><![CDATA[declare testOne(null) = null]]></TEXT></LINE><LINE><NBR>927</NBR><TEXT><![C\
DATA[declare testTwo(null) = null]]></TEXT></LINE><LINE><NBR>928</NBR><TEXT><![CDATA[declare testThree(null) = null]]></TEXT>\
</LINE><LINE><NBR>929</NBR><TEXT><![CDATA[declare teardown(null) = null]]></TEXT></LINE><LINE><NBR>930</NBR><TEXT><![CDATA[]]\
></TEXT></LINE><LINE><NBR>931</NBR><TEXT><![CDATA[declare guaranteedError = i4 with protect, noconstant(0)]]></TEXT></LINE><L\
INE><NBR>932</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>933</NBR><TEXT><![CDATA[if (validate(testing::errorLoading, FALS\
E) = TRUE)]]></TEXT></LINE><LINE><NBR>934</NBR><TEXT><![CDATA[    set guaranteedError = "loading error"]]></TEXT></LINE><LINE\
><NBR>935</NBR><TEXT><![CDATA[endif]]></TEXT></LINE><LINE><NBR>936</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>937</NBR><\
TEXT><![CDATA[subroutine setupOnce(null)]]></TEXT></LINE><LINE><NBR>938</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(CURREF,\
 "setupOnce executed", "SO", "SO")]]></TEXT></LINE><LINE><NBR>939</NBR><TEXT><![CDATA[  if (testing::failSetupOnce)]]></TEXT>\
</LINE><LINE><NBR>940</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "setupOnce failure", 0, 1)]]></TEXT></LINE><LIN\
E><NBR>941</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>942</NBR><TEXT><![CDATA[  if (testing::errorSetupOnce)]]></\
TEXT></LINE><LINE><NBR>943</NBR><TEXT><![CDATA[    set guaranteedError = "setupOnce error"]]></TEXT></LINE><LINE><NBR>944</NB\
R><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>945</NBR><TEXT><![CDATA[end ;setupOnce]]></TEXT></LINE><LINE><NBR>946</NB\
R><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>947</NBR><TEXT><![CDATA[subroutine teardownOnce(null)]]></TEXT></LINE><LINE><NBR\
>948</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(CURREF, "teardownOnce executed", "TO", "TO")]]></TEXT></LINE><LINE><NBR>94\
9</NBR><TEXT><![CDATA[  if (testing::failTeardownOnce)]]></TEXT></LINE><LINE><NBR>950</NBR><TEXT><![CDATA[    call cclutAsser\
tI4Equal(CURREF, "teardownOnce failure", 0, 1)]]></TEXT></LINE><LINE><NBR>951</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LI\
NE><NBR>952</NBR><TEXT><![CDATA[  if (testing::errorTeardownOnce)]]></TEXT></LINE><LINE><NBR>953</NBR><TEXT><![CDATA[    set \
guaranteedError = "teardownOnce error"]]></TEXT></LINE><LINE><NBR>954</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>\
955</NBR><TEXT><![CDATA[end ;teardownOnce]]></TEXT></LINE><LINE><NBR>956</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>957<\
/NBR><TEXT><![CDATA[subroutine setup(null)]]></TEXT></LINE><LINE><NBR>958</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURRE\
F, "setup executed", 0, 0)]]></TEXT></LINE><LINE><NBR>959</NBR><TEXT><![CDATA[  if (testing::failSetup)]]></TEXT></LINE><LINE\
><NBR>960</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "setup failure", 0, 1)]]></TEXT></LINE><LINE><NBR>961</NBR>\
<TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>962</NBR><TEXT><![CDATA[  if (testing::errorSetup)]]></TEXT></LINE><LINE><N\
BR>963</NBR><TEXT><![CDATA[    set guaranteedError = "setup error"]]></TEXT></LINE><LINE><NBR>964</NBR><TEXT><![CDATA[  endif\
]]></TEXT></LINE><LINE><NBR>965</NBR><TEXT><![CDATA[end ;;;setup]]></TEXT></LINE><LINE><NBR>966</NBR><TEXT><![CDATA[]]></TEXT\
></LINE><LINE><NBR>967</NBR><TEXT><![CDATA[subroutine teardown (null)]]></TEXT></LINE><LINE><NBR>968</NBR><TEXT><![CDATA[  ca\
ll cclutAssertI4Equal(CURREF, "teardown executed", 0, 0)]]></TEXT></LINE><LINE><NBR>969</NBR><TEXT><![CDATA[  if (testing::fa\
ilTeardown)]]></TEXT></LINE><LINE><NBR>970</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "teardown failure", 0, 1)]\
]></TEXT></LINE><LINE><NBR>971</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>972</NBR><TEXT><![CDATA[  if (testing::\
errorTeardown)]]></TEXT></LINE><LINE><NBR>973</NBR><TEXT><![CDATA[    set guaranteedError = "teardown error"]]></TEXT></LINE>\
<LINE><NBR>974</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>975</NBR><TEXT><![CDATA[end ;;;setup]]></TEXT></LINE><L\
INE><NBR>976</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>977</NBR><TEXT><![CDATA[subroutine testOne(null)]]></TEXT></LINE\
><LINE><NBR>978</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "testOne executed", 0, 0)]]></TEXT></LINE><LINE><NBR>97\
9</NBR><TEXT><![CDATA[  if (testing::failTest)]]></TEXT></LINE><LINE><NBR>980</NBR><TEXT><![CDATA[    call cclutAssertI4Equal\
(CURREF, "testOne failure", 0, 1)]]></TEXT></LINE><LINE><NBR>981</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>982</\
NBR><TEXT><![CDATA[  if (testing::errorTest)]]></TEXT></LINE><LINE><NBR>983</NBR><TEXT><![CDATA[    set guaranteedError = "te\
stOne error"]]></TEXT></LINE><LINE><NBR>984</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>985</NBR><TEXT><![CDATA[en\
d ;;;testOne]]></TEXT></LINE><LINE><NBR>986</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>987</NBR><TEXT><![CDATA[subroutin\
e testTwo(null)]]></TEXT></LINE><LINE><NBR>988</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "testTwo executed", 0, 0\
)]]></TEXT></LINE><LINE><NBR>989</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(CURREF, "testTwo executed", "A", "A")]]></TEXT\
></LINE><LINE><NBR>990</NBR><TEXT><![CDATA[end ;;;testTwo]]></TEXT></LINE><LINE><NBR>991</NBR><TEXT><![CDATA[]]></TEXT></LINE\
><LINE><NBR>992</NBR><TEXT><![CDATA[;; like testOne, but two failures and two errors and errors come before failures when bot\
h are specified.]]></TEXT></LINE><LINE><NBR>993</NBR><TEXT><![CDATA[subroutine testThree(null)]]></TEXT></LINE><LINE><NBR>994\
</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "testThree executed", 0, 0)]]></TEXT></LINE><LINE><NBR>995</NBR><TEXT>\
<![CDATA[  if (testing::errorTest)]]></TEXT></LINE><LINE><NBR>996</NBR><TEXT><![CDATA[    set guaranteedError = "testThree er\
ror"]]></TEXT></LINE><LINE><NBR>997</NBR><TEXT><![CDATA[    set guaranteedError = "testThree error 2"]]></TEXT></LINE><LINE><\
NBR>998</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>999</NBR><TEXT><![CDATA[  if (testing::failTest)]]></TEXT></LI\
NE><LINE><NBR>1000</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "testThree failure A", 0, 1)]]></TEXT></LINE><LINE\
><NBR>1001</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "testThree failure B", 0, 1)]]></TEXT></LINE><LINE><NBR>10\
02</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1003</NBR><TEXT><![CDATA[end ;;;testThree]]></TEXT></LINE></LINES><\
/LISTING>~, xml)  
end ;;;validateListingXml


/**
  Validates that a resultsXml matches the expected results. The behavior of this function depends on the contents 
  of the ut_cclut_suo_su_td_tdo.inc file so this function must be updated whenever that test case file is modified.
*/
subroutine validateResultsXml(xml, legacyFormat)
  declare foundInd = i2 with protect, noconstant(FALSE)
  declare testCaseName = vc with protect, noconstant("")
  declare xmlTests = vc with protect, noconstant("")
  declare xmlTest = vc with protect, noconstant("")
  declare testName = vc with protect, noconstant("")
  declare xmlAsserts = vc with protect, noconstant("")
  declare xmlAssert = vc with protect, noconstant("")
  declare lineNbr = vc with protect, noconstant("")
  declare assertContext = vc with protect, noconstant("")
  declare condition = vc with protect, noconstant("")
  declare result = vc with protect, noconstant("")
  declare xmlErrors = vc with protect, noconstant("")
  declare nextTestStart = i4 with protect, noconstant(0)
  declare nextAssertStart = i4 with protect, noconstant(0)
  
  call cclutAssertStartsWith(CURREF, "validate resultsXml", "<TESTCASE>", xml)
  call cclutAssertEndsWith(CURREF, "validate resultsXml", "</TESTCASE>", xml)
  set testCaseName = cclut::retrieveXmlContent(xml, "name", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate test case name found", foundInd, TRUE)  
  call cclutAssertVCEqual(CURREF, "validate test case", testCaseName, "ut_cclut_suo_su_td_tdo")  
  
  set xmlTests = cclut::retrieveXmlContent(xml, "tests", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate xmlTests found", foundInd, TRUE)
  
  set xmlTest = cclut::retrieveXmlContent(xmlTests, "test", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate xmlTest found 1", foundInd, TRUE)
  
  set testName = cclut::retrieveXmlContent(xmlTest, "name", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate test name found 1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate test name 1", testName, "SETUPONCE")
  
  set xmlAsserts = cclut::retrieveXmlContent(xmlTest, "asserts", 1, foundInd)
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert found 1.1", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 1.1", lineNbr, "938")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 1.1", assertContext, "setupOnce executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 1.1", condition, ~"SO"="SO"~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 1.1", result, "PASSED")
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", 17+textlen(xmlAssert) , foundInd) ;textlen(<ASSERT></ASSERT>) = 17
  call cclutAssertI4Equal(CURREF, "validate no more asserts 1", foundInd, FALSE)
  set xmlErrors = cclut::retrieveXmlContent(xmlTest, "errors", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate error list found 1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate no errors 1", xmlErrors, "")
  
  set nextTestStart = 13 + textlen(xmlTest) ;textlen(<TEST></TEST>) = 13
  set xmlTest = cclut::retrieveXmlContent(xmlTests, "test", nextTestStart, foundInd)
  call cclutAssertI4Equal(CURREF, "validate xmlTest found 2", foundInd, TRUE)
  
  set testName = cclut::retrieveXmlContent(xmlTest, "name", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate test name found 2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate test name 2", testName, "TESTONE")
  
  set xmlAsserts = cclut::retrieveXmlContent(xmlTest, "asserts", 1, foundInd)
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert found 2.1", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 2.1", lineNbr, "958")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.1", assertContext, "setup executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 2.1", condition, ~0=0~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 2.1", result, "PASSED")
  set nextAssertStart = 17 + textlen(xmlAssert) ;textlen(<ASSERT></ASSERT>) = 17
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", nextAssertStart, foundInd) 
  
  call cclutAssertI4Equal(CURREF, "validate assert found 2.2", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 2.2", lineNbr, "978")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.2", assertContext, "testOne executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 2.2", condition, ~0=0~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 2.2", result, "PASSED")
  
  set nextAssertStart = nextAssertStart + 17 + textlen(xmlAssert)
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", nextAssertStart, foundInd)
  
  call cclutAssertI4Equal(CURREF, "validate assert found 2.3", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 2.3", lineNbr, "968")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.3", assertContext, "teardown executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 2.3", condition, ~0=0~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 2.3", result, "PASSED")
  
  set nextAssertStart = nextAssertStart + 17 + textlen(xmlAssert)
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", nextAssertStart, foundInd)
  
  call cclutAssertI4Equal(CURREF, "validate no more asserts 2", foundInd, FALSE)
  set xmlErrors = cclut::retrieveXmlContent(xmlTest, "errors", 2, foundInd)
  call cclutAssertI4Equal(CURREF, "validate error list found 2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate no errors 2", xmlErrors, "")

  ;testTwo and testThree are similar
  set nextTestStart = nextTestStart + 13 + textlen(xmlTest)
  set xmlTest = cclut::retrieveXmlContent(xmlTests, "test", nextTestStart, foundInd)
  call cclutAssertI4Equal(CURREF, "validate xmlTest found 3", foundInd, TRUE)
  set testName = cclut::retrieveXmlContent(xmlTest, "name", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate test name found 3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate test name 3", testName, "TESTTWO")

  set nextTestStart = nextTestStart + 13 + textlen(xmlTest)
  set xmlTest = cclut::retrieveXmlContent(xmlTests, "test", nextTestStart, foundInd)
  call cclutAssertI4Equal(CURREF, "validate xmlTest found 4", foundInd, TRUE)
  set testName = cclut::retrieveXmlContent(xmlTest, "name", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate test name found 4", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate test name 4", testName, "TESTTHREE")
  
  set nextTestStart = nextTestStart + 13 + textlen(xmlTest)
  set xmlTest = cclut::retrieveXmlContent(xmlTests, "test", nextTestStart, foundInd)
  call cclutAssertI4Equal(CURREF, "validate xmlTest found 5", foundInd, TRUE)
  set testName = cclut::retrieveXmlContent(xmlTest, "name", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate test name found 5", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate test name 5", testName, "TEARDOWNONCE")

  set xmlAsserts = cclut::retrieveXmlContent(xmlTest, "asserts", 1, foundInd)
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert found 5.1", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 5.1", lineNbr, "948")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 5.1", assertContext, "teardownOnce executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 5.1", condition, ~"TO"="TO"~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 5.1", result, "PASSED")
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", 17+textlen(xmlAssert) , foundInd)
  call cclutAssertI4Equal(CURREF, "validate no more asserts 5", foundInd, FALSE)
  set xmlErrors = cclut::retrieveXmlContent(xmlTest, "errors", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate error list found 5", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate no errors 5", xmlErrors, "")

  set nextTestStart = nextTestStart + 13 + textlen(xmlTest)
  set xmlTest = cclut::retrieveXmlContent(xmlTests, "test", nextTestStart, foundInd)
  call cclutAssertI4Equal(CURREF, "validate no more tests", foundInd, FALSE)
end ;;;validateResultsXml

/**
  Validates that a coverageXml matches the expected coverage. Note that the behavior of this function depends on the content 
  of this file so this function must be updated whenever this test case file is modified.
*/
subroutine validateCoverageXml(coverage)
  return(null)

  call cclutAssertVCEqual(CURREF, "validate coverageXml", coverage,
~<COVERAGE><COVERAGE_NAME>UT_CCLUT_SUO_SU_TD_TDO</COVERAGE_NAME><LINES><LINE><NBR>787</NBR><TYPE>N</TYPE></LINE><LINE><NBR>78\
8</NBR><TYPE>N</TYPE></LINE><LINE><NBR>789</NBR><TYPE>N</TYPE></LINE><LINE><NBR>790</NBR><TYPE>N</TYPE></LINE><LINE><NBR>791<\
/NBR><TYPE>N</TYPE></LINE><LINE><NBR>792</NBR><TYPE>N</TYPE></LINE><LINE><NBR>793</NBR><TYPE>N</TYPE></LINE><LINE><NBR>794</N\
BR><TYPE>C</TYPE></LINE><LINE><NBR>795</NBR><TYPE>N</TYPE></LINE><LINE><NBR>796</NBR><TYPE>C</TYPE></LINE><LINE><NBR>797</NBR\
><TYPE>U</TYPE></LINE><LINE><NBR>798</NBR><TYPE>N</TYPE></LINE><LINE><NBR>799</NBR><TYPE>N</TYPE></LINE><LINE><NBR>800</NBR><\
TYPE>N</TYPE></LINE><LINE><NBR>801</NBR><TYPE>C</TYPE></LINE><LINE><NBR>802</NBR><TYPE>C</TYPE></LINE><LINE><NBR>803</NBR><TY\
PE>U</TYPE></LINE><LINE><NBR>804</NBR><TYPE>N</TYPE></LINE><LINE><NBR>805</NBR><TYPE>C</TYPE></LINE><LINE><NBR>806</NBR><TYPE\
>U</TYPE></LINE><LINE><NBR>807</NBR><TYPE>N</TYPE></LINE><LINE><NBR>808</NBR><TYPE>N</TYPE></LINE><LINE><NBR>809</NBR><TYPE>N\
</TYPE></LINE><LINE><NBR>810</NBR><TYPE>N</TYPE></LINE><LINE><NBR>811</NBR><TYPE>C</TYPE></LINE><LINE><NBR>812</NBR><TYPE>C</\
TYPE></LINE><LINE><NBR>813</NBR><TYPE>U</TYPE></LINE><LINE><NBR>814</NBR><TYPE>N</TYPE></LINE><LINE><NBR>815</NBR><TYPE>C</TY\
PE></LINE><LINE><NBR>816</NBR><TYPE>U</TYPE></LINE><LINE><NBR>817</NBR><TYPE>N</TYPE></LINE><LINE><NBR>818</NBR><TYPE>N</TYPE\
></LINE><LINE><NBR>819</NBR><TYPE>N</TYPE></LINE><LINE><NBR>820</NBR><TYPE>N</TYPE></LINE><LINE><NBR>821</NBR><TYPE>C</TYPE><\
/LINE><LINE><NBR>822</NBR><TYPE>C</TYPE></LINE><LINE><NBR>823</NBR><TYPE>U</TYPE></LINE><LINE><NBR>824</NBR><TYPE>N</TYPE></L\
INE><LINE><NBR>825</NBR><TYPE>C</TYPE></LINE><LINE><NBR>826</NBR><TYPE>U</TYPE></LINE><LINE><NBR>827</NBR><TYPE>N</TYPE></LIN\
E><LINE><NBR>828</NBR><TYPE>N</TYPE></LINE><LINE><NBR>829</NBR><TYPE>N</TYPE></LINE><LINE><NBR>830</NBR><TYPE>N</TYPE></LINE>\
<LINE><NBR>831</NBR><TYPE>C</TYPE></LINE><LINE><NBR>832</NBR><TYPE>C</TYPE></LINE><LINE><NBR>833</NBR><TYPE>U</TYPE></LINE><L\
INE><NBR>834</NBR><TYPE>N</TYPE></LINE><LINE><NBR>835</NBR><TYPE>C</TYPE></LINE><LINE><NBR>836</NBR><TYPE>U</TYPE></LINE><LIN\
E><NBR>837</NBR><TYPE>N</TYPE></LINE><LINE><NBR>838</NBR><TYPE>N</TYPE></LINE><LINE><NBR>839</NBR><TYPE>N</TYPE></LINE><LINE>\
<NBR>840</NBR><TYPE>N</TYPE></LINE><LINE><NBR>841</NBR><TYPE>C</TYPE></LINE><LINE><NBR>842</NBR><TYPE>C</TYPE></LINE><LINE><N\
BR>843</NBR><TYPE>U</TYPE></LINE><LINE><NBR>844</NBR><TYPE>N</TYPE></LINE><LINE><NBR>845</NBR><TYPE>C</TYPE></LINE><LINE><NBR\
>846</NBR><TYPE>U</TYPE></LINE><LINE><NBR>847</NBR><TYPE>N</TYPE></LINE><LINE><NBR>848</NBR><TYPE>N</TYPE></LINE><LINE><NBR>8\
49</NBR><TYPE>N</TYPE></LINE><LINE><NBR>850</NBR><TYPE>N</TYPE></LINE><LINE><NBR>851</NBR><TYPE>C</TYPE></LINE><LINE><NBR>852\
</NBR><TYPE>C</TYPE></LINE><LINE><NBR>853</NBR><TYPE>N</TYPE></LINE><LINE><NBR>854</NBR><TYPE>N</TYPE></LINE><LINE><NBR>855</\
NBR><TYPE>N</TYPE></LINE><LINE><NBR>856</NBR><TYPE>C</TYPE></LINE><LINE><NBR>857</NBR><TYPE>C</TYPE></LINE><LINE><NBR>858</NB\
R><TYPE>U</TYPE></LINE><LINE><NBR>859</NBR><TYPE>U</TYPE></LINE><LINE><NBR>860</NBR><TYPE>N</TYPE></LINE><LINE><NBR>861</NBR>\
<TYPE>C</TYPE></LINE><LINE><NBR>862</NBR><TYPE>U</TYPE></LINE><LINE><NBR>863</NBR><TYPE>U</TYPE></LINE><LINE><NBR>864</NBR><T\
YPE>N</TYPE></LINE><LINE><NBR>865</NBR><TYPE>N</TYPE></LINE></LINES></COVERAGE>~)
end ;;;validateCoverageXml

/**
  Helper function for validating the common response values of the multiple "no issues" tests
*/
subroutine validateNoIssuesResponse(response)
  declare testCount = i4 with protect, noconstant(0)
  
  call cclutAssertI2Equal(CURREF, "test case success", response->resultInd, TRUE)

  set testCount = size(response->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      response->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      response->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[2].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[2].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[2].asserts[3].resultInd, TRUE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[4].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[4].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[4].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[4].asserts[3].resultInd, TRUE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      response->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      response->tests[5].asserts[1].resultInd, TRUE)
end ;;;validateNoIssuesResponse

/**
  Run ut_cclut_suo_su_td_tdo and validate the results when no issues occur.
*/
subroutine testNoIssues(null)
  declare stat = i4 with protect, noconstant(0)
  
  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  if (validate(cclut_request) = TRUE)
    ;to capture code coverage if running from maven
    set stat = moverec(cclut_request->programs, ucets_request1->programs)
    if (size(cclut_request->programs, 5) > 0)
      set cclut_request->programs[1].compile = TRUE
    endif
  endif
  set ucets_request1->enforcePredeclare = TRUE
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucets_testCaseResults)

  call validateEnvironmentXml(ucets_reply->environmentXml)
  call validateListingXml(ucets_reply->listingXml)
  call validateResultsXml(ucets_reply->resultsXml, FALSE)
  call validateCoverageXml(ucets_reply->coverageXml)
end ;;;testNoIssues 


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when no issues occur with legacy format requested.
*/
subroutine testNoIssuesLegacy(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  if (validate(cclut_request) = TRUE)
    ;to capture code coverage if running from maven
    set stat = moverec(cclut_request->programs, ucets_request1->programs)
  endif
  set ucets_request1->legacyResultsFormat = TRUE
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
  
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucets_testCaseResults)
  
  call validateEnvironmentXml(ucets_reply->environmentXml)
  call validateListingXml(ucets_reply->listingXml)
  call validateResultsXml(ucets_reply->resultsXml, TRUE)
  call validateCoverageXml(ucets_reply->coverageXml)
end ;;;testNoIssuesLegacy


/**
  Run ut_cclut_suo_su_td_tdo and validate the results with optimizer mode CBO when no issues occur.
*/
subroutine testNoIssuesCBO(null)
  declare stat = i4 with protect, noconstant(0)
  
  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucets_request1->optimizerMode = "CBO"
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
  
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucets_testCaseResults)
end ;;;testNoIssuesCBO

/**
  Run ut_cclut_suo_su_td_tdo and validate the results with optimizer mode RBO when no issues occur.
*/
subroutine testNoIssuesRBO(null)
  declare stat = i4 with protect, noconstant(0)
  
  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucets_request1->optimizerMode = "RBO"
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
  
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucets_testCaseResults)
end ;;;testNoIssuesRBO


/**
  Run ut_cclut_suo_su_td_tdo and validate the results with an invalid optimizer mode when no issues occur.
*/
subroutine testNoIssuesInvalidOM(null)
  declare stat = i4 with protect, noconstant(0)
  
  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucets_request1->optimizerMode = "invalid"
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 5", 
      ucets_reply->status_data.subeventstatus[1].targetObjectValue, "Invalid optimizer mode specified.")  
end ;;;testNoIssuesInvalidOM


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce an error.
*/
subroutine testErrorsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetupOnce = TRUE
  set testing::errorSetup = TRUE
  set testing::errorTest = TRUE
  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE
 
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)

  set testCount = size(ucets_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[1].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucets_testCaseResults->tests[1].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[1].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[2].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[2].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucets_testCaseResults->tests[2].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucets_testCaseResults->tests[2].errors[errorIdx].errorText)
  endfor
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[3].errors, 5), 2)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  for (errorIdx = 1 to 2)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucets_testCaseResults->tests[3].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucets_testCaseResults->tests[3].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[4].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[4].errors, 5), 4)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  for (errorIdx = 1 to 4)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucets_testCaseResults->tests[4].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucets_testCaseResults->tests[4].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[5].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucets_testCaseResults->tests[4].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[4].errors[1].errorText)
end ;;;testErrorsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce a failed assert and an error.
*/
subroutine testFailsAndErrorsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetupOnce = TRUE
  set testing::errorSetup = TRUE
  set testing::errorTest = TRUE
  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE

  set testing::failSetupOnce = TRUE
  set testing::failSetup = TRUE
  set testing::failTest = TRUE
  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE
 
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)

  set testCount = size(ucets_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[1].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[1].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[2].context, "setupOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[2].resultInd, FALSE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucets_testCaseResults->tests[1].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[1].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[2].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[2].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[4].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[6].resultInd, FALSE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucets_testCaseResults->tests[2].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucets_testCaseResults->tests[2].errors[errorIdx].errorText)
  endfor
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[3].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[3].errors, 5), 2)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[4].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[6].resultInd, FALSE)
  for (errorIdx = 1 to 2)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucets_testCaseResults->tests[3].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucets_testCaseResults->tests[3].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[4].asserts, 5), 7)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[4].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[4].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[5].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[5].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[6].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[6].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[7].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[7].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[7].resultInd, FALSE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucets_testCaseResults->tests[4].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucets_testCaseResults->tests[4].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[5].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[5].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[2].context, "teardownOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[2].resultInd, FALSE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucets_testCaseResults->tests[5].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[5].errors[1].errorText)
end ;;;testFailsAndErrorsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce a failed assert but no errors.
*/
subroutine testFailsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)

  set testing::failSetupOnce = TRUE
  set testing::failSetup = TRUE
  set testing::failTest = TRUE
  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE
 
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)

  set testCount = size(ucets_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[1].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[2].context, "setupOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[2].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[2].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[4].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[6].resultInd, FALSE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[3].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[4].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[6].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[4].asserts, 5), 7)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[4].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[5].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[5].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[6].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[6].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[7].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[7].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[7].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[5].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[2].context, "teardownOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[2].resultInd, FALSE)
end ;;;testFailsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when the test subroutines produce a failed assert 
  but there are no other problems.
*/
subroutine testFailsTestsOnly(null)
  declare testCount = i4 with protect, noconstant(0)
  
  set testing::failTest = TRUE
 
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)

  set testCount = size(ucets_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[2].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[3].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[3].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[3].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[4].resultInd, TRUE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[4].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[4].asserts, 5), 5)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[3].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[3].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[3].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[4].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[5].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
end ;;;testFailsTestsOnly


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when the setup subroutine causes an error 
  but there are no other problems.
*/
subroutine testSetupErrors(null)
  declare testCount = i4 with protect, noconstant(0)

  set testing::errorSetup = TRUE
 
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "S")
  
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)

  set testCount = size(ucets_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[2].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[2].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[2].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", 
      "%CCL-E-94-PRG_", ucets_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[2].errors[1].errorText)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[3].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", 
      "%CCL-E-94-PRG_", ucets_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[4].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[4].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucets_testCaseResults->tests[4].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucets_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", 
      "%CCL-E-94-PRG_", ucets_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucets_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucets_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucets_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucets_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucets_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucets_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
end ;;;testSetupErrors

/**
  Test a scenario where errors occur before any test is executed.
*/
subroutine testLoadingErrors(null)
  declare testing::errorLoading = i4 with protect, constant(TRUE)
  declare matches = i2 with protect, noconstant(FALSE)
 
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_testCaseResults->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 2", 
      ucets_testCaseResults->status_data.subeventstatus[1].operationName, "pre-executeTestLogic-0")
  call cclutAssertVCEqual(CURREF, "execute success 3", 
      ucets_testCaseResults->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "execute success 4", 
      ucets_testCaseResults->status_data.subeventstatus[1].targetObjectName, "ut_cclut_suo_su_td_tdo")
  set matches = operator(ucets_testCaseResults->status_data.subeventstatus[1].targetObjectValue, "regexplike", 
"%CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}\
Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.");^
  call cclutAssertI4Equal(CURREF, "execute success 5", matches, TRUE)
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)
end ;;;testLoadingErrors


/**
  Test a scenario where the test case file does not exist
*/

subroutine testTestCaseFileDoesNotExist(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucets_request1->testCaseFileName = "non-existant-test-case-file"
  
  execute cclut_execute_test_case_file with 
     replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 2", 
      ucets_reply->status_data.subeventstatus[1].operationName, "generateTestCaseProgram")
  call cclutAssertVCEqual(CURREF, "execute success 3", 
      ucets_reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "execute success 4", 
      ucets_reply->status_data.subeventstatus[1].targetObjectName, "non-existant-test-case-fi")
  call cclutAssertVCEqual(CURREF, "execute success 5", 
      ucets_reply->status_data.subeventstatus[1].targetObjectValue, "non-existant-test-case-file.inc not found in cclsource")  
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)
end ;;;testTestCaseFileDoesNotExist

/**
  Test a scenario where the current CCL version does not meet the minimum required version.
*/
subroutine testCclMinimumNotMet(null)
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults),
      replace("GETCCLVERSION", getOutdatedCclVersion)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 5", ucets_reply->status_data.subeventstatus[1].targetObjectValue, 
  "The CCL version [1.0.0] does not meet the minimum version required [8.12.0] for this version of the CCL Unit Testing Framework")
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)
end
subroutine cclut::getOutdatedCclVersion(null)
  return("1.0.0")
end ;;;testTestCaseFileDoesNotExist


/**
  Test a scenario where the test case program generation fails.
*/
subroutine testCompileProgramFailure(null)
  declare tov = vc with protect, noconstant("")
  declare idx = i4 with protect, noconstant(0)
  declare stat = i4 with protect, noconstant(0)
  
  set stat = copyRec(ucets_request, ucets_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)

  if (size(ucets_request1->programs, 5) = 0)
    set stat = alterlist(ucets_request1->programs, 1)
    set ucets_request1->programs[1].programName = "cclut"
    set ucets_request1->programs[1].compile = TRUE
  endif  
  set ucets_request1->programs[1].compile = TRUE
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request1), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults),
      replace("COMPILEPROGRAM", compileProgramFailure)
      
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].operationName, "compileProgram")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].operationStatus, "F")
  set tov = ucets_reply->status_data.subeventstatus[1].targetObjectValue
  call cclutAssertStartsWith(CURREF, "execute success", "compileProgram ", tov)
  call cclutAssertEndsWith(CURREF, "execute success", "compileProgram failure forced by unit test", tov)
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)
end
subroutine cclut::compileProgramFailure(fileDir, fileName, listingDir, listingName, errMsg)
  set errMsg = "compileProgram failure forced by unit test"
  return (FALSE)
end ;;;testCompileProgramFailure


/**
  Test a scenario where the test case program generation fails.
*/
subroutine testGenerateTestCaseProgramFailure(null)
  declare prgStatus = i4 with protect, noconstant(0)
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults),
      replace("GENERATETESTCASEPROGRAM", generateTestCaseProgramFailure)
      
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success", 
      ucets_reply->status_data.subeventstatus[1].operationName, "generateTestCaseProgram")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].targetObjectValue,
      "generateTestCaseProgram failure forced by unit test")
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)
end
subroutine cclut::generateTestCaseProgramFailure(fileDir, fileName, listingDir, listingName, objName, errorMessage)
  set errorMessage = "generateTestCaseProgram failure forced by unit test"
  return (FALSE)
end ;;;testGenerateTestCaseProgramFailure


/**
  Test a scenario where the test case program generation succeeds but the program fails to get into the dictionary.
*/
subroutine testCheckPrgFailure(null)
  declare prgStatus = i4 with protect, noconstant(0)
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults),
      replace("CHECKPRG", checkPrgFailure)
      
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].operationName, "CHECKPRG")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertI2Equal(CURREF, "test case failure", ucets_testCaseResults->resultInd, FALSE)

  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].targetObjectValue,
      "Test case program for ut_cclut_suo_su_td_tdo not in CCL dictionary after compilation.")
  ;Unless something bad happened, the test case program actually was compiled. 
  ;The following check confirms that the framework dropped it even though checkprg failed. 
  set prgStatus = checkPrg(ucets_reply->status_data.subeventstatus[1].targetObjectName)
  call cclutAssertI4Equal(CURREF, "test case program dropped", prgStatus, 0)
end
subroutine checkPrgFailure(prgName)
  return (0)
end ;;;testCheckPrgFailure


/**
  Test a scenario where an error occurs in the framework after execution of the test case program has completed.
*/
subroutine testPostExecutionFailure(null)
  declare stat = i4 with protect, noconstant(0)
  declare fileCount = i4 with protect, noconstant(0)
  declare fileIndex = i4 with protect, noconstant(0)
  declare tov = vc with protect, noconstant("")
  declare cclut::debug = i2 with protect, noconstant(TRUE)
    
  record filesToRemove(1 item[*] 2 location = vc)
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults),
      replace("REMOVE", mockRemove)

  for (fileIndex = 1 to fileCount)
    set stat = remove(filesToRemove->item[fileIndex].location)
  endfor

  call validateNoIssuesResponse(ucets_testCaseResults)
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success", ucets_reply->status_data.subeventstatus[1].operationStatus, "F")

  set tov = trim(ucets_reply->status_data.subeventstatus[1].targetObjectValue)
  call cclutAssertStartsWith(CURREF, "execute success", "%CCL-E-94-CCLUT_EXECUTE_TEST_CASE_FILE(", tov)
  call cclutAssertEndsWith(CURREF, "execute success", 
      "{REMOVEFAILURE}Assignment of Report expression (REMOVEFAILURE) to incompatible type.", tov)
end
subroutine mockRemove(fileLocation)
  declare removeFailure = i4 with protect, noconstant(0)
  declare fileCount = i4 with protect, noconstant(1 + size(filesToRemove->item, 5))
    
  set cclut::stat = alterlist(filesToRemove->item, fileCount)
  set filesToRemove->item[fileCount].location = fileLocation
  
  if (fileLocation = concat(cclut::outputDirectory, cclut::testCaseListingName))
    set removeFailure = "remove failure"
    return(FALSE)
  endif
  return(TRUE)
end ;;;testPostExecutionFailure


/**
  Test a scenario where a test produces an error and calls go to exit script without producing any asserts.
  The error should be attributed to the test not to the fraemwork.
*/
subroutine testErrorThenExitScript(null)
  set ucets_request->testCaseFileName = "ut_cclut_error_then_exit_script"

  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", ucets_request), 
      replace("CCLUTREPLY", ucets_reply),
      replace("CCLUTTESTCASERESULTS", ucets_testCaseResults)

  call cclutAssertI2Equal(CURREF, "test case should fail", ucets_testCaseResults->resultInd, FALSE)
  call cclutAssertI4Equal(CURREF, "test count", size(ucets_testCaseResults->tests, 5), 2)
  call cclutAssertVcEqual(CURREF, "setupOnce name", ucets_testCaseResults->tests[1].name, "SETUPONCE")
  call cclutAssertI4Equal(CURREF, "setupOnce assert count", size(ucets_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertVcEqual(CURREF, 
      "setupOnce assert context", ucets_testCaseResults->tests[1].asserts[1].context, "setupOnce - guaranteed success")
  call cclutAssertI4Equal(CURREF, "setupOnce error count", size(ucets_testCaseResults->tests[1].errors, 5), 0);

  call cclutAssertVcEqual(CURREF, "testOne name", ucets_testCaseResults->tests[2].name, "TESTONE")
  call cclutAssertI4Equal(CURREF, "testOne assert count", size(ucets_testCaseResults->tests[2].asserts, 5), 0)
  call cclutAssertI4Equal(CURREF, "testOne error count", size(ucets_testCaseResults->tests[2].errors, 5), 1)
  call cclutAssertVcOperator(CURREF, "testOne error text", ucets_testCaseResults->tests[2].errors.errorText, "regexplike",
    "%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): forced error.");^
end ;;;testErrorThenExitScript


/**
  Confirms that code coverage exclusion works as exepcted.
*/
subroutine testCoverageExcludes(null)
  declare stat = i4 with protect, noconstant(0)
  
  if (currev > 8 or (currev = 8 and currevminor > 12))
    declare debugAvailable = i4 with protect, noconstant(0)
    declare previousDebugTrace = i4 with protect, noconstant(0)
    declare outputLine = vc with protect, noconstant("")

    select into cclut_check_debug from dummyt detail
      outputLine = "drop program cclut_check_debug go" 
      outputLine row+1
      outputLine = "create program cclut_check_debug" 
      outputLine row+1
      outputLine = "set debugAvailable = evaluate(trace('DEBUG'),1,1,0)"
      outputLine row+1
      outputLine = "if (debugAvailable = FALSE)"
      outputLine row+1
      outputLine = "set trace debug"
      outputLine row+1
      outputLine = "set debugAvailable = evaluate(trace('DEBUG'),1,1,0)"
      outputLine row+1
      outputLine = "if (previousDebugTrace = 0) set trace nodebug endif"
      outputLine row+1
      outputLine = "endif"
      outputLine row+1
      outputLine = "end go" 
      outputLine row+1
    with nocounter
    call compile("cclut_check_debug.dat")
    execute cclut_check_debug
    call parser ("drop program cclut_check_debug go")
    set stat = remove("cclut_check_debug.dat")
    set stat = 
        cclutAssertI4Equal(CURREF, "cclora_dbg required for code coverage since CCL version 8.13.0", debugAvailable, TRUE)
    if (stat = FALSE)
      return (null)
    endif
  endif 

  declare coverageXml = vc with protect, noconstant("")
  
  set stat = copyrec(ucets_request, tce_request)  
  set stat = copyrec(ucets_reply, tce_reply)  
  set stat = copyrec(ucets_testCaseResults, tce_testCaseResults)

  
  set tce_request->testCaseDirectory = "cclsource"
  set tce_request->testCaseFileName = "ut_ex_ut_cclut_cc_exclusions"
  set stat = alterlist(tce_request->programs, 1)
  set tce_request->programs[1].programName = "ut_cclut_cc_exclusions"
  set tce_request->programs[1].compile = TRUE
  
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", tce_request), 
      replace("CCLUTREPLY", tce_reply),
      replace("CCLUTTESTCASERESULTS", tce_testCaseResults)
  call cclutAssertVcNotEqual(CURREF, "expected non-empty listingXml", tce_reply->programs[1].listingXml, "")
  set coverageXml = tce_reply->programs[1].coverageXml
  call cclutAssertContains(CURREF, "expect 14 non-executable", "<LINE><NBR>14</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 27 non-executable", "<LINE><NBR>27</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 28 non-executable", "<LINE><NBR>28</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 38 non-executable", "<LINE><NBR>38</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 45 non-executable", "<LINE><NBR>45</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 46 non-executable", "<LINE><NBR>46</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 47 non-executable", "<LINE><NBR>47</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 48 non-executable", "<LINE><NBR>48</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  
  
  set stat = initrec(tce_reply)
  set stat = initrec(tce_testCaseResults)
  set tce_request->programs[1].compile = FALSE
  execute cclut_execute_test_case_file with 
      replace("CCLUTREQUEST", tce_request), 
      replace("CCLUTREPLY", tce_reply),
      replace("CCLUTTESTCASERESULTS", tce_testCaseResults)
  call cclutAssertVcEqual(CURREF, "expected empty listingXml", tce_reply->programs[1].listingXml, "")
  call cclutAssertVcEqual(CURREF, "expected same coverageXml", tce_reply->programs[1].coverageXml, coverageXml)
end ;;;testCoverageExcludes