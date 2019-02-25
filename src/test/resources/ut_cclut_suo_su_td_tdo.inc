/**
  A test case file for testing the CCL Unit framework. 
  This test case file contains all of the testing support subroutines, setupOnce, setup, teardown and teardownOnce and three test 
  subroutines. Each subroutine fires a successful assert to indicate it executed and based on configuration will then fire 
  a failed assert and/or cause an error based on the value of the boolean variable 
  testing::{fail|error}{SetupOnce|Setup|Test|Teardown|TeardownOnce} which needs to be provided by the invoking test.
*/
declare setupOnce(null) = null
declare teardownOnce(null) = null
declare setup(null) = null
declare testOne(null) = null
declare testTwo(null) = null
declare testThree(null) = null
declare teardown(null) = null

declare guaranteedError = i4 with protect, noconstant(0)

if (validate(testing::errorLoading, FALSE) = TRUE)
    set guaranteedError = "loading error"
endif 

subroutine setupOnce(null)
  call cclutAssertVCEqual(CURREF, "setupOnce executed", "SO", "SO")
  if (testing::failSetupOnce)
    call cclutAssertI4Equal(CURREF, "setupOnce failure", 0, 1)
  endif
  if (testing::errorSetupOnce)
    call cclutAssertI4Equal(CURREF, "setupOnce error", 1, 1)
    set guaranteedError = "setupOnce error"
  endif
end ;setupOnce

subroutine teardownOnce(null)
  call cclutAssertVCEqual(CURREF, "teardownOnce executed", "TO", "TO")
  if (testing::failTeardownOnce)
    call cclutAssertI4Equal(CURREF, "teardownOnce failure", 0, 1)
  endif
  if (testing::errorTeardownOnce)
    call cclutAssertI4Equal(CURREF, "teardownOnce error", 1, 1)
    set guaranteedError = "teardownOnce error"
  endif
end ;teardownOnce

subroutine setup(null)
  call cclutAssertI4Equal(CURREF, "setup executed", 0, 0)
  if (testing::failSetup)
    call cclutAssertI4Equal(CURREF, "setup failure", 0, 1)
  endif
  if (testing::errorSetup)
    call cclutAssertI4Equal(CURREF, "setup error", 1, 1)
    set guaranteedError = "setup error"
  endif
end ;;;setup

subroutine teardown (null)
  call cclutAssertI4Equal(CURREF, "teardown executed", 0, 0)
  if (testing::failTeardown)
    call cclutAssertI4Equal(CURREF, "teardown failure", 0, 1)
  endif
  if (testing::errorTeardown)
    call cclutAssertI4Equal(CURREF, "teardown error", 1, 1)
    set guaranteedError = "teardown error"
  endif
end ;;;setup

subroutine testOne(null)
  call cclutAssertI4Equal(CURREF, "testOne executed", 0, 0)
  if (testing::failTest)
    call cclutAssertI4Equal(CURREF, "testOne failure", 0, 1)
  endif
  if (testing::errorTest)
    call cclutAssertI4Equal(CURREF, "testOne error", 1, 1)
    set guaranteedError = "testOne error"
  endif
end ;;;testOne

subroutine testTwo(null)
  call cclutAssertI4Equal(CURREF, "testTwo executed", 0, 0)
  call cclutAssertVCEqual(CURREF, "testTwo executed", "A", "A")
end ;;;testTwo

;; like testOne, but two failures and two errors and errors come before failures when both are specified.
subroutine testThree(null)
  call cclutAssertI4Equal(CURREF, "testThree executed", 0, 0)
  if (testing::errorTest)
    set guaranteedError = "testThree error"
    set guaranteedError = "testThree error 2"
  endif
  if (testing::failTest)
    call cclutAssertI4Equal(CURREF, "testThree failure A", 0, 1)
    call cclutAssertI4Equal(CURREF, "testThree failure B", 0, 1)
  endif
end ;;;testThree
