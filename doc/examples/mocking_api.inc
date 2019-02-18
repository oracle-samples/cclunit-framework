;;; put the following script definition in a .prg file and compile it

/**
Mock implementation for other_script which sets reply equal to a copy of mock_other_script_reply.
*/
drop program mock_other_script go
create program mock_other_script
  free record reply
  set stat = copyrec(mock_other_script_reply, reply, 1)
end go




;;; put the following functions in a test case (.inc)

/**
* Test myFunction when other_script returns zero items
*/
subroutine (testMyFunctionOtherScriptZero(null) = null)
  record mock_other_script_reply (
    1 qual [*]
      2 id = f8
%i cclsource:status_block.inc
  ) with protect

  set mock_other_script_reply->status_data->status = "Z"

  call cclutAddMockImplementation("OTHER_SCRIPT", "mock_other_script")
  call cclutExecuteProgramWithMocks("the_script", "")

  ;assert things here
end ;;;testMyFunctionZero

/**
* Test myFunction when other_script returns more than five items
*/
subroutine (testMyFunctionOtherScriptMoreThanTen(null) = null)
  record mock_other_script_reply (
    1 qual [*]
      2 id = f8
%i cclsource:status_block.inc
  ) with protect

  set mock_other_script_reply->status_data->status = "S"
  set stat = alterlist(mock_other_script_reply->qual, 6)

  declare idx = i4 with protect, noconstant(0)
  for (idx = 1 to 6)
    set mock_other_script_reply->qual[idx].id = idx
  endfor

  call cclutAddMockImplementation("OTHER_SCRIPT", "mock_other_script")
  call cclutExecuteProgramWithMocks("the_script", "")

  ;assert things here
end ;;;testMyFunctionMoreThanTen

/**
* Test myFunction when other_script fails
*/
subroutine (testMyFunctionOtherScriptFail(null) = null)
  record mock_other_script_reply (
    1 qual [*]
      2 id = f8
%i cclsource:status_block.inc
  ) with protect

  set mock_other_script_reply->status_data->status = "F"

  call cclutAddMockImplementation("OTHER_SCRIPT", "mock_other_script")
  call cclutExecuteProgramWithMocks("the_script", "")

  ;assert things here
end ;;;testMyFunctionOtherScriptFail




;; this is the script to be tested

drop program the_script:dba go
create program the_script:dba

  execute other_script

end go
