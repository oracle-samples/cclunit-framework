drop program cclutExecuteFailTestLogic go
create program cclutExecuteFailTestLogic

%i cclsource:cclut_timers.inc
%i cclsource:cclut_error_handling.inc
%i cclsource:cclut_reflection_subs.inc
%i cclsource:cclutassert_impl_nff.inc

/*
record cclutReply
(
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
*/

declare cclut::testCaseFileName = vc with protect, constant("the test case file name")

declare testFail(dummyVar = i2) = null
declare testSuccess(dummyVar = i2) = null

subroutine testFail(dummyVar)
    set stat = cclutAsserti4Equal(CURREF, 'TEST FAIL', 1, 0)
end

subroutine testSuccess(dummyVar)
    set stat = cclutAsserti4Equal(CURREF, 'TEST SUCCESS', 1, 1)
end

%i cclsource:cclut_execute_test_logic.inc
end go