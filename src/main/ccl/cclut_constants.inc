%#ifndef CCLUT_CONSTANTS_INC
%#define CCLUT_CONSTANTS_INC
;**********************************************************************************************************************************
;
; Defines resuable constant values for the CCLUT framework.
;
;**********************************************************************************************************************************

declare CCLUT_CC_TC_START_MARKER  = vc with protect, constant(";;;CCLUT_START_TEST_INC_FILE")
declare CCLUT_CC_TC_END_MARKER    = vc with protect, constant(";;;CCLUT_END_TEST_INC_FILE")

declare CCLUT_UNCOVERED         = c1 with protect, constant("U")
declare CCLUT_NOT_EXECUTABLE    = c1 with protect, constant("N")
declare CCLUT_COVERED           = c1 with protect, constant("C")
declare CCLUT_UNKNOWN           = c1 with protect, constant("Z")

declare CCLUT_CC_UNCONVERED     = c1 with protect, constant("U")
declare CCLUT_CC_NOT_EXECUTABLE = c1 with protect, constant("N")
declare CCLUT_CC_COVERED        = c1 with protect, constant("C")    

%#endif CCLUT_CONSTANTS_INC
