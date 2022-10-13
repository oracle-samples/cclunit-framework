# CCL Unit Asserts

## Structure
Most CCL Unit assert statements have the following form  
&nbsp;&nbsp;&nbsp;   `set status = cclutAssert[Type]{Operator}(lineNumber, context, value1, value2)`   
A couple "almost equal" asserts for F8 values have the form  
&nbsp;&nbsp;&nbsp;   `set status = cclutAssertF8[Not]AlmostEqual(number, context, value1, value2, tolerance)`   
A group of "operator asserts" have the form  
&nbsp;&nbsp;&nbsp;   `set status = cclutAssert{Type}[Not]Operator(number, context, value1, operator, value2)`   

The `status` value will be TRUE if the assert passes and FALSE otherwise. The return value does not have to be captured.   
`call cclutAssert...` works fine.

The value of the lineNumber parameter should be <b>CURREF</b> which CCL translates as the current line number for code compiled with debug. 
The [cerreal-maven-plugin][cerreal-maven-plugin] needs this line number to display assert statements correctly.

There are a handful of string-only asserts which do not explicitly call out the data type. For all other asserts, 
the data types of the provided values must match the type identified in the name of the assert function. 
For most assert functions, the operator identified in the name of the function is applied to the provided values 
and the result is evaluated.  The assert passes and the assert function returns TRUE if and only if the result of the evaluation 
is true. The almost equal [not equal] asserts pass if and only if the provided values differ by less than [more than] the specified tolerance.
The family of operator asserts invoke CCL's operator [notoperator] function passing the provided values and operator. These asserts pass and return TRUE if and only if the
operator [notoperator] function returns TRUE.

The context can be any string but using unique contexts will help identify which assert failed if an assert failure occurs.

## List of Asserts

String-Only
- cclutAssertContains
- cclutAssertNotContains
- cclutAssertStartsWith
- cclutAssertNotStartsWith
- cclutAssertEndsWith
- cclutAssertNotEndsWith

Almost Equal
- cclutAssertF8AlmostEqual
- cclutAssertF8NotAlmostEqual

Equal
- cclutAssertF8Equal
- cclutAssertI4Equal
- cclutAssertI2Equal
- cclutAssertVCEqual
- cclutAssertDateTimeEqual

Not Equal
- cclutAssertF8NotEqual
- cclutAssertI4NotEqual
- cclutAssertI2NotEqual
- cclutAssertVCNotEqual
- cclutAssertDateTimeNotEqual 

Less Than
- cclutAssertF8LessThan
- cclutAssertI4LessThan
- cclutAssertI2LessThan
- cclutAssertVCLessThan
- cclutAssertDateTimeLessThan

Not Less Than
- cclutAssertF8NotLessThan
- cclutAssertI4NotLessThan
- cclutAssertI2NotLessThan
- cclutAssertVCNotLessThan
- cclutAssertDateTimeNotLessThan

Greater Than
- cclutAssertF8GreaterThan
- cclutAssertI4GreaterThan
- cclutAssertI2GreaterThan
- cclutAssertVCGreaterThan
- cclutAssertDateTimeGreaterThan

Not Greater Than
- cclutAssertF8NotGreaterThan
- cclutAssertI4NotGreaterThan
- cclutAssertI2NotGreaterThan
- cclutAssertVCNotGreaterThan
- cclutAssertDateTimeNotGreaterThan

Operator
- cclutAssertF8Operator
- cclutAssertI4Operator
- cclutAssertI2Operator
- cclutAssertVCOperator
- cclutAssertDateTimeOperator

Not Operator
- cclutAssertF8NotOperator
- cclutAssertI4NotOperator
- cclutAssertI2NotOperator
- cclutAssertVCNotOperator
- cclutAssertDateTimeNotOperator

[cerreal-maven-plugin]: https://engineering.cerner.com/ccl-testing
