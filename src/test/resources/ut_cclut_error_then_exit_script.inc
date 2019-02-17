/**
  A test case file for use by unit tests for the CCL Unit framework to check what happens when a unit test calls go to exit_scirpt
*/
declare setupOnce(null) = null
declare teardownOnce(null) = null
declare setup(null) = null
declare teardown(null) = null
declare testOne(null) = null
declare testNot(null) = null

subroutine setupOnce(null)
  call cclutAssertI4Equal(CURREF, "setupOnce - guaranteed success", 1, 1) 
end ;;; setupOnce

subroutine teardownOnce(null)
  call cclutAssertI4Equal(CURREF, "teardownOnce - guaranteed success", 1, 1) ;won't be seen
end ;;; setupOnce

;this has to be declared as a no-op to override the fact that the test case that executes this defines a setup
subroutine setup(null)
  declare stat = i4
end ;;;setup

;this has to be declared as a no-op to override the fact that the test case that executes this defines a teardown
subroutine teardown(null)
  declare stat = i4
end ;;;setup

subroutine testOne(null)
  call cclexception(100, "E", "forced error")
  go to exit_script
  call cclutAssertI4Equal(CURREF, "testOne - guaranteed success", 1, 1) ;won't be seen
end ;;; setupOnce

subroutine testNot(null)
  call cclutAssertI4Equal(CURREF, "testNot- guaranteed success", 1, 1) ;shouldn't be seen
end ;;; setupOnce
