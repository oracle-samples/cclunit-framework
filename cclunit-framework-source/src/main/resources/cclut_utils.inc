%#ifndef CCLUT_UTILS_INC
%#def CCLUT_UTILS_INC
;**********************************************************************************************************************************
;
; Provides utility functions that can be leveraged by the rest of the CCL Unit Testing framework in order to perform common
; operations.
;
;**********************************************************************************************************************************

declare cclut::cclutIsEmpty(cclutText = vc) = i2 with protect

/**
Checks a vc value to determine if it is an empty string or only contains whitespace/control characters.  Returns TRUE if
the string meets the criteria or FALSE if it does not.

@param cclutText
    A vc value to be checked.
@returns
    TRUE if the string is empty or only contains whitespace/control characters; otherwise, FALSE.

Example:
call cclut::cclutIsEmpty("test") ; returns FALSE
call cclut::cclutIsEmpty("") ; returns TRUE
call cclut::cclutIsEmpty("     ") ; returns TRUE
**/
subroutine cclut::cclutIsEmpty(cclutText)
    return (evaluate(textlen(trim(cclutText, 3)), 0, TRUE, FALSE))
end ;cclutIsEmpty

%#endif CCLUT_UTILS_INC
