%#ifndef CCLUT_UTILS_INC
%#def CCLUT_UTILS_INC
;**********************************************************************************************************************************
;
; Provides utility functions that can be leveraged by the rest of the CCL Unit Testing framework in order to perform common
; operations.
;
;**********************************************************************************************************************************

declare public::cclutIsEmpty(cclutText = vc) = i2 with protect
declare public::cclutInvokeParser(cclutParserString = vc) = null with protect

/**
Checks a vc value to determine if it is an empty string or only contains whitespace/control characters.  Returns TRUE if
the string meets the criteria or FALSE if it does not.

@param cclutText
    A vc value to be checked.
@returns
    TRUE if the string is empty or only contains whitespace/control characters; otherwise, FALSE.

Example:
call cclutIsEmpty("test") ; returns FALSE
call cclutIsEmpty("") ; returns TRUE
call cclutIsEmpty("     ") ; returns TRUE
*/
subroutine public::cclutIsEmpty(cclutText)
    return (evaluate(textlen(trim(cclutText, 3)), 0, TRUE, FALSE))
end ;cclutIsEmpty

/**
Invokes call parser on a specified string. Allows for overriding calls to call parser.

@param cclutParserString
    The string to invoke call parser on.        
*/
subroutine public::cclutInvokeParser(cclutParserString)
    call parser(cclutParserString)
end ;cclutInvokeParser
%#endif CCLUT_UTILS_INC
