/**
  An include file which contains code coverage exclusions used for unit testing the code coverage exclusion functions.
*/
declare sub4(null) = null
subroutine sub4(null)
  call sub1(null)
;;;CCLUNIT:OFF
  call sub2(null)
;;;CCLUNIT:ON
  call sub3(null)
end ;;;sub4