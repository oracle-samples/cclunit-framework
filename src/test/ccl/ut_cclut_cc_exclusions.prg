drop program ut_cclut_cc_exclusions:dba go
create program ut_cclut_cc_exclusions:dba
/**
  A CCL program which contains code coverage exclusions used for unit testing the code coverage exclusion functions.
*/
  prompt "Output Location: " = "MINE" with outdev
%i cclsource:ut_cclut_cc_exclusions.inc
  declare sub1(null) = null with protect
  declare sub2(null) = null with protect
  declare sub3(null) = null with protect
  subroutine sub1(null)
    declare idx = i4 with protect, noconstant(0)
    for (idx = 1 to 2)
      call echo("executing sub1") ;intentional
;;;CCLUNIT:OFF
      call echo("not sub2") ;intentional
      call echo("not sub2") ;intentional
;;;CCLUNIT:ON
    endfor
  end ;;;sub1
  subroutine sub2(null)
    declare idx = i4 with protect, noconstant(0)
    for (idx = 1 to 2)
      call echo("executing sub2") ;intentional
      call echo("executing sub2") ;intentional
;;;CCLUNIT:OFF
      call echo("not sub1") ;intentional
;;;CCLUNIT:ON
    endfor
  end ;;;sub2
  subroutine sub3(null)
    select into 'nl:' from code_value cv where cv.code_set = 8 order by cv.display
;;;CCLUNIT:OFF
    head cv.display
;this will be ignored because it comes before the corresponding ON
;;;CCLUNIT:OFF
      call echo(cv.display) ;intentional
;;;CCLUNIT:ON
    detail
      call echo(build2(cv.display_key, ": ", cv.display)) ;intentional
  end ;;;sub3
  
  call sub4(null)

#exit_script
  if (validate(_memory_reply_string))
    set _memory_reply_string = "done"
  endif
  
;this will be ignored because there is not a corresponding ON.
;;;CCLUNIT:OFF
call echo("done") ;intentional
  
end go
