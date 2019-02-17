%#ifndef CCLUT_ERROR_HANDLING_INC
%#def CCLUT_ERROR_HANDLING_INC

;**********************************************************************************************************************************
;
; Contains subroutines used to determine if CCL errors occurred.
;
;**********************************************************************************************************************************


declare cclut::getErrorList(cclutPErrorList = vc(ref)) = i4
declare cclut::errorOccurred(cclutPErrorMessage = vc(ref)) = i2
declare cclut::getCclErrorLineNumber(cclutPErrorMessage = vc) = i4
declare cclut::exitOnError(cclutOperation = vc, cclutObject = vc, cclutStruct = vc(ref)) = null


/**
  Retrievs the errors for the error queue and clears it.
  @param errorList
    A return buffer for the error list
    <pre>
      record errorList(
        1 error[*]
          2 errorCode = i4
          2 errorMessage = vc
          2 lineNumber = i4
      ) with protect
    </pre>
  @returns
    A boolean flag indicating whether there were any errors: TRUE there were errors; FALSE there were not.
*/
subroutine cclut::getErrorList(cclutPErrorList)
  declare cclutErrorMessage = vc with protect, noconstant("")
  declare cclutErrorCode = i2 with protect, noconstant(0)
  declare cclutErrorCount = i4 with protect, noconstant(size(cclutPErrorList->error, 5))
  declare cclutStat = i4 with protect, noconstant(0)
  
  set cclutErrorCode = error(cclutErrorMessage, 0)
  while (cclutErrorCode > 0)
    set cclutErrorCount = cclutErrorCount + 1
    set cclutStat = alterList(cclutPErrorList->error, cclutErrorCount)
    set cclutPErrorList->error[cclutErrorCount].errorCode = cclutErrorCode
    set cclutPErrorList->error[cclutErrorCount].errorMessage = cclutErrorMessage
    set cclutPErrorList->error[cclutErrorCount].lineNumber = cclut::getCclErrorLineNumber(cclutErrorMessage)
    set cclutErrorCode = error(cclutErrorMessage, 0)
  endwhile
  return (cclutErrorCount)
end ;getErrorList

/**
  Determines if CCL errors occurred and clears the error queue.
  @param cclutPErrorMessage 
    Return buffer for the error message if an error has occurred.
  @returns
    A boolean flag indicating whether a CCL error has occurred.
    @value TRUE
      At least one CCL error has occurred.
    @value FALSE
      No CCL errors have occurred.
*/ 
subroutine cclut::errorOccurred(cclutPErrorMessage)
  declare cclutErrorMessage = vc with protect, noconstant("")
  declare cclutErrorCode = i2 with protect, noconstant(0)

  set cclutErrorCode = error(cclutErrorMessage, 1)
  if (cclutErrorCode != 0)
    set cclutPErrorMessage = cclutErrorMessage
    return (TRUE)
  endif
  return (FALSE)
end ;errorOccurred
 
/**
  Parse a CCL error message to determine the line number on which the error message says the error occurred. 
  The CCL program in which the error occured  must have been compiled in debug mode for a non-zero value to be returned.
  @param cclutPErrorMessage 
    The CCL error message.
  @returns
    The line number at which the message indicates the error occurred.
*/
subroutine cclut::getCclErrorLineNumber(cclutPErrorMessage)
  declare cclutErrorIndex = i4 with protect, noconstant(0)
  declare cclutParenIndex = i4 with protect, noconstant(0)
  declare cclutCommaIndex = i4 with protect, noconstant(0)
  declare cclutLineNumber = i4 with protect, noconstant(0)

  set cclutErrorIndex = findstring("%CCL-E", cclutPErrorMessage, 1, 0)

  if (cclutErrorIndex > 0)
    set cclutParenIndex = findstring("(", cclutPErrorMessage, cclutErrorIndex+5, 0)
    set cclutCommaIndex = findstring(",", cclutPErrorMessage, cclutParenIndex, 0)

    if (cclutCommaIndex > 0 and cclutParenIndex > 0 and (cclutCommaIndex > cclutParenIndex))
      set cclutLineNumber = cnvtint(substring(cclutParenIndex+1, cclutCommaIndex-cclutParenIndex-1, cclutPErrorMessage))
    endif
  endif
  return (cclutLineNumber)
end ;;;getCclErrorLineNumber

/**
  Checks for errors and routes execution to exit_script if there are errors after first populating a provided record structure
  with error information as follows: the targetObjectValue is set equal to the error message; the status and operationStatus are
  both set equal to "F", the operationName and targetObjectValue are set equal to the provided operation and object.
  and the provided
  @param cclutOperation
    The operationName to set. Generally this will be the operation being performed at the time the function is called.
  @param cclutObject
    The targetOhjectName to set. Generally this will be the object on which the operation is being performed.
  @param cclutStruct 
    A record structure that includes the standard status_block.inc to be populated if there are errors.
*/
subroutine cclut::exitOnError(cclutOperation, cclutObject, cclutStruct)
  declare cclutMessage = vc with protect, noconstant("")
  if (cclut::errorOccurred(cclutMessage) = TRUE)
    if (cclutStruct->status_data.status != "F")
      set cclutStruct->status_data.status = "F"
      set cclutStruct->status_data.subeventstatus[1].operationName = cclutOperation
      set cclutStruct->status_data.subeventstatus[1].operationStatus = "F"
      set cclutStruct->status_data.subeventstatus[1].targetObjectName = cclutObject
      set cclutStruct->status_data.subeventstatus[1].targetObjectValue = cclutMessage
    endif
    go to exit_script
  endif
end ;;;exitOnError

%#endif CCLUT_ERROR_HANDLING_INC
