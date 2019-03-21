/**
  A test case file for testing the CCL Unit framework when there are no tests in a test case file.
*/
declare notATest(null) = null
subroutine notATest(null)
  call echo("I am not a test")  ;intentional
end ;;;notATest
