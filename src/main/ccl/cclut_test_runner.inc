;**********************************************************************************************************************************
;
; The initial portion of the code for the manual test runner.
;
;**********************************************************************************************************************************


%i cclsource:cclutassert_impl_ff.inc


declare cclut_test_runner::runUnitTest(cclutTestName = vc) = null with protect
declare cclut_test_runner::runMatchingUnitTests(cclutPattern = vc ) = null with protect
declare cclut_test_runner::runAllUnitTests(null) = null with protect
declare cclut_test_runner::checkForCclErrors(cclutLocation = vc, cclutDoExit = i2) = null with protect
  
;assert failures probably will not route the exit_script of this program, 
;but it will set cclut::assertSuccess equal to FALSE which will quickly get there.
declare cclut_test_runner::assertSuccess = i2 with protect, noconstant(TRUE)
declare cclut_test_runner::testRunnerSuccessInd = i2 with protect, noconstant(TRUE)
declare cclut_test_runner::lastTestName = vc with protect, noconstant("")
declare cclut_test_runner::setupOnceFinished = i2 with protect, noconstant(FALSE)
declare cclut_test_runner::setupFinished = i2 with protect, noconstant(FALSE)
declare cclut_test_runner::teardownStarted = i2 with protect, noconstant(TRUE)
declare cclut_test_runner::teardownFinished = i2 with protect, noconstant(TRUE)
declare cclut_test_runner::teardownOnceStarted = i2 with protect, noconstant(FALSE)
declare cclut_test_runner::teardownOnceFinished = i2 with protect, noconstant(FALSE)
;declare cclutStat = i4 with protect, noconstant(0) with protect

/**
  Checks for CCL errors.
  @param cclutLocation 
    The name of the unit test that caused the error.
  @param
    Boolean flag indicating whether to route execution to exit_script if an error does occur.
*/
subroutine cclut_test_runner::checkForCclErrors(cclutLocation, cclutDoExit)
  declare cclutErrorMessage = vc with protect, noconstant("")
  declare cclutErrorCode = i4 with protect, noconstant(0)
  declare cclutErrorFlag = i2 with protect, noconstant(FALSE)
  set cclutErrorCode = error(cclutErrorMessage, 0)
  while (cclutErrorCode != 0)
    set cclutErrorFlag = TRUE
    call cclut::doOutput(concat("CCL error in ", cclutLocation))
    call cclut::doOutput(cclutErrorMessage)
    set cclutErrorCode = error(cclutErrorMessage, 0)
  endwhile
  if (cclutErrorFlag)
    set cclut_test_runner::testRunnerSuccessInd = FALSE
    if (cclutDoExit)
      go to exit_script
    endif
  endif
end ;;;checkForCCLErrors 

/**
  Runs a specified unit test
  @ param cclutTestName
    The name of the test to run.
*/
subroutine cclut_test_runner::runUnitTest(cclutTestName)
  call cclut::doOutput(build2("running test ", cclutTestName))
  declare cclutParserString = vc with protect, noconstant("")
  set cclutParserString = concat("call ", cclutTestName, "(null) go")

  set cclut_test_runner::teardownStarted = FALSE
  set cclut_test_runner::teardownFinished = FALSE
  if (checkfun("SETUP") = cclut_test_runner::FUN_TYPE_SUBROUTINE)
    call cclut::doOutput("Executing setup...")
    call setup(null) 
    set cclut_test_runner::setupFinished = TRUE
    call cclut_test_runner::checkForCclErrors("setup", TRUE)
  endif
  set cclut_test_runner::lastTestName = cclutTestName
  call cclut::doOutput(build2("Executing ", cclutTestName, "..."))
  call parser(cclutParserString)
  call cclut_test_runner::checkForCclErrors(cclutTestName, TRUE)
  if (checkfun("TEARDOWN") = cclut_test_runner::FUN_TYPE_SUBROUTINE) 
    call cclut::doOutput("Executing teardown...")
    set cclut_test_runner::teardownStarted = TRUE
    call teardown(null) 
    set cclut_test_runner::teardownFinished = TRUE
    call cclut_test_runner::checkForCclErrors("teardown", TRUE)
  endif  
  if (cclut_test_runner::assertSuccess = FALSE)
    set cclut_test_runner::testRunnerSuccessInd = FALSE
    go to exit_script
  endif
end ;;;runUnitTest


/**
* Runs unit tests matching a given name pattern
*/
subroutine cclut_test_runner::runMatchingUnitTests(cclutPattern)
  record cclutRequest (
    1 programName = vc
  ) with protect
 
  record cclutReply (
    1 tests[*] 
      2 testName = vc
%i cclsource:status_block.inc
  ) with protect
  
  set cclutRequest->programName = curprog
  execute cclut_find_unit_tests
  call cclut_test_runner::checkForCclErrors("runMatchingUnitTests", TRUE)
  if (checkfun("SETUPONCE") = cclut_test_runner::FUN_TYPE_SUBROUTINE)
    call cclut::doOutput("Executing setupOnce...")
    call setupOnce(null)
    set cclut_test_runner::setupOnceFinished = TRUE
    call cclut_test_runner::checkForCclErrors("setupOnce", TRUE)
  endif
  declare runMatchingUnitTests::idx = i4 with protect, noconstant(0)
  declare runMatchingUnitTests::pos = i4 with protect, noconstant(0)
  declare runMatchingUnitTests::idxTest = i4 with protect, noconstant(0)
  declare runMatchingUnitTests::testName = vc with protect, noconstant("")
  for (runMatchingUnitTests::idxTest = 1 to size(cclutReply->tests, 5))
    set runMatchingUnitTests::testName = cclutReply->tests[runMatchingUnitTests::idxTest].testname
    if (operator(trim(cnvtlower(runMatchingUnitTests::testName), 3), "regexplike", trim(cnvtlower(cclutPattern), 3)))
      set runMatchingUnitTests::pos = locateVal(runMatchingUnitTests::idx, 1, 
          runMatchingUnitTests::idxTest-1, runMatchingUnitTests::testName, cclutReply->tests[runMatchingUnitTests::idx].testname)
      if (runMatchingUnitTests::pos = 0)
        call cclut_test_runner::runUnitTest(runMatchingUnitTests::testName)
      endif
    endif
  endfor
  if (checkfun("TEARDOWNONCE") = cclut_test_runner::FUN_TYPE_SUBROUTINE) 
    call cclut::doOutput("Executing teardownOnce...")
    set cclut_test_runner::teardownOnceStarted = TRUE
    call teardownOnce(null) 
    set cclut_test_runner::teardownOnceFinished = TRUE
    call cclut_test_runner::checkForCclErrors("teardownOnce", TRUE)
  endif    
end ;;;runMatchingUnitTests


/**
* Runs every unit test
*/
subroutine cclut_test_runner::runAllUnitTests(null)
  call cclut_test_runner::runMatchingUnitTests(".*")
end ;;;runAllUnitTests

set trace deprecated value(validate(cclut_test_runner::deprecatedFlag, "E"))
set modify predeclare
