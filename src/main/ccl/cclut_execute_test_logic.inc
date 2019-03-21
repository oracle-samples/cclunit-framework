%#ifndef CCLUT_EXECUTE_TEST_LOGIC_INC
%#def CCLUT_EXECUTE_TEST_LOGIC_INC
;**********************************************************************************************************************************
;
; Contains the necessary logic to find and execute the tests in a program compiled from a test case file and to evaluate 
; the results. If a non-empty testNamePattern is provied in the testing program's request, then only the matching tests are 
; executed. If the testing program contains a setup subroutine, it is executed before each test. If the testing program contains 
; a teardown subroutinem, it is executed after each test. If the testing program contains a setupOnce subroutine, it is 
; executed before any test is executed. If the testing program contains a teardownOnce subroutinem, it is executed after 
; all test have completed.
;
; Note that this file is intended to be included in the testing program. 
;
;**********************************************************************************************************************************
 
declare cclut::evaluateAsserts(cclutTestIndex = i4, cclutPPReplyStruct = vc(ref), cclutPPAssertStruct = vc(ref)) = i2 with protect
declare cclut::evaluateTimers(cclutTimers = vc(ref)) = null with protect
declare cclut::evaluateTestResults(
   cclutTestIndex = i4, cclutPReplyStruct = vc(ref), cclutPAssertStruct = vc(ref), cclutPTimerStruct = vc(ref)) = i2 with protect
declare cclut::executeTestLogic(null) = null with protect
  

/**
  Copies the assert statements from cclutPPAssertStruct to cclutPReplyStruct checking if any asserts failed 
  then clears cclutPPAssertStruct.
  @param cclutTestIndex 
    The index of unit test within the cclutPReplyStruct that contains the asserts.
  @param cclutPReplyStruct 
    The reply that stores all unit tests and their associated assert statements.
  @param cclutPPAssertStruct 
    The record structure that contains all the asserts that were executed. 
    This structure will be reinitialized by this subroutine.
  @retutns
    A boolean flag indicating that all the asserts passed and no errors occurred.
    @value TRUE
      All that asserts passed. 
    @value FALSE
      At least one assert failed.
*/
subroutine cclut::evaluateAsserts(cclutTestIndex, cclutPPReplyStruct, cclutPPAssertStruct)
  declare cclutResultValue = i2 with protect, noconstant(TRUE)
  declare cclutAssertIndex = i4 with protect, noconstant(0)
  declare cclutAssertCount = i4 with protect, noconstant(size(cclutPPAssertStruct->line, 5))
  declare cclutReplyAssertIndex = i4 with protect, noconstant(size(cclutPPReplyStruct->tests[cclutTestIndex].asserts, 5))
  declare cclutErrorCount = i4 with protect, noconstant(0)
  declare cclutErrorMessage = vc with protect, noconstant("")
  declare cclutStat = i4 with protect, noconstant(0) 

  if (cclut::errorOccurred(cclutErrorMessage))
    set cclutErrorCount = size(cclutPPReplyStruct->tests[cclutTestIndex].errors, 5) + 1
    set cclutStat = alterlist(cclutPPReplyStruct->tests[cclutTestIndex].errors, cclutErrorCount)
    set cclutPPReplyStruct->tests[cclutTestIndex].errors[cclutErrorCount].lineNumber =
        cclut::getCclErrorLineNumber(cclutErrorMessage)
    set cclutPPReplyStruct->tests[cclutTestIndex].errors[cclutErrorCount].errorText = cclutErrorMessage

    set cclutResultValue = FALSE
  endif

  set cclutStat = alterlist(cclutPPReplyStruct->tests[cclutTestIndex].asserts, cclutReplyAssertIndex + cclutAssertCount)

  ;Append all assert to the reply structure.
  for (cclutAssertIndex = 1 to cclutAssertCount)
    set cclutReplyAssertIndex = cclutReplyAssertIndex + 1
    set cclutPPReplyStruct->tests[cclutTestIndex].asserts[cclutReplyAssertIndex].lineNumber = 
        cclutPPAssertStruct->line[cclutAssertIndex].lineNumber
    set cclutPPReplyStruct->tests[cclutTestIndex].asserts[cclutReplyAssertIndex].context = 
        cclutPPAssertStruct->line[cclutAssertIndex].context
    set cclutPPReplyStruct->tests[cclutTestIndex].asserts[cclutReplyAssertIndex].condition = 
        cclutPPAssertStruct->line[cclutAssertIndex].condition
    set cclutPPReplyStruct->tests[cclutTestIndex].asserts[cclutReplyAssertIndex].resultInd =
        evaluate(cclutPPAssertStruct->line[cclutAssertIndex].result, "PASS", TRUE, "FAIL", FALSE)
    set cclutResultValue = 
        band(cclutResultValue, cclutPPReplyStruct->tests[cclutTestIndex].asserts[cclutReplyAssertIndex].resultInd)
 
    ;if we encountered an error before or after the assert, add it to the error list
    if (cclutPPAssertStruct->line[cclutAssertIndex].errorCodeBefore > 0)
      set cclutErrorCount = size(cclutPPReplyStruct->tests[cclutTestIndex].errors, 5) + 1
      set cclutStat = alterlist(cclutPPReplyStruct->tests[cclutTestIndex].errors, cclutErrorCount)
      set cclutPPReplyStruct->tests[cclutTestIndex].errors[cclutErrorCount].lineNumber = 
          cclut::getCclErrorLineNumber(cclutPPAssertStruct->line[cclutAssertIndex].errorMessageBefore)
      set cclutPPReplyStruct->tests[cclutTestIndex].errors[cclutErrorCount].errorText = 
          cclutPPAssertStruct->line[cclutAssertIndex].errorMessageBefore
      set cclutResultValue = FALSE
    endif
 
    if (cclutPPAssertStruct->line[cclutAssertIndex].errorCode > 0)
      set cclutErrorCount = size(cclutPPReplyStruct->tests[cclutTestIndex].errors, 5) + 1
      set cclutStat = alterlist(cclutPPReplyStruct->tests[cclutTestIndex].errors, cclutErrorCount)
      set cclutPPReplyStruct->tests[cclutTestIndex].errors[cclutErrorCount].lineNumber =
          cclut::getCclErrorLineNumber(cclutPPAssertStruct->line[cclutAssertIndex].errorMessage)
      set cclutPPReplyStruct->tests[cclutTestIndex].errors[cclutErrorCount].errorText = 
          cclutPPAssertStruct->line[cclutAssertIndex].errorMessage
      set cclutResultValue = FALSE
    endif
  endfor
 
  set cclutStat = initrec(cclutPPAssertStruct)

  return (cclutResultValue)
end ;;;evaluateAsserts

/**
  Executes for each provided timer an assert statement that validates the timer's actual elapse did not exceed its maximum elapse.
  If the startTime or endTime is not specified for any of the timers then a CCL exception is thrown.
  @param cclutTimers 
    A list of timers to check. See cclut_timerList for the structure definition.
*/
subroutine cclut::evaluateTimers(cclutTimers)
  declare cclutLimit = f8 with protect, noconstant(0.00000)
  declare cclutElapse = f8 with protect, noconstant(0.0000)
  declare cclutTimerIndex = i4 with protect, noconstant(0)
  declare cclutTimerName = vc with protect, noconstant("")
  declare cclutLineNumber = i4 with protect, noconstant(0)
 
  ;make sure all timers pass their maximum allowed time
  for (cclutTimerIndex = 1 to size(cclutTimers->timer, 5))
    set cclutTimerName = trim(cclutTimers->timer[cclutTimerIndex].name, 3)
    set cclutLineNumber = cclutTimers->timer[cclutTimerIndex].lineNumber
    set cclutLimit = cclutTimers->timer[cclutTimerIndex].limit
    set cclutElapse = cclutGetTimerElapse(cclutTimers->timer[cclutTimerIndex].name)
    
    ;If we got a valid time back, then assert the timer values
    if (cclutElapse != -1)
      call cclutAssertf8NotGreaterThan(cclutLineNumber, concat(cclutTimerName, " - Actual <= Maximum"), cclutElapse, cclutLimit)
    else
      call cclexception(
          100, "E", concat("evaluateTimers() - Unable to determine elapsed time for timer: ", trim(cclutTimerName, 3)))
    endif
  endfor
end ;;;evaluateTimers

/**
  Evaluates the results of running a unit test or a preparation or cleanup step inserting into cclutPReplyStruct 
  the last errors not captured by an assert if there are any as well as all evaluated asserts and timers.
  @param cclutTestIndex
    The index of the test case in cclutPReplyStruct to be updated. 
  @param cclutPReplyStruct
    The reply structure into which the results will be inserted.
  @param cclutPAssertStruct 
    A record structure that contains all the asserts that were executed. 
    The structure will be reinitialized when this subroutine returns.
  @parm cclutPTimerStruct
    The list of timers to check.
    The structure will be reinitialized when this subroutine returns.
  @retutns
    A boolean flag indicating that all asserts passed, all timer limits were met and no errors occurred.
    @value TRUE
      All asserts and timer limits passed without error. 
    @value FALSE
      Some assert or timer limit failed or and error occurred.
*/
subroutine cclut::evaluateTestResults(cclutTestIndex, cclutPReplyStruct, cclutPAssertStruct, cclutPTimerStruct)
  declare cclutErrorCount = i4 with protect, noconstant(0)
  declare cclutErrorIndex = i4 with protect, noconstant(0)
  declare cclutReplyErrorCount = i4 with protect, noconstant(0)
  declare cclutStat = i4 with protect, noconstant(0)
  declare testSuccess = i2 with protect, noconstant(TRUE)

  record errorList(
    1 error[*]
      2 errorCode = i4
      2 errorMessage = vc
      2 lineNumber = i4
  ) with protect
  
  if (cclutTestIndex <= 0)
    return (FALSE)
  endif
  call cclut::getErrorList(errorList)
  call cclut::evaluateTimers(cclutPTimerStruct)
  set cclutErrorCount = cclut::getErrorList(errorList)
  set cclutReplyErrorCount = size(cclutPReplyStruct->tests[cclutTestIndex].errors, 5)  
  for (cclutErrorIndex = 1 to cclutErrorCount)
    set cclutReplyErrorCount = cclutReplyErrorCount + 1
    set cclutStat = alterList(cclutPReplyStruct->tests[cclutTestIndex].errors, cclutReplyErrorCount)
    set cclutPReplyStruct->tests[cclutTestIndex].errors[cclutReplyErrorCount].lineNumber = 
        errorList->error[cclutErrorIndex].lineNumber 
    set cclutPReplyStruct->tests[cclutTestIndex].errors[cclutReplyErrorCount].errorText = 
        errorList->error[cclutErrorIndex].errorMessage
    set testSuccess = FALSE;
  endfor
  
  set testSuccess = band(testSuccess, cclut::evaluateAsserts(cclutTestIndex, cclutPReplyStruct, cclutPAssertStruct))
  set cclutStat = initRec(cclutPTimerStruct)
  return (testSuccess)
end ;;;evaluateTestResults
  

/**
  Executes the unit tests in the containing test case program and evaluates the results storing them in the cclutReply structure
  provided to the test case program. If the value of testNamePattern is populated in the cclutRequest structure
  provided to the test case program, only test matching the provided regular expression are executed. If there is a setupOnce
  subroutine it will be executed before executing any of the tests. If there is a teardownOnce subroutine it will be executed
  at the end of the text execution. If there is a setup subroutine it will be executed before each test is executed. If there
  is a teardown function it will be executed after each test is executed.
  @returns
    A boolean flag indicating whether some test caused errors or had an assertion failure:
    TRUE if there were no problems; FALSE otherwise.
    
*/
subroutine cclut::executeTestLogic(null)
  declare CCLUT_FUN_TYPE_SUBROUTINE   = i4 with protect, constant(7)
  declare cclutRegistryTestIndex      = i4 with protect, noconstant(0)
  declare cclutRegistryTestCount      = i4 with protect, noconstant(0)
  declare cclutReplyTestCount         = i4 with protect, noconstant(0)
  declare cclutErrorCount             = i4 with protect, noconstant(0)
  declare ccutTestNamePattern         = vc with protect, noconstant("")
  declare cclut::testName             = vc with protect, noconstant("")
  declare cclutStat                   = i4 with protect, noconstant(0)
  
  record cclut_testRegistryRequest
  (
    1 programName = vc
  ) with protect
   
  record cclut_testRegistryReply (
    1 tests[*]
      2 testName = vc
%i cclsource:status_block.inc
  ) with protect
   
  set cclutStat = initRec(cclut_runResult)
  set cclutStat = initRec(cclut_timerList)
  set cclutReply->resultInd = TRUE

  set cclut_testRegistryRequest->programName = curprog
  call cclut::exitOnError("pre-executeTestLogic-1", cclut::testCaseFileName, cclutReply)
  execute cclut_find_unit_tests with replace("CCLUTREQUEST", cclut_testRegistryRequest), 
      replace("CCLUTREPLY", cclut_testRegistryReply)
  set ccutTestNamePattern = trim(cnvtupper(cclutRequest->testNamePattern), 3)
  set cclutRegistryTestCount = size(cclut_testRegistryReply->tests, 5)
   
  if (cclutRegistryTestCount > 0)
    call echo("Starting Test Case execution...") ;intentional
   
    if (checkfun("SETUPONCE") = CCLUT_FUN_TYPE_SUBROUTINE)
      set cclutReplyTestCount = 1
      set cclutStat = alterlist(cclutReply->tests, cclutReplyTestCount)
      set cclutReply->tests[cclutReplyTestCount].name = "SETUPONCE"
      call cclut::exitOnError("pre-executeTestLogic-2", cclut::testCaseFileName, cclutReply)
      call echo("Executing setupOnce...") ;intentional
      call setupOnce(null)
      set cclutReply->resultInd = band(cclutReply->resultInd, 
          cclut::evaluateTestResults(cclutReplyTestCount, cclutReply, cclut_runResult, cclut_timerList))
      if (cclutReply->resultInd = FALSE and cclutRequest->failFast = TRUE)
        set cclutRegistryTestCount = 0
      endif
    endif
  
    for (cclutRegistryTestIndex = 1 to cclutRegistryTestCount)
      set cclut::testName = cclut_testRegistryReply->tests[cclutRegistryTestIndex].testName
      if (textlen(trim(ccutTestNamePattern)) = 0 
            or operator(cnvtlower(cclut::testName), "regexplike", cnvtlower(ccutTestNamePattern)))
        set cclutReplyTestCount = cclutReplyTestCount + 1
        set cclutStat = alterlist(cclutReply->tests, cclutReplyTestCount)
        set cclutReply->tests[cclutReplyTestCount].name = cclut::testName

        call echo(build2("running test ", cclut::testName)) ;intentional
        if (checkfun("SETUP") = CCLUT_FUN_TYPE_SUBROUTINE)
          if (cclutReplyTestCount = 1) ;there is no setupOnce
            call cclut::exitOnError("pre-executeTestLogic-3", cclut::testCaseFileName, cclutReply)
          endif
          call echo("Executing setup...") ;intentional
          call setup(null)
        endif
        set cclutReply->resultInd = band(cclutReply->resultInd, 
            cclut::evaluateTestResults(cclutReplyTestCount, cclutReply, cclut_runResult, cclut_timerList))

        if (cclutReplyTestCount = 1 and checkfun("SETUP") != CCLUT_FUN_TYPE_SUBROUTINE) ;there is no setupOnce or setup
          call cclut::exitOnError("pre-executeTestLogic-4", cclut::testCaseFileName, cclutReply)
        endif
        call echo(build2("Executing ", cclut::testName, "...")) ;intentional
        call parser(concat("call ", cclut::testName, "(null) go"))
        set cclutReply->resultInd = band(cclutReply->resultInd, 
            cclut::evaluateTestResults(cclutReplyTestCount, cclutReply, cclut_runResult, cclut_timerList))
        if (checkfun("TEARDOWN") = CCLUT_FUN_TYPE_SUBROUTINE)
          call echo("Executing teardown...") ;intentional
          call teardown(null)
        endif
        set cclutReply->resultInd = band(cclutReply->resultInd, 
            cclut::evaluateTestResults(cclutReplyTestCount, cclutReply, cclut_runResult, cclut_timerList) )
        if (cclutReply->resultInd = FALSE and cclutRequest->failFast = TRUE)
          set cclutRegistryTestIndex = cclutRegistryTestCount
        endif
      endif
    endfor
      
    if (checkfun("TEARDOWNONCE") = CCLUT_FUN_TYPE_SUBROUTINE)
      call echo("Executing teardownOnce...") ;intentional
      set cclutReplyTestCount = cclutReplyTestCount + 1
      set cclutStat = alterlist(cclutReply->tests, cclutReplyTestCount)
      set cclutReply->tests[cclutReplyTestCount].name = "TEARDOWNONCE"
      call teardownOnce(null)
      set cclutReply->resultInd = band(cclutReply->resultInd, 
          cclut::evaluateTestResults(cclutReplyTestCount, cclutReply, cclut_runResult, cclut_timerList))
    endif  
  endif
end ;;;executeTestLogic

call cclut::exitOnError("pre-executeTestLogic-0", cclut::testCaseFileName, cclutReply)
set cclutReply->status_data.status = "S"
call cclut::executeTestLogic(null)
 
#exit_script
;Clean up any mocks that were not cleaned up by the tests.
call cclutRemoveAllMocks(null)
;Record any dangling errors and asserts in case the last executed unit test called go to exit_script.
set cclutReply->resultInd = band(cclutReply->resultInd, 
    cclut::evaluateTestResults(size(cclutReply->tests, 5), cclutReply, cclut_runResult, cclut_timerList))
  
%#endif CCLUT_EXECUTE_TEST_LOGIC_INC
 