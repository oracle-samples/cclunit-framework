/**
  A program for testing that cclut_find_unit_tests can find unit tests declared using all the different types of subroutine 
  declaration syntax.
  
  If you are trying to install the framework into a domain that has an older version of CCL which does not support some of these
  constructs, the build will fail. Remeove this test case file from the build or delete the specific tests which are causing 
  problems.
*/
drop program ut_cclut_find_unit_tests:dba go
create program ut_cclut_find_unit_tests:dba
  declare testThree(null) = null
  declare testing::testFour(null) = null
  declare testing::thisIsNotATest(null) = null
  declare retesting::testingOne(null) = null
      
  subroutine testOne(null)
    call echo("testOne")
  end
  subroutine testing::testTwo(null)
    call echo("testTwo")
  end
  subroutine testThree(null)
    call echo("testThree")
  end
  subroutine testing::testFour(null)
    call echo("testFour")
  end
  subroutine (testFive(null) = null)
    call echo("testFive")
  end
  subroutine (testing::testSix(null) = null)
    call echo("testSix")
  end
  subroutine testing::thisIsNotATest(null)
    call echo("this is not a test")
  end
  subroutine (testing::thisIsNotATestEither(null) = null)
    call echo("this is not a test either")
  end
  subroutine ns::testSeven(null)
    call echo("ns::testSeven")
  end
  subroutine (ns::testEight(null) = null)
    call echo("ns::testEight")
  end
end go
