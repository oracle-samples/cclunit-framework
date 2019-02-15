/**
  Unit Tests for cclut_execute_test_case.
*/

%i cclsource:cclut_xml_functions.inc

declare setup(null) = null with protect
declare validateEnvironmentXml(xml = vc) = null with protect
declare validateListingXml(xml = vc) = null with protect
declare validateResultsXml(xml = vc) = null with protect
declare validateCoverageXml(coverage = vc) = null with protect
declare testNoIssues(null) = null with protect
declare testErrorsEverywhere(null) = null with protect
declare testFailsAndErrorsEverywhere(null) = null with protect
declare testFailsEverywhere(null) = null with protect
declare testFailsTestsOnly(null) = null with protect
declare testSetupErrors(null) = null with protect


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

record ucetc_testCaseResults (
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

  set stat = initRec(ucetc_request)
  set stat = initRec(ucetc_reply)
  set stat = initRec(ucetc_testCaseResults)

  set ucetc_request->testINCName = "ut_cclut_suo_su_td_tdo"

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

  declare expectedListing = vc with protect, noconstant("")
  set expectedListing =
~</COMPILE_DATE><LINES><LINE><NBR>1817</NBR><TEXT><![CDATA[/**]]></TEXT></LINE><LINE><NBR>1818</NBR><TEXT><![CDATA[  A test c\
ase file for testing the CCL Unit framework.]]></TEXT></LINE><LINE><NBR>1819</NBR><TEXT><![CDATA[  This test case file contai\
ns all of the testing support subroutines, setupOnce, setup, teardown and teardownOnce and three test]]></TEXT></LINE><LINE><\
NBR>1820</NBR><TEXT><![CDATA[  subroutines. Each subroutine fires a successful assert to indicate it executed and based on co\
nfiguration will then fire]]></TEXT></LINE><LINE><NBR>1821</NBR><TEXT><![CDATA[  a failed assert and/or cause an error based \
on the value of the boolean variable]]></TEXT></LINE><LINE><NBR>1822</NBR><TEXT><![CDATA[  testing::{fail|error}{SetupOnce|Se\
tup|Test|Teardown|TeardownOnce} which needs to be provided by the invoking test.]]></TEXT></LINE><LINE><NBR>1823</NBR><TEXT><\
![CDATA[*/]]></TEXT></LINE><LINE><NBR>1824</NBR><TEXT><![CDATA[declare setupOnce(null) = null]]></TEXT></LINE><LINE><NBR>1825\
</NBR><TEXT><![CDATA[declare teardownOnce(null) = null]]></TEXT></LINE><LINE><NBR>1826</NBR><TEXT><![CDATA[declare setup(null\
) = null]]></TEXT></LINE><LINE><NBR>1827</NBR><TEXT><![CDATA[declare testOne(null) = null]]></TEXT></LINE><LINE><NBR>1828</NB\
R><TEXT><![CDATA[declare testTwo(null) = null]]></TEXT></LINE><LINE><NBR>1829</NBR><TEXT><![CDATA[declare testThree(null) = n\
ull]]></TEXT></LINE><LINE><NBR>1830</NBR><TEXT><![CDATA[declare teardown(null) = null]]></TEXT></LINE><LINE><NBR>1831</NBR><T\
EXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1832</NBR><TEXT><![CDATA[declare guaranteedError = i4 with protect, noconstant(0)]]>\
</TEXT></LINE><LINE><NBR>1833</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1834</NBR><TEXT><![CDATA[if (validate(testing::\
errorLoading, FALSE) = TRUE)]]></TEXT></LINE><LINE><NBR>1835</NBR><TEXT><![CDATA[    set guaranteedError = "loading error"]]>\
</TEXT></LINE><LINE><NBR>1836</NBR><TEXT><![CDATA[endif]]></TEXT></LINE><LINE><NBR>1837</NBR><TEXT><![CDATA[]]></TEXT></LINE>\
<LINE><NBR>1838</NBR><TEXT><![CDATA[subroutine setupOnce(null)]]></TEXT></LINE><LINE><NBR>1839</NBR><TEXT><![CDATA[  call ccl\
utAssertVCEqual(CURREF, "setupOnce executed", "SO", "SO")]]></TEXT></LINE><LINE><NBR>1840</NBR><TEXT><![CDATA[  if (testing::\
failSetupOnce)]]></TEXT></LINE><LINE><NBR>1841</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "setupOnce failure", 0\
, 1)]]></TEXT></LINE><LINE><NBR>1842</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1843</NBR><TEXT><![CDATA[  if (te\
sting::errorSetupOnce)]]></TEXT></LINE><LINE><NBR>1844</NBR><TEXT><![CDATA[    set guaranteedError = "setupOnce error"]]></TE\
XT></LINE><LINE><NBR>1845</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1846</NBR><TEXT><![CDATA[end ;setupOnce]]></\
TEXT></LINE><LINE><NBR>1847</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1848</NBR><TEXT><![CDATA[subroutine teardownOnce(\
null)]]></TEXT></LINE><LINE><NBR>1849</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(CURREF, "teardownOnce executed", "TO", "T\
O")]]></TEXT></LINE><LINE><NBR>1850</NBR><TEXT><![CDATA[  if (testing::failTeardownOnce)]]></TEXT></LINE><LINE><NBR>1851</NBR\
><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "teardownOnce failure", 0, 1)]]></TEXT></LINE><LINE><NBR>1852</NBR><TEXT>\
<![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1853</NBR><TEXT><![CDATA[  if (testing::errorTeardownOnce)]]></TEXT></LINE><LINE>\
<NBR>1854</NBR><TEXT><![CDATA[    set guaranteedError = "teardownOnce error"]]></TEXT></LINE><LINE><NBR>1855</NBR><TEXT><![CD\
ATA[  endif]]></TEXT></LINE><LINE><NBR>1856</NBR><TEXT><![CDATA[end ;teardownOnce]]></TEXT></LINE><LINE><NBR>1857</NBR><TEXT>\
<![CDATA[]]></TEXT></LINE><LINE><NBR>1858</NBR><TEXT><![CDATA[subroutine setup(null)]]></TEXT></LINE><LINE><NBR>1859</NBR><TE\
XT><![CDATA[  call cclutAssertI4Equal(CURREF, "setup executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1860</NBR><TEXT><![CDATA[  i\
f (testing::failSetup)]]></TEXT></LINE><LINE><NBR>1861</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "setup failure\
", 0, 1)]]></TEXT></LINE><LINE><NBR>1862</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1863</NBR><TEXT><![CDATA[  if\
 (testing::errorSetup)]]></TEXT></LINE><LINE><NBR>1864</NBR><TEXT><![CDATA[    set guaranteedError = "setup error"]]></TEXT><\
/LINE><LINE><NBR>1865</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1866</NBR><TEXT><![CDATA[end ;;;setup]]></TEXT><\
/LINE><LINE><NBR>1867</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1868</NBR><TEXT><![CDATA[subroutine teardown (null)]]><\
/TEXT></LINE><LINE><NBR>1869</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "teardown executed", 0, 0)]]></TEXT></LINE\
><LINE><NBR>1870</NBR><TEXT><![CDATA[  if (testing::failTeardown)]]></TEXT></LINE><LINE><NBR>1871</NBR><TEXT><![CDATA[    cal\
l cclutAssertI4Equal(CURREF, "teardown failure", 0, 1)]]></TEXT></LINE><LINE><NBR>1872</NBR><TEXT><![CDATA[  endif]]></TEXT><\
/LINE><LINE><NBR>1873</NBR><TEXT><![CDATA[  if (testing::errorTeardown)]]></TEXT></LINE><LINE><NBR>1874</NBR><TEXT><![CDATA[ \
   set guaranteedError = "teardown error"]]></TEXT></LINE><LINE><NBR>1875</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><\
NBR>1876</NBR><TEXT><![CDATA[end ;;;setup]]></TEXT></LINE><LINE><NBR>1877</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>187\
8</NBR><TEXT><![CDATA[subroutine testOne(null)]]></TEXT></LINE><LINE><NBR>1879</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(\
CURREF, "testOne executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1880</NBR><TEXT><![CDATA[  if (testing::failTest)]]></TEXT></LIN\
E><LINE><NBR>1881</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "testOne failure", 0, 1)]]></TEXT></LINE><LINE><NBR\
>1882</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1883</NBR><TEXT><![CDATA[  if (testing::errorTest)]]></TEXT></LI\
NE><LINE><NBR>1884</NBR><TEXT><![CDATA[    set guaranteedError = "testOne error"]]></TEXT></LINE><LINE><NBR>1885</NBR><TEXT><\
![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1886</NBR><TEXT><![CDATA[end ;;;testOne]]></TEXT></LINE><LINE><NBR>1887</NBR><TEXT\
><![CDATA[]]></TEXT></LINE><LINE><NBR>1888</NBR><TEXT><![CDATA[subroutine testTwo(null)]]></TEXT></LINE><LINE><NBR>1889</NBR>\
<TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "testTwo executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1890</NBR><TEXT><![CDAT\
A[  call cclutAssertVCEqual(CURREF, "testTwo executed", "A", "A")]]></TEXT></LINE><LINE><NBR>1891</NBR><TEXT><![CDATA[end ;;;\
testTwo]]></TEXT></LINE><LINE><NBR>1892</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1893</NBR><TEXT><![CDATA[;; like test\
One, but two failures and two errors and errors come before failures when both are specified.]]></TEXT></LINE><LINE><NBR>1894\
</NBR><TEXT><![CDATA[subroutine testThree(null)]]></TEXT></LINE><LINE><NBR>1895</NBR><TEXT><![CDATA[  call cclutAssertI4Equal\
(CURREF, "testThree executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1896</NBR><TEXT><![CDATA[  if (testing::errorTest)]]></TEXT><\
/LINE><LINE><NBR>1897</NBR><TEXT><![CDATA[    set guaranteedError = "testThree error"]]></TEXT></LINE><LINE><NBR>1898</NBR><T\
EXT><![CDATA[    set guaranteedError = "testThree error 2"]]></TEXT></LINE><LINE><NBR>1899</NBR><TEXT><![CDATA[  endif]]></TE\
XT></LINE><LINE><NBR>1900</NBR><TEXT><![CDATA[  if (testing::failTest)]]></TEXT></LINE><LINE><NBR>1901</NBR><TEXT><![CDATA[  \
  call cclutAssertI4Equal(CURREF, "testThree failure A", 0, 1)]]></TEXT></LINE><LINE><NBR>1902</NBR><TEXT><![CDATA[    call c\
clutAssertI4Equal(CURREF, "testThree failure B", 0, 1)]]></TEXT></LINE><LINE><NBR>1903</NBR><TEXT><![CDATA[  endif]]></TEXT><\
/LINE><LINE><NBR>1904</NBR><TEXT><![CDATA[end ;;;testThree]]></TEXT></LINE></LINES></LISTING>~

;The next assert is a maintenance hastle when the cclut_execute_test_case code changes.
;Uncomment the echo statment to get the real value and update expectedListing accordingly.
;call echo(build2("listingXml = ", xml))
  call cclutAssertEndsWith(CURREF, "validate listingXml", expectedListing, xml)
end ;;;validateListingXml


/**
  Validates that a resultsXml matches the expected results.

  Note: The behavior of this function depends on the specific contents of various framework files
  so this function must be updated whenever those files are modified.
*/
subroutine validateResultsXml(xml)
  declare foundInd = i2 with protect, noconstant(FALSE)
  declare testCaseName = vc with protect, noconstant("")
  declare testCount = i4 with protect, noconstant(0)
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
  call cclutAssertVCEqual(CURREF, "validate line number 1.1", lineNbr, "1839")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 1.1", assertContext, "setupOnce executed")
  set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
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
  call cclutAssertVCEqual(CURREF, "validate line number 2.1", lineNbr, "1859")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.1", assertContext, "setup executed")
  set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
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
  call cclutAssertVCEqual(CURREF, "validate line number 2.2", lineNbr, "1879")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.2", assertContext, "testOne executed")
  set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
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
  call cclutAssertVCEqual(CURREF, "validate line number 2.3", lineNbr, "1869")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.3", assertContext, "teardown executed")
  set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
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
  call cclutAssertVCEqual(CURREF, "validate line number 5.1", lineNbr, "1849")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 5.1", assertContext, "teardownOnce executed")
  set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
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
  Validates that a coverageXml matches the expected coverage. 
  
  Note: The behavior of this function depends on the specific contents of various framework files 
  including this file so this function must be updated whenever those files are modified.
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
  Run ut_cclut_suo_su_td_tdo and validate the results when no issues occur.
*/
subroutine testNoIssues(null)
  declare stat = i4 with protect, noconstant(0)
  declare testCount = i4 with protect, noconstant(0)
  declare testIdx = i4 with protect, noconstant(0)
  declare assertIdx = i4 with protect, noconstant(0)
  declare xml = vc with protect, noconstant("")
  declare xmlItem = vc with protect, noconstant("")
  declare itemFound = i2 with protect, noconstant(FALSE)
  declare cclut::debug = i2 with protect, noconstant(TRUE)

  set stat = copyRec(ucetc_request, ucetc_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set stat = alterlist(ucetc_request1->programs, 2)
  set ucetc_request1->programs[1].programName = "someProgramName"
  set ucetc_request1->programs[2].programName = "someOtherProgramName"

  execute cclut_execute_test_case with 
      replace("CCLUTREQUEST", ucetc_request1), 
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)
  
  call cclutAssertVCEqual(CURREF, "execute success 1", ucetc_reply->status_data.status, "S")

  call validateEnvironmentXml(ucetc_reply->environmentXml)
  call validateListingXml(ucetc_reply->testINCListingXml)
  call validateResultsXml(ucetc_reply->testINCResultsXml)
  call validateCoverageXml(ucetc_reply->testINCCoverageXml)
  
  call cclutAssertI4Equal(CURREF, "validate programs", size(ucetc_reply->programs, 5), 2)
  call cclutAssertVCEqual(CURREF, "validate program1", ucetc_reply->programs[1].programName, "someProgramName")
  call cclutAssertVCEqual(CURREF, "validate program1", ucetc_reply->programs[1].listingXml, "")
  call cclutAssertVCEqual(CURREF, "validate program1", ucetc_reply->programs[1].coverageXml, 
    "<COVERAGE><COVERAGE_NAME>someProgramName</COVERAGE_NAME></COVERAGE>")
  call cclutAssertVCEqual(CURREF, "validate program1", ucetc_reply->programs[2].programName, "someOtherProgramName")
  call cclutAssertVCEqual(CURREF, "validate program1", ucetc_reply->programs[2].listingXml, "")
  call cclutAssertVCEqual(CURREF, "validate program1", ucetc_reply->programs[2].coverageXml, 
    "<COVERAGE><COVERAGE_NAME>someOtherProgramName</COVERAGE_NAME></COVERAGE>")

end ;;;testNoIssues 


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce an error.
*/
subroutine testErrorsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare testIdx = i4 with protect, noconstant(0)
  declare assertIdx = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetupOnce = TRUE
  set testing::errorSetup = TRUE
  set testing::errorTest = TRUE
  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE
 
  execute cclut_execute_test_case with
      replace("CCLUTREQUEST", ucetc_request),
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucetc_reply->status_data.status, "S")
  set testCount = size(ucetc_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[1].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[1].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[1].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[2].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[2].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[2].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetc_testCaseResults->tests[2].errors[errorIdx].errorText)
  endfor
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[3].errors, 5), 2)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  for (errorIdx = 1 to 2)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[3].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetc_testCaseResults->tests[3].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[4].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[4].errors, 5), 4)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  for (errorIdx = 1 to 4)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[4].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetc_testCaseResults->tests[4].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[5].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[4].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[4].errors[1].errorText)
end ;;;testErrorsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce a failed assert and an error.
*/
subroutine testFailsAndErrorsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare testIdx = i4 with protect, noconstant(0)
  declare assertIdx = i4 with protect, noconstant(0)
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
 
  execute cclut_execute_test_case with 
      replace("CCLUTREQUEST", ucetc_request), 
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucetc_reply->status_data.status, "S")
  
  set testCount = size(ucetc_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[1].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[1].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[2].context, "setupOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[2].resultInd, FALSE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[1].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[1].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[2].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[2].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[4].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[6].resultInd, FALSE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[2].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetc_testCaseResults->tests[2].errors[errorIdx].errorText)
  endfor
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[3].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[3].errors, 5), 2)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[4].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[6].resultInd, FALSE)
  for (errorIdx = 1 to 2)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[3].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetc_testCaseResults->tests[3].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[4].asserts, 5), 7)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[4].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[4].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[5].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[5].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[6].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[6].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[7].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[7].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[7].resultInd, FALSE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText", 
        "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[4].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText", 
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetc_testCaseResults->tests[4].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[5].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[5].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[2].context, "teardownOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[2].resultInd, FALSE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[4].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[4].errors[1].errorText)
end ;;;testFailsAndErrorsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce a failed assert but no errors.
*/
subroutine testFailsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare testIdx = i4 with protect, noconstant(0)
  declare assertIdx = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::failSetupOnce = TRUE
  set testing::failSetup = TRUE
  set testing::failTest = TRUE
  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE
 
  execute cclut_execute_test_case with 
      replace("CCLUTREQUEST", ucetc_request), 
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucetc_reply->status_data.status, "S")
  
  set testCount = size(ucetc_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[1].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[2].context, "setupOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[2].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[2].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[4].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[6].resultInd, FALSE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[3].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[4].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[6].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[6].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[4].asserts, 5), 7)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[4].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[5].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[5].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[6].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[6].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[7].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[7].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[7].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[5].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[2].context, "teardownOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[2].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[2].resultInd, FALSE)
end ;;;testFailsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when the test subroutines produce a failed assert 
  but there are no other problems.
*/
subroutine testFailsTestsOnly(null)
  declare testCount = i4 with protect, noconstant(0)
  declare testIdx = i4 with protect, noconstant(0)
  declare assertIdx = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::failTest = TRUE
 
  execute cclut_execute_test_case with 
      replace("CCLUTREQUEST", ucetc_request), 
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucetc_reply->status_data.status, "S")
  
  set testCount = size(ucetc_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[2].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[3].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[3].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[3].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[4].resultInd, TRUE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[4].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[4].asserts, 5), 5)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[3].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[3].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[3].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[4].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[4].condition, "0=1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[5].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[5].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
end ;;;testFailsTestsOnly


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when the setup subroutine causes an error 
  but there are no other problems.
*/
subroutine testSetupErrors(null)
  declare testCount = i4 with protect, noconstant(0)
  declare testIdx = i4 with protect, noconstant(0)
  declare assertIdx = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetup = TRUE
 
  execute cclut_execute_test_case with 
      replace("CCLUTREQUEST", ucetc_request), 
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)
      
  call cclutAssertVCEqual(CURREF, "execute success 1", ucetc_reply->status_data.status, "S")
  
  set testCount = size(ucetc_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[1].asserts[1].condition, '"SO"="SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[2].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[2].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[2].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[2].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", 
      "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[2].errors[1].errorText)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[3].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[3].condition, '"A"="A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[3].asserts[4].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", 
      "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[4].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[4].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[1].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[2].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetc_testCaseResults->tests[4].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[4].asserts[3].condition, "0=0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetc_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", 
      "%CCL-E-94-PRG_", ucetc_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText", 
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetc_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetc_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetc_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      ucetc_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetc_testCaseResults->tests[5].asserts[1].condition, '"TO"="TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      ucetc_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
end ;;;testSetupErrors

/**
  Test that the framework removes any mock tables that test cases fail to do when calling cclut_execute_test_case.
*/
subroutine testMockTableCleanup(null)
  declare cclutMockTableName = vc with protect, noconstant("")

  set ucetc_request->testINCName = "ut_cclut_mock_table_misbehaving_cleanup"

  execute cclut_execute_test_case with
      replace("CCLUTREQUEST", ucetc_request),
      replace("CCLUTREPLY", ucetc_reply),
      replace("CCLUTTESTCASERESULTS", ucetc_testCaseResults)

  call cclutAssertStartsWith(CURREF, "testMockTableCleanup 001", "CUST_CCLUT", cclutMockTableName)

  declare mockId = f8 with protect, noconstant(0.0)
  declare mockText = vc with protect, noconstant("")
  declare mockDate = dq8 with protect, noconstant(0)
  select into "nl:"
  from (value(cclutMockTableName) m)
  detail
      mockId = m.SAMPLE_TABLE_ID
      mockText = m.SAMPLE_TABLE_TEXT
      mockDate = m.SAMPLE_TABLE_DATE
  with nocounter

  declare errorMessage = vc with protect, noconstant("")
  declare errorCode = i4 with protect, noconstant(0)
  set errorCode = error(errorMessage, 0)

  call cclutAssertVcOperator(CURREF, "testMockTableCleanup 002", trim(errorMessage, 3),
      "regexplike", concat("%CCL-E-18-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{\}Unable to add range, definition for",
          " table \(CUST_CCLUT_[^)]*\) not found in dictionary."))
end ;;;testMockTableCleanup
