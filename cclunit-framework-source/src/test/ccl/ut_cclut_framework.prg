drop program ut_cclut_framework:dba go
create program ut_cclut_framework:dba
/**
  A prompt program that is called by some unit tests in the ut_cclut_framework test case.
*/
  prompt "Output Location: " = "MINE" with outdev
  
  record testData (
    1 testName = vc
    1 testNumber = i4
  ) with protect
  
  declare testName = vc with protect, constant(cclut::getTestName(null))
  declare testNumber = i4 with protect, noconstant(0)

  case (testName)
    of "TESTONE":
      set testNumber = 1
    of "TESTING::TESTTWO":
      set testNumber = 2
    of "TESTTHREE":
      set testNumber = 3
    of "TESTING::TESTFOUR":
      set testNumber = 4
    of "TESTFIVE":
      set testNumber = 5
    of "TESTING::TESTSIX":
      set testNumber = 6
  endcase
  
  set testData->testName = testName
  set testData->testNumber = testNumber
  set _memory_reply_string = cnvtrectojson(testData)  
end go
