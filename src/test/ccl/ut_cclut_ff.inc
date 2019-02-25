/**
  Unit Tests for the cclut script
*/

%i cclsource:cclut_get_file_as_string.inc

declare captureEcho(msg = vc) = null with protect
declare captureEcho2(msg = vc) = null with protect
declare testing::checkMessages(null) = null with protect
declare testing::checkMRS(mrs = vc) = null with protect
declare setup(null) = null with protect
declare testNoIssues(null) = null with protect
declare testNoIssuesCBO(null) = null with protect
declare testNoIssuesTestTwoAndThreeRBO(null) = null with protect
declare testFailTest(null) = null with protect
declare testSetupError(null) = null with protect
declare testSetupOnceErrorFailTest(null) = null with protect
declare testNoIssuesNotMuchSpecified(null) = null with protect
declare testTestCaseFileNameRequired(null) = null with protect
declare testFrameworkFailure(null) = null with protect
declare testNoTests(null) = null with protect
declare testAllTestsFail(null) = null with protect
declare testTeardownAndTeardownOnceFail(null) = null with protect
declare testTeardownAndTeardownOnceError(null) = null with protect
declare testErroredTests(null) = null with protect
declare testErroredAndFailedTestsA(null) = null with protect
declare testErroredAndFailedTestsB(null) = null with protect
declare testCclutffErrorsLate(null) = null with protect
declare testFailedCompile(null) = null with protect


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

record testing::echoMessages (
  1 message[*] 
    2 value = vc
) with protect

record testing::expectedMessages (
  1 message[*] 
    2 value = vc
    2 type = i4
) with protect

declare testing::ASSERT_TYPE_EQUAL = i4 with protect, constant(0)
declare testing::ASSERT_TYPE_DATE = i4 with protect, constant(1)
declare testing::ASSERT_TYPE_MATCH = i4 with protect, constant(2)


/**
  Override for the echo function to capture the message before echoing it
*/
subroutine captureEcho(msg)
  declare stat = i4 with protect, noconstant(0)
  declare messageCount = i4 with protect, noconstant(0)
  
  set messageCount = 1 + size(testing::echoMessages->message, 5)
  set stat = alterlist(testing::echoMessages->message, messageCount)
  set testing::echoMessages->message[messageCount].value = msg
  call echo(msg)
  return(null)
end ;;;captureEcho


/**
  Override for the echo function which captures the message before echoing it and which causes a runtime error
  if the message is "*   cclut_ff:      SUCCESS       *"
*/
subroutine captureEcho2(msg)
  declare contrivedError = i4 with protect, noconstant(0)
  call captureEcho(msg)
  if (msg = "*   cclut_ff:      SUCCESS       *")
    set contrivedError = "this is a contrived error"
  endif
  return(null)
end ;;;captureEcho


subroutine testing::checkMessages(null)
  declare count = i4 with protect, noconstant(0)
  declare idx = i4 with protect, noconstant(0)
  declare matches = i2 with protect, noconstant(FALSE)

  set count = size(testing::expectedMessages->message, 5)
  call cclutAssertI4Equal(CURREF, "message count", size(testing::echoMessages->message, 5), count)
  
  for (idx = 1 to count)
    case(testing::expectedMessages->message[idx].type)
      of testing::ASSERT_TYPE_EQUAL:
        call cclutAssertVCEqual(CURREF, build2("message-",trim(build(idx),3)), testing::echoMessages->message[idx].value, 
            testing::expectedMessages->message[idx].value)
      of testing::ASSERT_TYPE_MATCH:
        call cclutAssertVCOperator(CURREF, build2("message-",trim(build(idx),3)), 
            testing::echoMessages->message[idx].value, "regexplike", testing::expectedMessages->message[idx].value)
    endcase
  endfor
end ;;;checkMessages


subroutine testing::checkMRS(mrs)
  declare line = vc with protect, noconstant("")
  declare nl = vc with protect, constant(concat(char(10), char(13)))
  declare nlPosLast = i4 with protect, noconstant(-1)
  declare nlPos = i4 with protect, noconstant(0)
  declare idx = i4 with protect, noconstant(0)
  declare count = i4 with protect, noconstant(0)
  declare matches = i2 with protect, noconstant(FALSE)

  set nlPos = findstring(nl, mrs, 1+nlPos)
  while (nlPos > 0)
    set idx = idx + 1
    set line = substring(2+nlPosLast, nlPos-nlPosLast-2, mrs)\
    case(testing::expectedMessages->message[idx].type)
      of testing::ASSERT_TYPE_EQUAL:
        call cclutAssertVCEqual(CURREF, build2("line-",trim(build(idx),3)), line, testing::expectedMessages->message[idx].value)
      of testing::ASSERT_TYPE_MATCH:
        set matches = operator(line, "regexplike", testing::expectedMessages->message[idx].value)
        call cclutAssertI2Equal(CURREF, build2("message-",trim(build(idx),3)), matches, TRUE)
    endcase
    set nlPosLast = nlPos
    set nlPos = findstring(nl, mrs, 1+nlPos)
  endwhile
  
  set count = size(testing::expectedMessages->message, 5)
  call cclutAssertI4Equal(CURREF, "message count", size(testing::echoMessages->message, 5), count)
  
  for (idx = 1 to count)
    case(testing::expectedMessages->message[idx].type)
    of testing::ASSERT_TYPE_EQUAL:
      call cclutAssertVCEqual(CURREF, build2("message-",trim(build(idx),3)), testing::echoMessages->message[idx].value, 
        testing::expectedMessages->message[idx].value)
    of testing::ASSERT_TYPE_MATCH:
      set matches = operator(testing::echoMessages->message[idx].value, "regexplike", 
          testing::expectedMessages->message[idx].value)
      call cclutAssertI2Equal(CURREF, build2("message-",trim(build(idx),3)), matches, TRUE)
    endcase
  endfor
end ;;;checkMRS


/**
  per-test initialization
*/
subroutine setup(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = initRec(testing::echoMessages)
  set stat = initRec(testing::expectedMessages)

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
  Test the happy path with no optimizer mode specified
*/
subroutine testNoIssues(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 19)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTWO"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTWO..."
  set testing::expectedMessages->message[9].value = "Executing teardown..."
  set testing::expectedMessages->message[10].value = "running test TESTTHREE"
  set testing::expectedMessages->message[11].value = "Executing setup..."
  set testing::expectedMessages->message[12].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[13].value = "Executing teardown..."
  set testing::expectedMessages->message[14].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[15].value = "**********************************"
  set testing::expectedMessages->message[16].value = "*                                *"
  set testing::expectedMessages->message[17].value = "*   cclut_ff:      SUCCESS       *"
  set testing::expectedMessages->message[18].value = "*                                *"
  set testing::expectedMessages->message[19].value = "**********************************"

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testNoIssues


/**
  Test the happy path with optimizer mode CBO specified
*/
subroutine testNoIssuesCBO(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 19)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTWO"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTWO..."
  set testing::expectedMessages->message[9].value = "Executing teardown..."
  set testing::expectedMessages->message[10].value = "running test TESTTHREE"
  set testing::expectedMessages->message[11].value = "Executing setup..."
  set testing::expectedMessages->message[12].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[13].value = "Executing teardown..."
  set testing::expectedMessages->message[14].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[15].value = "**********************************"
  set testing::expectedMessages->message[16].value = "*                                *"
  set testing::expectedMessages->message[17].value = "*   cclut_ff:      SUCCESS       *"
  set testing::expectedMessages->message[18].value = "*                                *"
  set testing::expectedMessages->message[19].value = "**********************************"

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo.inc", "", "CBO" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testNoIssuesCBO


/**
  Test the happy path with specific tests and optimizer mode RBO specified
*/
subroutine testNoIssuesTestTwoAndThreeRBO(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 15)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTTWO"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTTWO..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTHREE"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[9].value = "Executing teardown..."
  set testing::expectedMessages->message[10].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[11].value = "**********************************"
  set testing::expectedMessages->message[12].value = "*                                *"
  set testing::expectedMessages->message[13].value = "*   cclut_ff:      SUCCESS       *"
  set testing::expectedMessages->message[14].value = "*                                *"
  set testing::expectedMessages->message[15].value = "**********************************"

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(two|three)", "RBO", "W" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testNoIssuesCBO


/**
  Test a case where the test cases have assert failures
*/
subroutine testFailTest(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 12)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[5].value = 
      "FAILURE: line number \[[0-9]+\]; context \[testOne failure\]; condition \[0 = 1"
  set testing::expectedMessages->message[6].value = "Executing teardown..."
  set testing::expectedMessages->message[7].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[8].value = "**********************************"
  set testing::expectedMessages->message[9].value = "*                                *"
  set testing::expectedMessages->message[10].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[11].value = "*                                *"
  set testing::expectedMessages->message[12].value = "**********************************"

  set testing::failTest = TRUE
  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "CBO" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testTestsFail


/**
  Test a case where the setup routine throws errors
*/
subroutine testSetupError(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 12)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "CCL error in setup"
  set testing::expectedMessages->message[5].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[5].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[6].value = "Executing teardown..."
  set testing::expectedMessages->message[7].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[8].value = "**********************************"
  set testing::expectedMessages->message[9].value = "*                                *"
  set testing::expectedMessages->message[10].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[11].value = "*                                *"
  set testing::expectedMessages->message[12].value = "**********************************"

  set testing::errorSetup = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testSetupError


/**
  Test a case where the setupOnce routine throws errors and the tests have assert failures
*/
subroutine testSetupOnceErrorFailTest(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 9)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "CCL error in setupOnce"
  set testing::expectedMessages->message[3].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[3].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[4].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[5].value = "**********************************"
  set testing::expectedMessages->message[6].value = "*                                *"
  set testing::expectedMessages->message[7].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[8].value = "*                                *"
  set testing::expectedMessages->message[9].value = "**********************************"

  set testing::errorSetupOnce = TRUE
  set testing::failTest = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testSetupOnceErrorFailTest

/**
  Test the happy path with nothing specified but the test case.
*/
subroutine testNoIssuesNotMuchSpecified(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 19)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTWO"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTWO..."
  set testing::expectedMessages->message[9].value = "Executing teardown..."
  set testing::expectedMessages->message[10].value = "running test TESTTHREE"
  set testing::expectedMessages->message[11].value = "Executing setup..."
  set testing::expectedMessages->message[12].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[13].value = "Executing teardown..."
  set testing::expectedMessages->message[14].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[15].value = "**********************************"
  set testing::expectedMessages->message[16].value = "*                                *"
  set testing::expectedMessages->message[17].value = "*   cclut_ff:      SUCCESS       *"
  set testing::expectedMessages->message[18].value = "*                                *"
  set testing::expectedMessages->message[19].value = "**********************************"

  execute cclut_ff "ut_cclut_output", "", "ut_cclut_suo_su_td_tdo", "", "", "" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testNoIssuesNotMuchSpecified


/**
  Test that a test case name must be provided.
*/
subroutine testTestCaseFileNameRequired(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 1)
  set testing::expectedMessages->message[1].value = "A test case name must be provided"

  execute cclut_ff "ut_cclut_output", "ut_cclut_suo_su_td_tdo", "" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testtestCaseFileNameRequired



/**
  Test a framework failure.
*/
subroutine testFrameworkFailure(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 1)
  set testing::expectedMessages->message[1].value = 
      "%CCL-E-151-CCLUT_FF\([^)]+\)[0-9]+:[0-9]+\{Z_SET.\}TRACE_NOT is not previously defined.";^
  set testing::expectedMessages->message[1].type = testing::ASSERT_TYPE_MATCH

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("ECHO", captureEcho),
      replace("TRACE", trace_not)
  call testing::checkMessages(null)
end ;;;testtestCaseFileNameRequired

/**
  Test no tests
*/
subroutine testNoTests(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 5)
  set testing::expectedMessages->message[1].value = "**********************************"
  set testing::expectedMessages->message[2].value = "*                                *"
  set testing::expectedMessages->message[3].value = "*   cclut_ff:      SUCCESS       *"
  set testing::expectedMessages->message[4].value = "*                                *"
  set testing::expectedMessages->message[5].value = "**********************************"

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_no_tests" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testNoTests


/**
  Test no tests pass
*/
subroutine testAllTestsFail(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 8)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[2].value = 
      "FAILURE: line number \[[0-9]+\]; context \[setupOnce failure\]; condition \[0.*"    
  set testing::expectedMessages->message[3].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[4].value = "**********************************"
  set testing::expectedMessages->message[5].value = "*                                *"
  set testing::expectedMessages->message[6].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[7].value = "*                                *"
  set testing::expectedMessages->message[8].value = "**********************************"

  set testing::failSetupOnce = TRUE
  set testing::failTest = TRUE
  set testing::failTeardownOnce = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testAllTestsFail



/**
  Test teardown and teardownOnce failures
*/
subroutine testTeardownAndTeardownOnceFail(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 12)

  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[6].value = 
      "FAILURE: line number \[[0-9]+\]; context \[teardown failure\]; condition \[0 = 1.*"
  set testing::expectedMessages->message[7].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[8].value = "**********************************"
  set testing::expectedMessages->message[9].value = "*                                *"
  set testing::expectedMessages->message[10].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[11].value = "*                                *"
  set testing::expectedMessages->message[12].value = "**********************************"

  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testTeardownAndTeardownOnceFail


/**
  Test teardown and teardownOnce produce errors
*/
subroutine testTeardownAndTeardownOnceError(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 15)

  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "CCL error in teardown"
  set testing::expectedMessages->message[7].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[7].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[8].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[9].value = "CCL error in teardownOnce"    
  set testing::expectedMessages->message[10].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[10].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[11].value = "**********************************"
  set testing::expectedMessages->message[12].value = "*                                *"
  set testing::expectedMessages->message[13].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[14].value = "*                                *"
  set testing::expectedMessages->message[15].value = "**********************************"

  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testTeardownAndTeardownOnceError

/**
  Test when the tests error out.
*/
subroutine testErroredTests(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 19)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTTWO"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTTWO..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTHREE"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[9].value = "CCL error in TESTTHREE"    
  set testing::expectedMessages->message[10].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[10].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[11].value = "CCL error in TESTTHREE"    
  set testing::expectedMessages->message[12].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[12].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[13].value = "Executing teardown..."
  set testing::expectedMessages->message[14].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[15].value = "**********************************"
  set testing::expectedMessages->message[16].value = "*                                *"
  set testing::expectedMessages->message[17].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[18].value = "*                                *"
  set testing::expectedMessages->message[19].value = "**********************************"


  set testing::errorTest = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(two|three)" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
end ;;;testErroredTests


/**
  Test when a test fails first errors second.
*/
subroutine testErroredAndFailedTestsA(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::expectedMessages->message, 12)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."

  set testing::expectedMessages->message[5].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[5].value = 
      "FAILURE: line number \[[0-9]+\]; context \[testOne failure\]; condition \[0 = 1.*"
  set testing::expectedMessages->message[6].value = "Executing teardown..."
  set testing::expectedMessages->message[7].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[8].value = "**********************************"
  set testing::expectedMessages->message[9].value = "*                                *"
  set testing::expectedMessages->message[10].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[11].value = "*                                *"
  set testing::expectedMessages->message[12].value = "**********************************"


  set testing::failTest = TRUE
  set testing::errorTest = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|two)" with
      replace("ECHO", captureEcho)
    call testing::checkMessages(null)
end ;;;testErroredAndFailedTestsA


/**
  Test when a test errors first and fails second.
*/
subroutine testErroredAndFailedTestsB(null)
  declare stat = i4 with protect, noconstant(0)
  declare _memory_reply_string = vc with protect, noconstant("")

  set stat = alterlist(testing::expectedMessages->message, 20)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTTWO"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTTWO..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTHREE"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[9].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[9].value = 
      "FAILURE: line number \[[0-9]+\]; context \[testThree failure A\]; condition \[0 = 1.*"
  set testing::expectedMessages->message[10].value = "CCL error in TESTTHREE"    
  set testing::expectedMessages->message[11].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[11].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[12].value = "CCL error in TESTTHREE"    
  set testing::expectedMessages->message[13].value = concat(
    "%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
    "\{GUARANTEEDERROR\}Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  set testing::expectedMessages->message[13].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[14].value = "Executing teardown..."
  set testing::expectedMessages->message[15].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[16].value = "**********************************"
  set testing::expectedMessages->message[17].value = "*                                *"
  set testing::expectedMessages->message[18].value = "*   cclut_ff:      FAILURE       *"
  set testing::expectedMessages->message[19].value = "*                                *"
  set testing::expectedMessages->message[20].value = "**********************************"


  set testing::failTest = TRUE
  set testing::errorTest = TRUE

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(two|three)" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
  call testing::checkMRS(_memory_reply_string)
end ;;;testErroredAndFailedTestsB


/**
  Test cclut_ff errors after the tests complete
*/
subroutine testCclutffErrorsLate(null)
  declare stat = i4 with protect, noconstant(0)
  declare _memory_reply_string = vc with protect, noconstant("")

  set stat = alterlist(testing::expectedMessages->message, 16)
  set testing::expectedMessages->message[1].value = "Executing setupOnce..."
  set testing::expectedMessages->message[2].value = "running test TESTONE"
  set testing::expectedMessages->message[3].value = "Executing setup..."
  set testing::expectedMessages->message[4].value = "Executing TESTONE..."
  set testing::expectedMessages->message[5].value = "Executing teardown..."
  set testing::expectedMessages->message[6].value = "running test TESTTHREE"
  set testing::expectedMessages->message[7].value = "Executing setup..."
  set testing::expectedMessages->message[8].value = "Executing TESTTHREE..."
  set testing::expectedMessages->message[9].value = "Executing teardown..."
  set testing::expectedMessages->message[10].value = "Executing teardownOnce..."
  set testing::expectedMessages->message[11].value = "**********************************"
  set testing::expectedMessages->message[12].value = "*                                *"
  set testing::expectedMessages->message[13].value = "*   cclut_ff:      SUCCESS       *"
  set testing::expectedMessages->message[14].value = "*                                *"
  set testing::expectedMessages->message[15].value = "**********************************"
  set testing::expectedMessages->message[16].value = concat("%CCL-E-94-CCLUT_[0-9]+\([^)]+\)[0-9]+:[0-9]+",;^
      "\{CONTRIVEDERROR\}Assignment of Report expression \(CONTRIVEDERROR\) to incompatible type.") 
  set testing::expectedMessages->message[16].type = testing::ASSERT_TYPE_MATCH
      
  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("ECHO", captureEcho2)
  call testing::checkMessages(null)
  call testing::checkMRS(_memory_reply_string)
end ;;;testCclutffErrorsLate


/**
  Test a failed compile
*/
subroutine testFailedCompile(null)
  declare stat = i4 with protect, noconstant(0)
  declare _memory_reply_string = vc with protect, noconstant("")

  set stat = alterlist(testing::expectedMessages->message, 4)
  set testing::expectedMessages->message[1].value = 
      "%CCL-E-51-CCLUT_FF\([^)]+\)[0-9]+:[0-9]+\{COMPILE\(\)\}This is an invalid \(COMPILE\).";^
  set testing::expectedMessages->message[1].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[2].value = 
      "%CCL-E-9-CCLUT_FF\([^)]+\)[0-9]+:[0-9]+\{DECLARE\}Unexpected symbol found.";^
  set testing::expectedMessages->message[2].type = testing::ASSERT_TYPE_MATCH
  set testing::expectedMessages->message[3].value = "Failed to generate the test case program."
  set testing::expectedMessages->message[4].value = "Check the listing file cclut_[0-9]+\.lis"
  set testing::expectedMessages->message[4].type = testing::ASSERT_TYPE_MATCH
      
  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_faulty_test_case" with
      replace("ECHO", captureEcho)
  call testing::checkMessages(null)
  call testing::checkMRS(_memory_reply_string)
end ;;;testFailedCompile

/**
  Test that the framework removes any mock tables that test cases fail to do when calling cclut_ff.
*/
subroutine testMockTableCleanup(null)
  declare cclutMockTableName = vc with protect, noconstant("uninitialized")

  execute cclut_ff "ut_cclut_output", "cclsource", "ut_cclut_mock_table_misbehaving_cleanup"
call echo(build2("cclutMockTableName after: ", cclutMockTableName))        

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
