%#ifndef CCLUT_REFLECTION_SUBS_INC
%#def CCLUT_REFLECTION_SUBS_INC
;**********************************************************************************************************************************
;
; Provides subroutines for obtaining information about the currently executing unit test from within a unit test.
;
;**********************************************************************************************************************************
 
declare cclut::getTestName(null) = vc

/**
  Returns the name of the currently executing test. This function should always return a value even if invoked from 
  setup, teardown and other helper routines since because those routines are called in the context of a unit test.
  @returns
    The name of the currently executing test in all caps.
*/  
subroutine cclut::getTestName(null)
  return (cclut::testName)
end ;;;getTestName
 
%#endif CCLUT_REFLECTION_SUBS_INC