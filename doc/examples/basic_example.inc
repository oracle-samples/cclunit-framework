;; this is the test

/**
* Confirms that subroutineB returns "subroutineBTest" when it is passed "Test"
* and "subroutineBSubroutine" when it is passed "Subroutine".  public::mainForSubroutineB is the workhorse. It will be
* called instead of public::main because of "with replace"
*/
subroutine (testSubroutineB(null) = null)

  execute the_script:dba with replace("MAIN", MAINFORSUBROUTINEB)

end

subroutine (public::mainForSubroutineB(null) = null)
  declare b_string = vc with protect, noconstant("")

  set b_string = subroutineB("Test")
  set stat = cclutAssertVCEqual(CURREF, "testSubroutineB Test", b_string, "subroutineBTest")

  set b_string = subroutineB("Subroutine")
  set stat = cclutAssertVCEqual(CURREF, "testSubroutineB Subroutine", b_string, "subroutineBSubroutine")
end;;;mainForSubroutineB




;; this is the script to be tested

drop program the_script:dba go
create program the_script:dba

  record reply (
%i cclsource:status_block.inc
  )

  subroutine (PUBLIC::subroutineA(id = f8) = vc)
  	call echo("subroutineA")
    ;;; do stuff
    return("subroutineA")
  end

  subroutine (PUBLIC::subroutineB(name = vc) = vc)
    call echo("subroutineB")
    ;;; do stuff
    return(concat("subroutineB", name))
  end

  subroutine (PUBLIC::subroutineC(null) = null)
  	call echo("subroutineC")
    ;;; do stuff
    call subroutineD(null)
  end

  subroutine (PUBLIC::subroutineD(null) = null)
    call echo("subroutineD")
  end

  subroutine(PUBLIC::main(null) = null)
    declare a_string = vc with protect, noconstant("")
    declare b_string = vc with protect, noconstant("")
    set a_string = subroutineA(1.0)
    set b_string = subroutineB(a_string)
    call subroutineC(null)
  end

  call main(null)

#exit_script
  ;; script finalizer code can go here
  ;; a PUBLIC::exitScript(null) subroutine that encapsulates this work could be created and called
  ;; if it does something that should occur for some unit tests.
end go
