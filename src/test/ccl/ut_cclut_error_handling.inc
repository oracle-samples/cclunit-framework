declare testGetErrorStackErrors(null) = null
declare testStashErrors(null) = null
declare testGetErrorListEmptyErrorStash(null) = null
declare testGetErrorListNonEmptyErrorStash(null) = null  


/**
  Verifies the behavior of cclut::getErrorStackErrors
*/
subroutine testGetErrorStackErrors(null)
  call cclException(303, "E", "error one")
  call cclException(304, "E", "error two")
  call cclException(305, "E", "error three")
  call cclException(306, "E", "error four")
  call cclException(307, "E", "error five")
  call cclException(308, "E", "error six")

  declare idx = i4 with protect
  declare errorCode = i4 with protect
  declare errorMessage = vc with protect
  record errorList(
    1 error[*]
      2 errorCode = i4
      2 errorMessage = vc
      2 lineNumber = i4
  ) with protect

  call cclut::getErrorList(errorList)
  set errorCode = error(errorMessage, 0)
  
  call cclutAssertI4Equal(CURREF, "error count", size(errorList->error, 5), 5)
  for (idx = 1 to 5)
    call cclutAssertI4Equal(CURREF, "error code", errorList->error[idx].errorCode, 392)
  endfor
  call cclutAssertContains(CURREF, "errorList->error one", 
        "Exception(308): error six", errorList->error[1].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error two", 
        "Exception(306): error four", errorList->error[2].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error three", 
        "Exception(305): error three", errorList->error[3].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error four", 
        "Exception(304): error two", errorList->error[4].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error five", 
        "Exception(303): error one", errorList->error[5].errorMessage)
            
  call cclutAssertI4Equal(CURREF, "error code", errorCode, 0)
end ;testGetErrorStackErrors

/**
  Verifies the behavior of cclut::stashErrors
*/
subroutine testStashErrors(null)
    declare stat = i4 with protect, noconstant(0)
    call cclutAssertI4Equal(CURREF, "size cclutErrorStash->error A", size(cclutErrorStash->error, 5), 0)

    call cclException(303, "E", "error one")
    call cclException(304, "E", "error two")
    call cclut::stashErrors(null)

    call cclutAssertI4Equal(CURREF, "size cclutErrorStash->error B", size(cclutErrorStash->error, 5), 2)
    call cclutAssertI4Equal(CURREF, "cclutErrorStash->error code one A", cclutErrorStash->error[1].errorCode, 392)
    call cclutAssertContains(CURREF, "cclutErrorStash->error one A", 
        "Exception(304): error two", cclutErrorStash->error[1].errorMessage)
    call cclutAssertI4Equal(CURREF, "cclutErrorStash->error code two A", cclutErrorStash->error[2].errorCode, 392)
    call cclutAssertContains(CURREF, "cclutErrorStash->error two A", 
        "Exception(303): error one", cclutErrorStash->error[2].errorMessage)

    call cclException(305, "E", "error three")
    call cclException(306, "E", "error four")
    call cclut::stashErrors(null)

    call cclutAssertI4Equal(CURREF, "cclutErrorStash->error code one B", cclutErrorStash->error[1].errorCode, 392)
    call cclutAssertContains(CURREF, "cclutErrorStash->error one B", 
        "Exception(304): error two", cclutErrorStash->error[1].errorMessage)
    call cclutAssertI4Equal(CURREF, "cclutErrorStash->error code two B", cclutErrorStash->error[2].errorCode, 392)
    call cclutAssertContains(CURREF, "cclutErrorStash->error two B", 
        "Exception(303): error one", cclutErrorStash->error[2].errorMessage)

    call cclutAssertI4Equal(CURREF, "cclutErrorStash->error code three B", cclutErrorStash->error[3].errorCode, 392)
    call cclutAssertContains(CURREF, "cclutErrorStash->error three B", 
        "Exception(306): error four", cclutErrorStash->error[3].errorMessage)
    call cclutAssertI4Equal(CURREF, "cclutErrorStash->error code four B", cclutErrorStash->error[4].errorCode, 392)
    call cclutAssertContains(CURREF, "cclutErrorStash->error four B", 
        "Exception(305): error three", cclutErrorStash->error[4].errorMessage)
    set stat = alterlist(cclutErrorStash->error, 0)
end ;testStashErrors


/**
  Verifies the behavior of cclut::getErrorList when cclutErrorStash is empty
*/
subroutine testGetErrorListEmptyErrorStash(null)
  call cclException(303, "E", "error one")
  call cclException(304, "E", "error two")
  call cclException(305, "E", "error three")
  call cclException(306, "E", "error four")
  call cclException(307, "E", "error five")
  call cclException(308, "E", "error six")

  declare idx = i4 with protect
  declare errorCode = i4 with protect
  declare errorMessage = vc with protect
  record errorList(
    1 error[*]
      2 errorCode = i4
      2 errorMessage = vc
      2 lineNumber = i4
  ) with protect


  call cclut::getErrorList(errorList)    
  call cclutAssertI4Equal(CURREF, "cclutErrorStash is empty", size(cclutErrorStash->error, 5), 0)

  set errorCode = error(errorMessage, 0)

  call cclutAssertI4Equal(CURREF, "error count", size(errorList->error, 5), 5)
  for (idx = 1 to 5)
    call cclutAssertI4Equal(CURREF, "error code", errorList->error[idx].errorCode, 392)
  endfor
  call cclutAssertContains(CURREF, "errorList->error one", 
        "Exception(308): error six", errorList->error[1].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error two", 
        "Exception(306): error four", errorList->error[2].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error three", 
        "Exception(305): error three", errorList->error[3].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error four", 
        "Exception(304): error two", errorList->error[4].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error five", 
        "Exception(303): error one", errorList->error[5].errorMessage)
            
  call cclutAssertI4Equal(CURREF, "error code", errorCode, 0)
end ;testGetErrorListEmptyErrorStash


/**
  Verifies the behavior of cclut::getErrorList when cclutErrorStash is not empty
*/
subroutine testGetErrorListNonEmptyErrorStash(null)  
  call cclException(303, "E", "error one")
  call cclException(304, "E", "error two")
  call cclException(305, "E", "error three")
  call cclException(306, "E", "error four")
  call cclException(307, "E", "error five")
  call cclException(308, "E", "error six")

  declare stat = i4 with protect
  declare idx = i4 with protect
  declare errorCode = i4 with protect
  declare errorMessage = vc with protect
  record errorList(
    1 error[*]
      2 errorCode = i4
      2 errorMessage = vc
      2 lineNumber = i4
  ) with protect

  set stat = alterlist(cclutErrorStash->error, 3)
  set cclutErrorStash->error[1].errorCode = 1
  set cclutErrorStash->error[1].errorMessage = "stashed error one"
  set cclutErrorStash->error[1].lineNumber = 101
  set cclutErrorStash->error[2].errorCode = 2
  set cclutErrorStash->error[2].errorMessage = "stashed error two"
  set cclutErrorStash->error[2].lineNumber = 202
  set cclutErrorStash->error[3].errorCode = 3
  set cclutErrorStash->error[3].errorMessage = "stashed error three"
  set cclutErrorStash->error[3].lineNumber = 303

  call cclut::getErrorList(errorList)
  call cclutAssertI4Equal(CURREF, "error count", size(errorList->error, 5), 8)
  set errorCode = error(errorMessage, 0)

  call cclutAssertI4Equal(CURREF, "error code", errorCode, 0)
  call cclutAssertI4Equal(CURREF, "error stash", size(cclutErrorStash->error, 5), 0)

  call cclutAssertI4Equal(CURREF, "error code one", errorList->error[1].errorCode, 1)
  call cclutAssertVcEqual(CURREF, "error one", errorList->error[1].errorMessage, "stashed error one")
  call cclutAssertI4Equal(CURREF, "error line one", errorList->error[1].lineNumber, 101)

  call cclutAssertI4Equal(CURREF, "error code two", errorList->error[2].errorCode, 2)
  call cclutAssertVcEqual(CURREF, "error two", errorList->error[2].errorMessage, "stashed error two")
  call cclutAssertI4Equal(CURREF, "error line two", errorList->error[2].lineNumber, 202)

  call cclutAssertI4Equal(CURREF, "error code three", errorList->error[3].errorCode, 3)
  call cclutAssertVcEqual(CURREF, "error three", errorList->error[3].errorMessage, "stashed error three")
  call cclutAssertI4Equal(CURREF, "error line three", errorList->error[3].lineNumber, 303)

  for (idx = 4 to 8)
    call cclutAssertI4Equal(CURREF, "error code", errorList->error[idx].errorCode, 392)
  endfor
  call cclutAssertContains(CURREF, "errorList->error four", 
        "Exception(308): error six", errorList->error[4].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error five", 
        "Exception(306): error four", errorList->error[5].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error six", 
        "Exception(305): error three", errorList->error[6].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error seven", 
        "Exception(304): error two", errorList->error[7].errorMessage)
  call cclutAssertContains(CURREF, "errorList->error eight", 
        "Exception(303): error one", errorList->error[8].errorMessage)

end ;testGetErrorListNonEmptyErrorStash

