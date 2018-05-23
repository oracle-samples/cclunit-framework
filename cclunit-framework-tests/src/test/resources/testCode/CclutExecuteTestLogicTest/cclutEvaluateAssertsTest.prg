drop program cclutEvaluateAssertsTest go
create program cclutEvaluateAssertsTest

%i cclsource:cclut_timers.inc
%i cclsource:cclut_error_handling.inc
%i cclsource:cclut_reflection_subs.inc


/*
record dt_assert(
 1 line[*]
     2 nbr = i4
     2 ctx = vc
     2 result = vc
     2 datetime = vc
     2 condition = vc
     2 before_ecode = i4
     2 before_emsg = vc
     2 ecode = i4
     2 emsg = vc
)

record dt_request(
 1 testIndex = i4
)

record cclutReply(
  1 resultInd = i2
  1 tests[*]
    2 name = vc
    2 asserts[*]
      3 lineNumber = i4
      3 context = vc
      3 resultInd = i2 ;1 = PASS, 0 = FAIL
      3 condition = vc
    2 errors[*]
      3 lineNumber = i4
      3 errorText = vc
 %i cclsource:status_block.inc
)

record dt_reply(
 1 result = i2
)
*/



 
set stat = alterlist(cclutReply->tests, 1)
set cclutReply->resultInd = 1 ;must be born=1 because it will be set equal to band of itself with the current result ind.
set cclutReply->tests[1].name = "first_test"

declare cclut::testCaseFileName = vc with protect, constant("the test case file name")
declare doNothing(null) = null
subroutine cclut::doNothing(null)
  declare stat = i4 with protect, noconstant(0)
end 

;if cclut_runResult (i.e., dt_assert) contains data 
;then cclut::evaluateAsserts will be called at the exit_script in cclut_execute_test_logic.inc
%i cclsource:cclut_execute_test_logic.inc

set dt_reply->result = cclutReply->resultInd ;cclut::evaluateAsserts(1, cclutReply, dt_assert)


#EXIT_TEST

end go