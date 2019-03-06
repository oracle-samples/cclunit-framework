/**
  Unit Test code common to both cclut_execute_test_case and cclut_execute_test_case_file.
*/

declare setup(null) = null with protect
declare testNoIssuesLegacy(null) = null with protect
declare validateEnvironmentXml(xml = vc) = null with protect
declare validateListingXml(xml = vc) = null with protect
declare validateResultsXml(xml = vc) = null with protect
declare validateResultsXml(xml = vc, legacyFormat = i2) = null with protect
declare validateNoIssuesResponse(response = vc(ref)) = null with protect

record ucetcf_testCaseResults (
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

  set stat = initRec(ucetcf_testCaseResults)

  if (testing::sut = "cclut_execute_test_case_file")
    set stat = initRec(ucetcf_request)
    set stat = initRec(ucetcf_reply)
    set ucetcf_request->testCaseDirectory = "cclsource"
    set ucetcf_request->testCaseFileName = "ut_cclut_suo_su_td_tdo"
  else
    set stat = initRec(ucetc_request)
    set stat = initRec(ucetc_reply)
    set ucetc_request->testINCName = "ut_cclut_suo_su_td_tdo"
  endif

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
  Run ut_cclut_suo_su_td_tdo and validate the results when no issues occur with legacy format requested.
*/
subroutine testNoIssuesLegacy(null)
  declare stat = i4 with protect, noconstant(0)
  declare programCount = i4 with protect, noconstant(0)
    
  if (testing::sut = "cclut_execute_test_case_file")  
    set stat = copyRec(ucetcf_request, ucet_request, 1)
    call cclutAssertI4Equal(CURREF, "validate copyRec one", stat, TRUE)
    set ucet_request->legacyResultsFormat = TRUE
    set stat = copyRec(ucetcf_reply, ucet_reply, 1)
  else
    set stat = copyRec(ucetc_request, ucet_request, 1)
    call cclutAssertI4Equal(CURREF, "validate copyRec one", stat, TRUE)
    set stat = copyRec(ucetc_reply, ucet_reply, 1)
  endif
  call cclutAssertI4Equal(CURREF, "validate copyRec two", stat, TRUE)

  if (validate(cclut_request) = TRUE)
    ;to capture code coverage if running from maven
    set stat = moverec(cclut_request->programs, ucet_request->programs)
  endif
  set programCount = 2 + size(ucet_request->programs, 5)
  set stat = alterlist(ucet_request->programs, programCount)
  set ucet_request->programs[programCount - 1].programName = "someProgramName"
  set ucet_request->programs[programCount].programName = "someOtherProgramName"
  
  
  execute value(cnvtupper(testing::sut)) with 
      replace("CCLUTREQUEST", ucet_request), 
      replace("CCLUTREPLY", ucet_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)
call echorecord(ucet_reply)  
  call cclutAssertVCEqual(CURREF, "execute success 1", ucet_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucetcf_testCaseResults)
  
  if (testing::sut = "cclut_execute_test_case_file")  
    call validateEnvironmentXml(ucet_reply->environmentXml)
    call validateListingXml(ucet_reply->listingXml)
    call validateResultsXml(ucet_reply->resultsXml, TRUE)
    call validateCoverageXml(ucet_reply->coverageXml)
  else
    call validateEnvironmentXml(ucet_reply->environmentXml)
    call validateListingXml(ucet_reply->testINCListingXml)
    call validateResultsXml(ucet_reply->testINCResultsXml, TRUE)
    call validateCoverageXml(ucet_reply->testINCCoverageXml)
  endif
  
  call cclutAssertI4Equal(CURREF, "validate programs", size(ucet_reply->programs, 5), programCount)
  call cclutAssertVCEqual(CURREF, "validate program1", ucet_reply->programs[programCount-1].programName, "someProgramName")
  call cclutAssertVCEqual(CURREF, "validate program1", ucet_reply->programs[programCount-1].listingXml, "")
  call cclutAssertVCEqual(CURREF, "validate program1", ucet_reply->programs[programCount-1].coverageXml, 
    "<COVERAGE><COVERAGE_NAME>someProgramName</COVERAGE_NAME></COVERAGE>")
  call cclutAssertVCEqual(CURREF, "validate program1", ucet_reply->programs[programCount].programName, "someOtherProgramName")
  call cclutAssertVCEqual(CURREF, "validate program1", ucet_reply->programs[programCount].listingXml, "")
  call cclutAssertVCEqual(CURREF, "validate program1", ucet_reply->programs[programCount].coverageXml, 
    "<COVERAGE><COVERAGE_NAME>someOtherProgramName</COVERAGE_NAME></COVERAGE>")
end ;;;testNoIssuesLegacy


/**
  Validates that an environmentXml matches the expected enviromentXml. 
*/
subroutine validateEnvironmentXml(xml)
  declare xmlItem = vc with protect, noconstant("")
  declare itemFound = i2 with protect, noconstant(FALSE)

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDB", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract currdb", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdb", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDBNAME", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract currdbname", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdbname", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDBUSER", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract currdbuser", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdbuser", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURRDBSYS", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract currdbsys", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currdbsys", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CCLVER", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract cclver", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate cclver", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURSYS", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract cursys", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate cursys", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURLOCALE", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curlocale", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curlocale", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURUSER", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curuser", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curuser", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURUTC", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curutc", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curutc", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURUTCDIFF", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curutcdiff", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curutcdiff", xmlItem, "")
  
  set xmlItem = cclut::retrieveXmlContent(xml, "CURTIMEZONE", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curtimezone", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curtimezone", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURTIMEZONEAPP", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curtimezoneapp", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curtimezoneapp", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURTIMEZONESYS", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curtimezonesys", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curtimezonesys", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURREVAFD", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract currevafd", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate currevafd", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "CURGROUP", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract curgroup", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate curgroup", xmlItem, "")
  
  set xmlItem = cclut::retrieveXmlContent(xml, "DBVERSION", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract dbversion", itemFound, TRUE)
  call cclutAssertVCNotEqual(CURREF, "validate dbversion", xmlItem, "")

  set xmlItem = cclut::retrieveXmlContent(xml, "DBOPTMODE", 1, itemFound)
  call cclutAssertI4Equal(CURREF, "extract dboptmode", itemFound, TRUE)
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
~</COMPILE_DATE><LINES><LINE><NBR>1884</NBR><TEXT><![CDATA[/**]]></TEXT></LINE><LINE><NBR>1885</NBR><TEXT><![CDATA[  A t\
est case file for testing the CCL Unit framework.]]></TEXT></LINE><LINE><NBR>1886</NBR><TEXT><![CDATA[  This test case f\
ile contains all of the testing support subroutines, setupOnce, setup, teardown and teardownOnce and three test]]></TEXT\
></LINE><LINE><NBR>1887</NBR><TEXT><![CDATA[  subroutines. Each subroutine fires a successful assert to indicate it exec\
uted and based on configuration will then fire]]></TEXT></LINE><LINE><NBR>1888</NBR><TEXT><![CDATA[  a failed assert and\
/or cause an error based on the value of the boolean variable]]></TEXT></LINE><LINE><NBR>1889</NBR><TEXT><![CDATA[  test\
ing::{fail|error}{SetupOnce|Setup|Test|Teardown|TeardownOnce} which needs to be provided by the invoking test.]]></TEXT>\
</LINE><LINE><NBR>1890</NBR><TEXT><![CDATA[*/]]></TEXT></LINE><LINE><NBR>1891</NBR><TEXT><![CDATA[declare setupOnce(null\
) = null]]></TEXT></LINE><LINE><NBR>1892</NBR><TEXT><![CDATA[declare teardownOnce(null) = null]]></TEXT></LINE><LINE><NB\
R>1893</NBR><TEXT><![CDATA[declare setup(null) = null]]></TEXT></LINE><LINE><NBR>1894</NBR><TEXT><![CDATA[declare testOn\
e(null) = null]]></TEXT></LINE><LINE><NBR>1895</NBR><TEXT><![CDATA[declare testTwo(null) = null]]></TEXT></LINE><LINE><N\
BR>1896</NBR><TEXT><![CDATA[declare testThree(null) = null]]></TEXT></LINE><LINE><NBR>1897</NBR><TEXT><![CDATA[declare t\
eardown(null) = null]]></TEXT></LINE><LINE><NBR>1898</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1899</NBR><TEXT><![\
CDATA[declare guaranteedError = i4 with protect, noconstant(0)]]></TEXT></LINE><LINE><NBR>1900</NBR><TEXT><![CDATA[]]></\
TEXT></LINE><LINE><NBR>1901</NBR><TEXT><![CDATA[if (validate(testing::errorLoading, FALSE) = TRUE)]]></TEXT></LINE><LINE\
><NBR>1902</NBR><TEXT><![CDATA[    set guaranteedError = "loading error"]]></TEXT></LINE><LINE><NBR>1903</NBR><TEXT><![C\
DATA[endif]]></TEXT></LINE><LINE><NBR>1904</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1905</NBR><TEXT><![CDATA[subr\
outine setupOnce(null)]]></TEXT></LINE><LINE><NBR>1906</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(CURREF, "setupOnce \
executed", "SO", "SO")]]></TEXT></LINE><LINE><NBR>1907</NBR><TEXT><![CDATA[  if (testing::failSetupOnce)]]></TEXT></LINE\
><LINE><NBR>1908</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "setupOnce failure", 0, 1)]]></TEXT></LINE><LIN\
E><NBR>1909</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1910</NBR><TEXT><![CDATA[  if (testing::errorSetupOnc\
e)]]></TEXT></LINE><LINE><NBR>1911</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "setupOnce error", 1, 1)]]></\
TEXT></LINE><LINE><NBR>1912</NBR><TEXT><![CDATA[    set guaranteedError = "setupOnce error"]]></TEXT></LINE><LINE><NBR>1\
913</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1914</NBR><TEXT><![CDATA[end ;setupOnce]]></TEXT></LINE><LINE\
><NBR>1915</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1916</NBR><TEXT><![CDATA[subroutine teardownOnce(null)]]></TE\
XT></LINE><LINE><NBR>1917</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(CURREF, "teardownOnce executed", "TO", "TO")]]><\
/TEXT></LINE><LINE><NBR>1918</NBR><TEXT><![CDATA[  if (testing::failTeardownOnce)]]></TEXT></LINE><LINE><NBR>1919</NBR><\
TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "teardownOnce failure", 0, 1)]]></TEXT></LINE><LINE><NBR>1920</NBR><TE\
XT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1921</NBR><TEXT><![CDATA[  if (testing::errorTeardownOnce)]]></TEXT></LIN\
E><LINE><NBR>1922</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "teardownOnce error", 1, 1)]]></TEXT></LINE><L\
INE><NBR>1923</NBR><TEXT><![CDATA[    set guaranteedError = "teardownOnce error"]]></TEXT></LINE><LINE><NBR>1924</NBR><T\
EXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1925</NBR><TEXT><![CDATA[end ;teardownOnce]]></TEXT></LINE><LINE><NBR>19\
26</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1927</NBR><TEXT><![CDATA[subroutine setup(null)]]></TEXT></LINE><LINE\
><NBR>1928</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "setup executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1929\
</NBR><TEXT><![CDATA[  if (testing::failSetup)]]></TEXT></LINE><LINE><NBR>1930</NBR><TEXT><![CDATA[    call cclutAssertI\
4Equal(CURREF, "setup failure", 0, 1)]]></TEXT></LINE><LINE><NBR>1931</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE>\
<NBR>1932</NBR><TEXT><![CDATA[  if (testing::errorSetup)]]></TEXT></LINE><LINE><NBR>1933</NBR><TEXT><![CDATA[    call cc\
lutAssertI4Equal(CURREF, "setup error", 1, 1)]]></TEXT></LINE><LINE><NBR>1934</NBR><TEXT><![CDATA[    set guaranteedErro\
r = "setup error"]]></TEXT></LINE><LINE><NBR>1935</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1936</NBR><TEXT\
><![CDATA[end ;;;setup]]></TEXT></LINE><LINE><NBR>1937</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1938</NBR><TEXT><\
![CDATA[subroutine teardown (null)]]></TEXT></LINE><LINE><NBR>1939</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF,\
 "teardown executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1940</NBR><TEXT><![CDATA[  if (testing::failTeardown)]]></TEXT></\
LINE><LINE><NBR>1941</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "teardown failure", 0, 1)]]></TEXT></LINE><\
LINE><NBR>1942</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1943</NBR><TEXT><![CDATA[  if (testing::errorTeard\
own)]]></TEXT></LINE><LINE><NBR>1944</NBR><TEXT><![CDATA[    call cclutAssertI4Equal(CURREF, "teardown error", 1, 1)]]><\
/TEXT></LINE><LINE><NBR>1945</NBR><TEXT><![CDATA[    set guaranteedError = "teardown error"]]></TEXT></LINE><LINE><NBR>1\
946</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1947</NBR><TEXT><![CDATA[end ;;;setup]]></TEXT></LINE><LINE><\
NBR>1948</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1949</NBR><TEXT><![CDATA[subroutine testOne(null)]]></TEXT></LI\
NE><LINE><NBR>1950</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, "testOne executed", 0, 0)]]></TEXT></LINE><LINE\
><NBR>1951</NBR><TEXT><![CDATA[  if (testing::failTest)]]></TEXT></LINE><LINE><NBR>1952</NBR><TEXT><![CDATA[    call ccl\
utAssertI4Equal(CURREF, "testOne failure", 0, 1)]]></TEXT></LINE><LINE><NBR>1953</NBR><TEXT><![CDATA[  endif]]></TEXT></\
LINE><LINE><NBR>1954</NBR><TEXT><![CDATA[  if (testing::errorTest)]]></TEXT></LINE><LINE><NBR>1955</NBR><TEXT><![CDATA[ \
   call cclutAssertI4Equal(CURREF, "testOne error", 1, 1)]]></TEXT></LINE><LINE><NBR>1956</NBR><TEXT><![CDATA[    set gu\
aranteedError = "testOne error"]]></TEXT></LINE><LINE><NBR>1957</NBR><TEXT><![CDATA[  endif]]></TEXT></LINE><LINE><NBR>1\
958</NBR><TEXT><![CDATA[end ;;;testOne]]></TEXT></LINE><LINE><NBR>1959</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1\
960</NBR><TEXT><![CDATA[subroutine testTwo(null)]]></TEXT></LINE><LINE><NBR>1961</NBR><TEXT><![CDATA[  call cclutAssertI\
4Equal(CURREF, "testTwo executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1962</NBR><TEXT><![CDATA[  call cclutAssertVCEqual(C\
URREF, "testTwo executed", "A", "A")]]></TEXT></LINE><LINE><NBR>1963</NBR><TEXT><![CDATA[end ;;;testTwo]]></TEXT></LINE>\
<LINE><NBR>1964</NBR><TEXT><![CDATA[]]></TEXT></LINE><LINE><NBR>1965</NBR><TEXT><![CDATA[;; like testOne, but two failur\
es and two errors and errors come before failures when both are specified.]]></TEXT></LINE><LINE><NBR>1966</NBR><TEXT><!\
[CDATA[subroutine testThree(null)]]></TEXT></LINE><LINE><NBR>1967</NBR><TEXT><![CDATA[  call cclutAssertI4Equal(CURREF, \
"testThree executed", 0, 0)]]></TEXT></LINE><LINE><NBR>1968</NBR><TEXT><![CDATA[  if (testing::errorTest)]]></TEXT></LIN\
E><LINE><NBR>1969</NBR><TEXT><![CDATA[    set guaranteedError = "testThree error"]]></TEXT></LINE><LINE><NBR>1970</NBR><\
TEXT><![CDATA[    set guaranteedError = "testThree error 2"]]></TEXT></LINE><LINE><NBR>1971</NBR><TEXT><![CDATA[  endif]\
]></TEXT></LINE><LINE><NBR>1972</NBR><TEXT><![CDATA[  if (testing::failTest)]]></TEXT></LINE><LINE><NBR>1973</NBR><TEXT>\
<![CDATA[    call cclutAssertI4Equal(CURREF, "testThree failure A", 0, 1)]]></TEXT></LINE><LINE><NBR>1974</NBR><TEXT><![\
CDATA[    call cclutAssertI4Equal(CURREF, "testThree failure B", 0, 1)]]></TEXT></LINE><LINE><NBR>1975</NBR><TEXT><![CDA\
TA[  endif]]></TEXT></LINE><LINE><NBR>1976</NBR><TEXT><![CDATA[end ;;;testThree]]></TEXT></LINE></LINES></LISTING>~

;The next assert is a maintenance hastle when the cclut_execute_test_case_file code changes.
;Uncomment the echo statment to get the real value and update expectedListing accordingly.
;call echo(build2("listingXml = ", xml))
  call cclutAssertEndsWith(CURREF, "validate listingXml", expectedListing, xml)
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
  call cclutAssertVCEqual(CURREF, "validate line number 1.1", lineNbr, "1906")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 1.1", assertContext, "setupOnce executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 1.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 1.1", condition, ~"SO" = "SO"~)
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
  call cclutAssertVCEqual(CURREF, "validate line number 2.1", lineNbr, "1928")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.1", assertContext, "setup executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 2.1", condition, ~0 = 0~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 2.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 2.1", result, "PASSED")
  set nextAssertStart = 17 + textlen(xmlAssert) ;textlen(<ASSERT></ASSERT>) = 17
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", nextAssertStart, foundInd)

  call cclutAssertI4Equal(CURREF, "validate assert found 2.2", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 2.2", lineNbr, "1950")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.2", assertContext, "testOne executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 2.2", condition, ~0 = 0~)
  set result = cclut::retrieveXmlContent(xmlAssert, "result", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate result found 2.2", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate result 2.2", result, "PASSED")

  set nextAssertStart = nextAssertStart + 17 + textlen(xmlAssert)
  set xmlAssert = cclut::retrieveXmlContent(xmlAsserts, "assert", nextAssertStart, foundInd)

  call cclutAssertI4Equal(CURREF, "validate assert found 2.3", foundInd, TRUE)
  set lineNbr = cclut::retrieveXmlContent(xmlAssert, "lineNumber", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate line number found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate line number 2.3", lineNbr, "1939")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 2.3", assertContext, "teardown executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 2.3", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 2.3", condition, ~0 = 0~)
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
  call cclutAssertVCEqual(CURREF, "validate line number 5.1", lineNbr, "1917")
  set assertContext = cclut::retrieveXmlContent(xmlAssert, "context", 1, foundInd)
  call cclutAssertI4Equal(CURREF, "validate assert context found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate assert context 5.1", assertContext, "teardownOnce executed")
  if (legacyFormat = TRUE)
    set condition = cclut::retrieveXmlContent(xmlAssert, "test", 1, foundInd)
  else
    set condition = cclut::retrieveXmlContent(xmlAssert, "condition", 1, foundInd)
  endif
  call cclutAssertI4Equal(CURREF, "validate condition found 5.1", foundInd, TRUE)
  call cclutAssertVCEqual(CURREF, "validate condition 5.1", condition, ~"TO" = "TO"~)
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
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[1].asserts[1].condition, '"SO" = "SO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      response->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[2].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[2].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[2].asserts[2].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[2].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[2].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[2].asserts[3].resultInd, TRUE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[2].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[3].condition, '"A" = "A"')
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[3].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[3].asserts[4].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[4].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[4].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[4].asserts[2].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", response->tests[4].asserts[3].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[4].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", response->tests[4].asserts[3].resultInd, TRUE)
  
  call cclutAssertI4Equal(CURREF, "assert count", size(response->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(response->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", 
      response->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", response->tests[5].asserts[1].condition, '"TO" = "TO"')
  call cclutAssertI4Equal(CURREF, "assert value", 
      response->tests[5].asserts[1].resultInd, TRUE)
end ;;;validateNoIssuesResponse


