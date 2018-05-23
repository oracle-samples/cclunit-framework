%#ifndef CCLUT_ENV_UTILS_INC
%#def CCLUT_ENV_UTILS_INC

;**********************************************************************************************************************************
;
; Contains subroutines for evaluating the current runtime environment.
;
;**********************************************************************************************************************************

declare cclut::compareCclVersion(lhs = vc, rhs = vc) = i2
declare cclut::getCclVersion(null) = vc

/**
  Simple routine for determining if the LHS CCL version is less than the RHS
  @param lhs
    A X.X.X formated CCL version (left hand side)
  @param rhs 
    A X.X.X formated CCL version (right hand side)
  @returns
    A boolean flag indicating whether LHS version is less than the RHS
    @value TRUE
      lhs version is less than the rhs
    @value FALSE
      lhs version is not less than the rhs
*/
subroutine cclut::compareCclVersion(lhs, rhs)
  declare cclutLhsNumber       = i4 with protect, noconstant(0)
  declare cclutRhsNumber       = i4 with protect, noconstant(0)
  declare cclutLhsPeriodPos    = i4 with protect, noconstant(findstring(".", lhs))
  declare cclutRhsPeriodPos    = i4 with protect, noconstant(findstring(".", rhs))
  
  declare cclutLhsTail = vc with protect, noconstant("")
  declare cclutRhsTail = vc with protect, noconstant("")
  declare cclutRet     = i2 with protect, noconstant(FALSE)
  
  if (cclutLhsPeriodPos = 0 or cclutRhsPeriodPos = 0)
    if (cnvtint(lhs) < cnvtint(rhs))
      return (TRUE)
    else
      return (FALSE)
    endif
  endif
  
  set cclutLhsNumber = cnvtint(substring(1, cclutLhsPeriodPos, lhs))
  set cclutRhsNumber = cnvtint(substring(1, cclutRhsPeriodPos, rhs))
  
  if (cclutLhsNumber = cclutRhsNumber)
    set cclutLhsTail = substring(cclutLhsPeriodPos + 1, 100, lhs)
    set cclutRhsTail = substring(cclutRhsPeriodPos + 1, 100, rhs)
    
    set cclutRet = cclut::compareCclVersion(cclutLhsTail, cclutRhsTail)
    
    return (cclutRet)
  elseif (cclutLhsNumber < cclutRhsNumber)
    return (TRUE)
  else
    return (FALSE)
  endif
end ;;;compareCclVersion()

/**
  Return the current CCL version in X.X.X format.
  @returns
    The current CCL version.
*/
subroutine cclut::getCclVersion(null)
  return (concat(trim(cnvtstring(currev)), ".", trim(cnvtstring(currevminor)), ".", trim(cnvtstring(currevminor2))))
end ;;;getCclVersion()

%#endif CCLUT_ENV_UTILS_INC