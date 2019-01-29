# CCL Unit Guidance

## Building Unit Tests

Before there can be unit testing, there must be units. The ultimate goal is to have small, testable, modular units and to test each of them individually. The goal of this guidance is to describe a way to structure a CCL script that supports testing its units individually. It will not discuss how to create units or unit testable code as there are many other sources that go into that detail.
 
Say our CCL script contains a testable unit "functionX" that we want to test in some particular scenario, "scenarioY".  Then we will create and execute a unit test named something like "testFunctionXForScenarioY".  We want our test to execute functionX and the descendants of functionX. We do not want it to execute any other functions in our script. Here is how to go about it.
 
First off, namespace every subroutine with the PUBLIC namespace. Then put all  (most all) of the executable code for the script into a main function, say public::main(null) = null
Use a unique namespace to define an override for the main subroutine within the test suite, say testFunctionXForScenarioY::main(null) = null, and have the override first set up the conditions for scenarioY and then have it call functionX directly.
 
The key point here is that the script does not contain a lot of loose code that gets executed every time the script executes. It all gets bundled into the main subroutine which is what allows a unit test to dictate exactly how much of the script's code will be executed.

Here is an illustration of the concept:
```
;; this is the test
;;; testFunctionB::main is the workhorse. It will be called instead of public::main because of the curnamespace
 
/**
* Confirms that functionB returns "functionBTest" when it is passed "Test" 
* and "functionBSubroutine" when it is passed "Subroutine".
*/
subroutine (testFunctionB(null) = null)
 
  execute the_script:dba with curnamespace = "testFunctionB"
 
end
 
subroutine (testFunctionB::main(null) = null)
  declare b_string = vc with protect, noconstant("")
 
  set b_string = functionB("Test")
  set stat = cclutAssertVCEquals(CURREF, "testFunctionB Test", b_string, "functionBTest")
 
  set b_string = functionB("Subroutine")
  set stat = cclutAssertVCEquals(CURREF, "testFunctionB Subroutine", b_string, "functionBSubroutine")
end;;;testFunctionB

;; this is the script to be tested
 
drop program the_script:dba go
create program the_script:dba
 
  record reply (
%i cclsource:status_block.inc
  )
 
  subroutine (PUBLIC::functionA(id = f8) = vc)
  	call echo("functionA")
    ;;; do stuff
    return("functionA")
  end
  
  subroutine (PUBLIC::functionB(name = vc) = vc)
    call echo("functionB")
    ;;; do stuff
    return(concat("functionB", name))
  end
  
  subroutine (PUBLIC::functionC(null) = null)
  	call echo("functionC")
    ;;; do stuff
    call functionD(null)
  end
  
  subroutine (PUBLIC::functionD(null) = null)
    call echo("functionD")
  end
 
  ;;; observe that subroutine main deserves to be tested in its own right with all other functions mocked.
  subroutine(PUBLIC::main(null) = null)
    declare a_string = vc with protect, noconstant("")
    declare b_string = vc with protect, noconstant("")
    set a_string = functionA(1.0)
    set b_string = functionB(a_string)
    call functionC(null)
  end
  
  call main(null)
  
#exit_script
  ;; script finalizer code can go here
  ;; a PUBLIC::exitScript(null) subroutine that encapsulates this work could be created and called 
  ;; if it does something that should occur for some unit tests.
end go 

```

Be careful!  There is a small problem if you structure all of your scripts using this pattern and you test a function in one script that calls another script that isn't mocked by your test.   Try the following example to understand what can go wrong:

```
drop program callstackA go
create program callstackA
subroutine (public::main(null) = null with private)
  call echo("callstackA's main")
end  
  call main(null)
  execute callstackB
end go
 
drop program callstackB go
create program callstackB 
subroutine (public::main(null) = null with private)
  call echo("callstackB's main")
end  
  call main(null)
  execute callstackC
end go
 
drop program callstackC go
create program callstackC
subroutine (public::main(null) = null with private)
  call echo("callstackC's main")
end  
  call main(null)
end go
 
drop program callstack go
create program callstack
  subroutine (callstack::main(null) = null with protect)
    call echo("callstack's main")
  end
 
  execute callstackA with curnamespace = 'callstack'
end go 
```

If you execute callstackA directly, each program calls the correct main function.  That's because each program gravitates to the copy of main that is in the closest scope, i.e., the one it declared for itself.  

If you execute call stack, each program calls the override.  That is because when the namespace is forced, there is only one copy of callstack::main to choose from and since the forced namespace propagates to descendants, all the programs are forced to chose it.

There are many ways to circumvent it.  One is to always mock any descendant programs.  Another is to use unique function names rather than generic ones. The script script_X could name its main function public::script_x_main, for example. Since script names are limited to 32 characters and subroutine names can be resolved to at least 40 characters, this will work, but another option still is to use "with replace".  An example can be seen below:

```
drop program callstackA go
create program callstackA
subroutine (public::main(null) = null with private)
  call echo("callstackA's main")
end  
  call main(null)
  execute callstackB
end go
 
drop program callstackB go
create program callstackB 
subroutine (public::main(null) = null with private)
  call echo("callstackB's main")
end  
  call main(null)
  execute callstackC
end go
 
drop program callstackC go
create program callstackC
subroutine (public::main(null) = null with private)
  call echo("callstackC's main")
end  
  call main(null)
end go
 
drop program callstack go
create program callstack
  subroutine (public::callstackmain(null) = null with protect)
    call echo("callstack's main")
  end
 
  execute callstackA with replace("MAIN", callstackmain)
end go 
```

This works because "with replace" does not propagate to descendant scripts by default (though an option is available to do so).  Additionally, the CCL Unit Testing framework contains a mocking API that abstracts the "with replace" logic.  See the documentation for cclutAddMockImplementation at [CCLUTMOCKING.md](../CCLUTMOCKING.md)

## CCL Mocks

In general, there are two ways to mock objects in CCL unit tests:

1. Use "with replace"
2. Use "with curnamespace"

    2a. Add the PUBLIC namespace to the real thing.  Use an alternate namespace to define an override.  Execute the script using 'with curnamespace = "\<alternate namespace\>"'
    
    2b. In practice, it is convenient to use the name of the test case for the alternate namespace.
    
Generally speaking, use "with replace" to mock things that are defined outside the script (CCL subroutines, UARs, other scripts), and use "with curnamespace" to mock things defined within the script.

The CCL Unit Testing framework also supports an abstraction for creating mocks.  The purpose is to help make it easier to define mock tables and other mock objects to be used when executing a script.  Details on the API can be found at [CCLUTMOCKING.md](../CCLUTMOCKING.md)

Below are some examples using the CCL Unit Testing framework's mocking API.  These examples assume the script under test is called "the_script" and executes a program called "other_script".  They test for scenarios where other_script returns 0 items, more than 5 items, and a failed ("F") status.

```
;;; put the following script definition in a .prg file in src/test/ccl
 
 
drop program mock_other_script go
create program mock_other_script
  free record reply
  set stat = copyrec(mock_other_script_reply, reply, 1)
end go

;;; put the following functions in a test suite (.inc) in src/test/ccl
 
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
```

There are other variations on this.  For example, you could put asserts within mock_other_script itself.  Additionally, other_script might generate its own reply structure, so you would want to do the same in mock_other_script.

Below is an example where the mock script calls a validation subroutine defined from the testing subroutine.  It tests that other_script is called exactly 3 times with the correct parameters each time.

```
;;; here is the script
 
drop program the_script go
create program the_script
  execute other_script 4, 5
  execute other_script 3, 2
  execute other_script 0, 1
end go
 
 
;;; put this definition in a .prg file in src/test/ccl
 
drop program mock_other_script go
create program mock_other_script
  prompt "param 1", "param 2" with param1, param2
  
  set otherScriptCallCount = otherScriptCallCount + 1
  call validateOtherScriptParams($param1, $param2)
end go

;;; put this functions in a test suite (.inc) in src/test/ccl
 
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
```

Finally, a namespace example where getPersonName will always return the same value.

```
/**
* Executes a test knowing that every call to getPersonName(id) will return "Bob Marley".
*/
subroutine (testGetNameReturnsBobMarley(null) = null)
  declare otherScriptCallCount = i4 with protect, noconstant(0)
  
  call cclutExecuteProgramWithMocks("the_script", "", "testGetNameReturnsBobMarley")
  
  ; assert stuff here
end
subroutine (testGetNameReturnsBobMarley::getPersonName(id = f8) = vc)
    return ("Bob Marley")
end ;;;testGetNameReturnsBobMarley 
```

## Commit/Rollback Guidance

"commit" and "rollback" are keywords within CCL that apply the respective commands to the RDBMS.  This can be particularly annoying when dealing with real tables, especially if one is testing insert/update/delete functionality.  The table mocking API helps mitigate this by using separate custom tables, but it may still be advantageous to separate any usages of commit/rollback into their own subroutines and test them independently of the other tests.

