# CCL Unit Asserts

## Structure
Most CCL Unit assert statements have the following form  
&nbsp;&nbsp;&nbsp;   `set status = cclutAssert[Type]{Operator}(lineNumber, context, value1, value2)`   
A couple "almost equal" asserts for F8 values have the form  
&nbsp;&nbsp;&nbsp;   `set status = cclutAssertF8[Not]AlmostEqual(number, context, value1, value2, tolerance)`   
A group of "operator asserts" have the form  
&nbsp;&nbsp;&nbsp;   `set status = cclutAssert{Type}[Not]Operator(number, context, value1, operator, value2)`   

The value of the lineNumber parameter should be <b>CURREF</b> which translates to the current line number in CCL code that is compiled with debug. 
The cerreal plugin needs the value to be the actual line number in order to obtain the actual line of code from the compile listing file in order to display it
in the details for the assert.

There are a handful of string-only asserts which do not explicitly call out the data type. For all other asserts, 
the data types of the provided values must match the type identified in the name of the assert function. 
For most assert functions, the operator identified in the name of the function is applied to the provided values 
and the result is evaluated.  The assert passes and the assert function returns TRUE if and only if the result of the evaluation 
is true. The almost equal [not equal] asserts pass if and only if the provided values differ by less than [more than] the specified tolerance.
The family of operator asserts invoke CCL's operator [notoperator] function passing the provided values and operator. These asserts pass and return TRUE if and only if the
operator [notoperator] function returns TRUE.

The context can be any string. If selected uniquely, it can help identify which assert failed in case an assert failure occurs.

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
- cclutAssertDatetTimeEqual

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
- cclutAssertDatetTimeOperator

Not Operator
- cclutAssertF8NotOperator
- cclutAssertI4NotOperator
- cclutAssertI2NotOperator
- cclutAssertVCNotOperator
- cclutAssertDatetTimeNotOperator

