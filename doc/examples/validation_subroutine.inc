;;; here is the script

drop program the_script go
create program the_script
  execute other_script 4, 5
  execute other_script 3, 2
  execute other_script 0, 1
end go




/**
Mock implementation for other_script which calls validateOtherScriptParams to validate the two input parameters
and increments a counter for the number of calls to other_script.
*/
drop program mock_other_script go
create program mock_other_script
  prompt "param 1", "param 2" with param1, param2

  set otherScriptCallCount = otherScriptCallCount + 1
  call validateOtherScriptParams($param1, $param2)
end go




;;; put this functions in a test case (.inc)

/**
* confirms that the script executes other_script exactly three times passing in (4, 5) then (3, 2) then (0, 1)
*/
subroutine (testOtherScriptCalledThreeTimes(null) = null)
  declare otherScriptCallCount = i4 with protect, noconstant(0)

  call cclutAddMockImplementation("OTHER_SCRIPT", "mock_other_script")
  call cclutExecuteProgramWithMocks("the_script", "")

  set stat = cclutAssertI4Equal(CURREF, "testMyFunction_6_9 a", otherScriptCallCount, 3)
end ;;;testMyFunctionZero

subroutine (validateOtherScriptParams(p1 = i4, p2 = i4) = null)
  case (otherScriptCallCount)
  of 1:
    set stat = cclutAssertI4Equal(CURREF, "validateOtherScriptParams 1 a", p1, 4)
    set stat = cclutAssertI4Equal(CURREF, "validateOtherScriptParams 1 b", p2, 5)
  of 2:
    set stat = cclutAssertI4Equal(CURREF, "validateOtherScriptParams 2 a", p1, 3)
    set stat = cclutAssertI4Equal(CURREF, "validateOtherScriptParams 2 b", p2, 2)
  of 3:
    set stat = cclutAssertI4Equal(CURREF, "validateOtherScriptParams 3 a", p1, 0)
    set stat = cclutAssertI4Equal(CURREF, "validateOtherScriptParams 3 b", p2, 1)
  endcase
end ;;;validateOtherScriptParams
