%#ifndef CCLUTASSERT_IMPL_BASE_INC
%#def CCLUTASSERT_IMPL_BASE_INC
;**********************************************************************************************************************************
;
; Provides a base implementation for the cclutAssert* subroutines which can be called to ensure values match what is expected.
; The exact behavior is decided by the specific implementations of the evaluateCondition and evaluateComparison 
; subroutines which are made avaialable.
;
;**********************************************************************************************************************************

%i cclsource:cclutassert_declare.inc 

set modify maxvarlen 10000000

/**
  Container for the results of the assert statements executed during a test run.
  @field line
    The result of a single assert statement.
    @field cclutLineNum
      The lineNumber provided to the assert statement.
    @field cclutCtx
      The context message provided to the assert statement.
    @field result
      The result of the assert statement.
      @value PASS
        The assert statement passed.
      @value FAIL
        The assert statement failed.
    @field datetime
      The date/time when the result of assert statement was reported.
    @field test
      The cclutCnd that was evaluated by the assert statement.
    @field errorCodeBefore
      The error code in the error code before the assert statment was evaluated.
    @field errorMessageBefore 
      The error message in the error code before the assert statment was evaluated.
    @field  errorCode
      The error code generated while evaluating the assert statment.
    @field errorMessageBefore 
       The error message generated while evaluating the assert statment.
*/
record cclut_runResult (
  1 line[*]
    2 lineNumber = i4
    2 context = vc
    2 result = vc
    2 datetime = vc
    2 condition = vc
    2 errorCodeBefore = i4
    2 errorMessageBefore = vc
    2 errorCode = i4
    2 errorMessage = vc
%i cclsource:status_block.inc    
) with protect

;**********************************************************************************************************************************
;** Implementation Support
;**********************************************************************************************************************************
declare cclut::evaluateCondition(cclutLineNum = i4, cclutCtx = vc, cclutCondition = vc) = i2 
declare cclut::evaluateCondition2(cclutLineNum = i4, cclutCtx = vc, cclutCondition = vc, cclutDisp = vc) = i2 
declare cclut::evaluateComparison(cclutLineNum = i4, cclutCtx = vc, cclutComparison = vc, cclutVal1 = vc, cclutVal2 = vc) = i2

;**********************************************************************************************************************************
;** Equal
;**********************************************************************************************************************************
subroutine cclutAssertf8Equal(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "=", cclutCompVal)))
end ;;;clutAssertf8Equal
 
subroutine cclutAsserti4Equal(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "=", cclutCompVal)))
end ;;;clutAsserti4Equal
 
subroutine cclutAsserti2Equal(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "=", cclutCompVal)))
end ;;;clutAsserti2Equal
 
subroutine cclutAssertVCEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "=", cclutVal, cclutCompVal))
end; cclutAssertVCEqual
 
subroutine cclutAssertdatetimeEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build2("'", format(cclutVal, "yyyy-mm-dd hh:mm:ss;;d"), "'",
                                        "=", "'", format(cclutCompVal, "yyyy-mm-dd hh:mm:ss;;d"), "'")))
end; cclutAssertdatetimeEqual
 
;**********************************************************************************************************************************
;** AlmostEqual
;**********************************************************************************************************************************
subroutine cclutAssertf8AlmostEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal, cclutTolerance)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build("abs(", cclutVal, "-", cclutCompVal, ") <= ", cclutTolerance)))
end ;;;clutAssertf8AlmostEqual
 
subroutine cclutAssertf8NotAlmostEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal, cclutTolerance)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build("abs(", cclutVal, "-", cclutCompVal, ") > ", cclutTolerance)))
end ;;;clutAssertf8NotAlmostEqual

;**********************************************************************************************************************************
;** NotEqual
;**********************************************************************************************************************************
subroutine cclutAssertf8NotEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "!=", cclutCompVal)))
end ;;;clutAssertf8NotEqual
 
subroutine cclutAsserti4NotEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "!=", cclutCompVal)))
end ;;;clutAsserti4NotEqual
 
subroutine cclutAsserti2NotEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "!=", cclutCompVal)))
end ;;;clutAsserti2NotEqual
 
subroutine cclutAssertVCNotEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "!=", cclutVal, cclutCompVal))
end; cclutAssertNotEqual
 
subroutine cclutAssertdatetimeNotEqual(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build2("'", format(cclutVal, "yyyy-mm-dd hh:mm:ss;;d"), "'", "!=", 
          "'", format(cclutCompVal, "yyyy-mm-dd hh:mm:ss;;d"), "'")))
end; cclutAssertdatetimeNotEqual
 
;**********************************************************************************************************************************
;** LessThan
;**********************************************************************************************************************************
subroutine cclutAssertf8LessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "<", cclutCompVal)))
end ;;;clutAssertf8LessThan
 
subroutine cclutAsserti4LessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "<", cclutCompVal)))
end ;;;clutAsserti4LessThan
 
subroutine cclutAsserti2LessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "<", cclutCompVal)))
end ;;;clutAsserti2LessThan
 
subroutine cclutAssertVCLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "<", cclutVal, cclutCompVal))
end; cclutAssertVCLessThan
 
subroutine cclutAssertDateTimeLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build2("'", format(cclutVal, "yyyy-mm-dd hh:mm:ss;;d"), "'", "<", 
          "'", format(cclutCompVal, "yyyy-mm-dd hh:mm:ss;;d"), "'")))
end; cclutAssertdatetimeLessThan
 
;**********************************************************************************************************************************
;** NotLessThan
;**********************************************************************************************************************************
subroutine cclutAssertf8NotLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, ">=", cclutCompVal)))
end ;;;clutAssertf8NotLessThan
 
subroutine cclutAsserti4NotLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, ">=", cclutCompVal)))
end ;;;clutAsserti4NotLessThan
 
subroutine cclutAsserti2NotLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, ">=", cclutCompVal)))
end ;;;clutAsserti2NotLessThan
 
subroutine cclutAssertVCNotLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, ">=", cclutVal, cclutCompVal))
end; cclutAssertVCNotLessThan
 
subroutine cclutAssertdatetimeNotLessThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build2("'", format(cclutVal, "yyyy-mm-dd hh:mm:ss;;d"), "'", ">=", 
          "'", format(cclutCompVal, "yyyy-mm-dd hh:mm:ss;;d"), "'")))
end; cclutAssertdatetimeNotLessThan
 
;**********************************************************************************************************************************
;** GreaterThan
;**********************************************************************************************************************************
subroutine cclutAssertf8GreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, ">", cclutCompVal)))
end ;;;clutAssertf8GreaterThan
 
subroutine cclutAsserti4GreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, ">", cclutCompVal)))
end ;;;clutAsserti4GreaterThan
 
subroutine cclutAsserti2GreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, ">", cclutCompVal)))
end ;;;clutAsserti2GreaterThan
 
subroutine cclutAssertVCGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, ">", cclutVal, cclutCompVal))
end; cclutAssertVCGreaterThan
 
subroutine cclutAssertdatetimeGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build2("'", format(cclutVal, "yyyy-mm-dd hh:mm:ss;;d"), "'", ">", 
          "'", format(cclutCompVal, "yyyy-mm-dd hh:mm:ss;;d"), "'")))
end; cclutAssertdatetimeGreaterThan
 
;**********************************************************************************************************************************
;** NotGreaterThan
;**********************************************************************************************************************************
subroutine cclutAssertf8NotGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "<=", cclutCompVal)))
end ;;;clutAssertf8NotGreaterThan
 
subroutine cclutAsserti4NotGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "<=", cclutCompVal)))
end ;;;clutAsserti4NotGreaterThan
 
subroutine cclutAsserti2NotGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, build(cclutVal, "<=", cclutCompVal)))
end ;;;clutAsserti2NotGreaterThan
 
subroutine cclutAssertVCNotGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "<=", cclutVal, cclutCompVal))
end; cclutAssertVCNotGreaterThan
 
subroutine cclutAssertdatetimeNotGreaterThan(cclutLineNum, cclutCtx, cclutVal, cclutCompVal)
  return (cclut::evaluateCondition(cclutLineNum, cclutCtx, 
      build2("'", format(cclutVal, "yyyy-mm-dd hh:mm:ss;;d"), "'", "<=", 
          "'", format(cclutCompVal, "yyyy-mm-dd hh:mm:ss;;d"), "'")))
end; cclutAssertdatetimeNotGreaterThan
 
;**********************************************************************************************************************************
;** Contains
;**********************************************************************************************************************************
subroutine cclutAssertContains(cclutLineNum, cclutCtx, cclutSubstring, cclutString)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "contains", cclutSubstring, cclutString))
end; cclutAssertContains
 
subroutine cclutAssertNotContains(cclutLineNum, cclutCtx, cclutSubstring, cclutString)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "not contains", cclutSubstring, cclutString))
end; cclutAssertNotContains
 
;**********************************************************************************************************************************
;** StartsWith
;**********************************************************************************************************************************
subroutine cclutAssertStartsWith(cclutLineNum, cclutCtx, cclutSubstring, cclutString)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "starts with", cclutSubstring, cclutString))
end; cclutAssertStartsWith
 
subroutine cclutAssertNotStartsWith(cclutLineNum, cclutCtx, cclutSubstring, cclutString)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "not starts with", cclutSubstring, cclutString))
end; cclutAssertNotStartsWith
 
;**********************************************************************************************************************************
;** EndsWith
;**********************************************************************************************************************************
subroutine cclutAssertEndsWith(cclutLineNum, cclutCtx, cclutSubstring, cclutString)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "ends with", cclutSubstring, cclutString))
end; cclutAssertEndsWith
 
subroutine cclutAssertNotEndsWith(cclutLineNum, cclutCtx, cclutSubstring, cclutString)
  return (cclut::evaluateComparison(cclutLineNum, cclutCtx, "not ends with", cclutSubstring, cclutString))
end; cclutAssertNotEndsWith

;**********************************************************************************************************************************
;** Operartor
;**********************************************************************************************************************************
subroutine cclutAssertF8Operator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(operator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("operator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertF8Operator

subroutine cclutAssertF8NotOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(notoperator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("notoperator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertF8NotOperator

subroutine cclutAssertI4Operator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(operator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("operator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertI4Operator

subroutine cclutAssertI4NotOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(notoperator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("notoperator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertI4NotOperator

subroutine cclutAssertI2Operator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(operator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("operator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertI2Operator

subroutine cclutAssertI2NotOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(notoperator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("notoperator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertI2NotOperator

subroutine cclutAssertVCOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, noconstant(build2(trim(build(operator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, noconstant(build2("operator('",cclutExp1, "', '", cclutOp, "', '", cclutExp2, "')"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertVCOperator

subroutine cclutAssertVCNotOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, noconstant(build2(trim(build(notoperator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, noconstant(build2("notoperator('", cclutExp1, "', '", cclutOp, "', '", cclutExp2, "')"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertVCNotOperator

subroutine cclutAssertDateTimeOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(operator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("operator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertDateTimeOperator

subroutine cclutAssertDateTimeNotOperator(cclutLineNum, cclutCtx, cclutExp1, cclutOp, cclutExp2)
  declare cclutCnd = vc with protect, 
      noconstant(build2(trim(build(notoperator(cclutExp1, cclutOp, cclutExp2)),3), "=1"))
  declare cclutCndDisp = vc with protect, 
      noconstant(build2("notoperator(", trim(build(cclutExp1),3), ", '", cclutOp, "', ", trim(build(cclutExp2),3) , ")"))
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCnd, cclutCndDisp))
end ;;;cclutAssertDateTimeNotOperator

%#endif CCLUTASSERT_IMPL_BASE_INC