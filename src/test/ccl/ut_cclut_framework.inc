/**
  A test case file for testing the CCL Unit framework. This test case contains tests using the different types of 
  subroutine declaration syntax.   If you are trying to install the framework into a domain that has an older version 
  of CCL which does not support some of these constructs, the build will fail. 
  Remove this test case file from the build or delete the specific tests which are causing problems.
*/
declare setupOnce(null) = null with protect
declare teardownOnce(null) = null with protect
declare setup(null) = null with protect
declare teardown(null) = null with protect

declare testOne(null) = null with protect
declare testing::testTwo(null) = null with protect
declare doTestBody(null) = null with protect
declare getTestName(testNumber = i4) = vc with protect

record operations (
  1 setup[*]
    2 testName = vc
  1 teardown[*]
    2 testName = vc
  1 testRun[*]
    2 testName = vc
) with protect

declare setupOnceHasExecuted = i2 with protect, noconstant(FALSE)
declare _memory_reply_string = vc with protect, noconstant("")

subroutine setupOnce(null)
  set setupOnceHasExecuted = TRUE
end ;;;setupOnce

subroutine getTestName(testNumber)
  case(testNumber)
    of 1:
      return("TESTONE")
    of 2:
      return("TESTING::TESTTWO")
    of 3:
      return("TESTTHREE")
    of 4:
      return("TESTING::TESTFOUR")
    of 5:
      return("TESTFIVE")
    of 6:
      return("TESTING::TESTSIX")
  endcase
  return("")
end ;;;getTestName

subroutine teardownOnce(null)
  declare idx = i4 with protect, noconstant(0)
  declare testName = vc with protect, noconstant("")

  call cclutAssertI4Equal(CURREF, "teardownOnce setup size", size(operations->setup, 5), 6)
  call cclutAssertI4Equal(CURREF, "teardownOnce testRun size", size(operations->testRun, 5), 6)
  call cclutAssertI4Equal(CURREF, "teardownOnce teardown size", size(operations->teardown, 5), 6)
  
  for (idx = 1 to 6)
    set testName = getTestName(idx)
    call cclutAssertVCEqual(CURREF, build("setup-", idx), operations->setup[idx].testName, testName)
    call cclutAssertVCEqual(CURREF, build("testRun-", idx), operations->testRun[idx].testName, testName)
    call cclutAssertVCEqual(CURREF, build("teardown-", idx), operations->teardown[idx].testName, testName)
  endfor
end ;;;teardownOnce

subroutine setup(null)
  declare stat = i4 with protect, noconstant(0)
  
  set stat = alterList(operations->setup, 1 + size(operations->setup, 5))
  set operations->setup[size(operations->setup, 5)].testName = cclut::getTestName(null)
  call cclutAssertI2Equal(CURREF, build("setupOnceHasExecuted-", cclut::getTestName(null)), setupOnceHasExecuted, TRUE)
end ;;;setup

subroutine teardown(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterList(operations->teardown, 1 + size(operations->teardown, 5))
  set operations->teardown[size(operations->teardown, 5)].testName = cclut::getTestName(null)
end ;;;teardown

subroutine doTestBody(null)
  declare testName = vc with protect, constant(cclut::getTestName(null))
  declare stat = i4 with protect, noconstant(0)

  set stat = alterList(operations->testRun, 1 + size(operations->testRun, 5))
  set operations->testRun[size(operations->testRun, 5)].testName = testName
  execute ut_cclut_framework "MINE"
  set stat = cnvtjsontorec(_memory_reply_string)
  call cclutAssertVCEqual(CURREF, build("testName-", testName), testData->testName, testName)
  call cclutAssertVCEqual(CURREF, build("testNameX-", testName), getTestName(testData->testNumber), testName)
end ;;;doTestBody

;declared
subroutine testOne(null)
  call doTestBody(null)
end ;;;testOne

;declared with namespace
subroutine testing::testTwo(null)
  call doTestBody(null)
end ;;;testTwo

;declared in line
subroutine (testThree(null) = null)
  call doTestBody(null)
end ;;;testThree

;declared in line with namespace
subroutine (testing::testFour(null) = null)
  call doTestBody(null)
end ;;;testFour

;undeclared
subroutine testFive(null)
  call doTestBody(null)
end ;;;testFive

;undeclared with namespace
subroutine testing::testSix(null)
  call doTestBody(null)
end ;;;testFive
