drop program cclutExecuteSuccessTestLogic go
create program cclutExecuteSuccessTestLogic

%i cclsource:cclutassert_impl_nff.inc
%i cclsource:cclut_error_handling.inc
%i cclsource:cclut_reflection_subs.inc

/*
record cclutReply
(
  1 tests[*]
    2 name = vc
    2 asserts[*]
      3 lineNumber = i4
      3 context = vc
      3 resultInd = i2 ;1 = PASS, 0 = FAIL
      3 test = vc
    2 errors[*]
      3 lineNumber = i4
      3 errorText = vc
%i cclsource:status_block.inc
)
*/

declare cclut::testCaseFileName = vc with protect, constant("no test case file")

declare test1(dummyVar = i2) = null
declare test2(dummyVar = i2) = null

subroutine test1(dummyVar)
    set stat = cclutAsserti4Equal(CURREF, 'TEST 1', 1, 1)
end

subroutine test2(dummyVar)
    set stat = cclutAsserti4Equal(CURREF, 'TEST 2', 1, 1)
end
%i cclsource:cclut_timers.inc
%i cclsource:cclut_execute_test_logic.inc
end
go