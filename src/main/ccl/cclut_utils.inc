%#ifndef CCLUT_UTILS_INC
%#def CCLUT_UTILS_INC
;**********************************************************************************************************************************
;
; Provides utility functions that can be leveraged by the rest of the CCL Unit Testing framework to perform common operations or
; to mock various built-in CCL functions and reserved variables.
;
;**********************************************************************************************************************************

declare public::cclutIsEmpty(cclutText = vc) = i2 with protect
declare public::cclutGetCurrentRevision(null) = i4 with protect
declare public::cclutGetCurrentVersion(null) = i4 with protect
declare public::cclutGetCurrentGroup(null) = i4 with protect
declare public::cclutDoArraySplit(cclutRec = vc(ref), cclutSource = vc, cclutDelimiter = vc) = i4 with protect

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
Returns the current revision of CCL. Allows CURREV to be mocked.
**/
subroutine public::cclutGetCurrentRevision(null)
    return(currev)
end ;cclutGetCurrentRevision


/**
Returns the current version of CCL. Allows CURCCLVER to be mocked.
*/
subroutine public::cclutGetCurrentVersion(null)
    return(curcclver)
end ;cclutGetCurrentVersion


/**
Returns the group for the current CCL user. Allows CURGROUP to be mocked.
*/
subroutine public::cclutGetCurrentGroup(null)
    return(curgroup)
end ;cclutGetCurrentGroup


/**
    Parses a delimited string placing the (right trimmed) parts into the qual[*].str member of a provided record structure.
    If the minimum required CCL version is bumped above 8.13.2 / 9.1.2, arraySplit can be used and this can be removed.
*/
subroutine public::cclutDoArraySplit(cclutRec, cclutSource, cclutDelimiter)
    declare cclutStat = i4 with protect, noconstant(0)
    declare cclutDelimPos = i4 with protect, noconstant(0)
    declare cclutDelimEnd = i4 with protect, noconstant(0)
    declare cclutItemCount = i4 with protect, noconstant(0)

    declare cclutDelimLen = i4 with protect, constant(textlen(cclutDelimiter))

    set cclutDelimPos = findString(cclutDelimiter, cclutSource)
    while(cclutDelimPos > 0)
        set cclutDelimEnd = cclutDelimPos + cclutDelimLen
        set cclutItemCount = cclutItemCount + 1
        set cclutStat = alterlist(cclutRec->qual, cclutItemCount)
        set cclutRec->qual[cclutItemCount].str = substring(1, cclutDelimPos-1, cclutSource)
        set cclutSource = substring(cclutDelimEnd, 1 + textlen(cclutSource) - cclutDelimEnd, cclutSource)
        set cclutDelimPos = findString(cclutDelimiter, cclutSource)
    endwhile
    set cclutItemCount = cclutItemCount + 1
    set cclutStat = alterlist(cclutRec->qual, cclutItemCount)
    set cclutRec->qual[cclutItemCount].str = cclutSource
    return (cclutItemCount)
end

%#endif CCLUT_UTILS_INC
