;**********************************************************************************************************************************
;
; The final portion of the code for the manual test runner.
;
;**********************************************************************************************************************************


#exit_script


;check for dangling errors missed because of a failed assert
declare errorSource = vc with protect, noconstant("")
if (cclut_test_runner::setupOnceFinished = FALSE)
  set errorSource = "setupOnce"
elseif (cclut_test_runner::setupFinished = FALSE)
  set errorSource = "setup"
else
  set errorSource = cclut_test_runner::lastTestName
endif
call cclut_test_runner::checkForCclErrors(errorSource, FALSE)

;complete teardown execution if missed because of a failed assert
if (checkfun("TEARDOWN") = cclut_test_runner::FUN_TYPE_SUBROUTINE)
  if (cclut_test_runner::teardownStarted = FALSE)
    call cclut::doOutput("Executing teardown...")
    set cclut_test_runner::teardownStarted = TRUE    
    call teardown(null) 
    set cclut_test_runner::teardownFinished = TRUE
    call cclut_test_runner::checkForCclErrors("teardown", FALSE)
  elseif (cclut_test_runner::teardownFinished = FALSE)
    set cclut_test_runner::teardownFinished = TRUE
    call cclut_test_runner::checkForCclErrors("teardown", FALSE)
  endif
endif

;complete teardownOnce execution if missed because of a failed assert
call cclut_test_runner::checkForCclErrors(cclut_test_runner::lastTestName, FALSE)
if (checkfun("TEARDOWNONCE") = cclut_test_runner::FUN_TYPE_SUBROUTINE)
  if (cclut_test_runner::teardownOnceStarted = FALSE)
    set cclut_test_runner::teardownOnceStarted = TRUE
    call cclut::doOutput("Executing teardownOnce...")
    call teardownOnce(null) 
    set cclut_test_runner::teardownOnceFinished = TRUE
    call cclut_test_runner::checkForCclErrors("teardownOnce", FALSE)
  elseif (cclut_test_runner::teardownOnceFinished = FALSE)
    set cclut_test_runner::teardownOnceFinished = TRUE
    call cclut_test_runner::checkForCclErrors("teardownOnce", FALSE)
  endif
endif

call parser(concat("drop program ", cclut_ff::testProgramName, " go"))

if (cclut_test_runner::assertSuccess = TRUE AND cclut_test_runner::testRunnerSuccessInd = TRUE)
  call cclut::doOutput("**********************************")
  call cclut::doOutput("*                                *")
  call cclut::doOutput("*   cclut_ff:      SUCCESS       *")
  call cclut::doOutput("*                                *")
  call cclut::doOutput("**********************************")
else
  call cclut::doOutput("**********************************")
  call cclut::doOutput("*                                *")
  call cclut::doOutput("*   cclut_ff:      FAILURE       *")
  call cclut::doOutput("*                                *")
  call cclut::doOutput("**********************************")
endif