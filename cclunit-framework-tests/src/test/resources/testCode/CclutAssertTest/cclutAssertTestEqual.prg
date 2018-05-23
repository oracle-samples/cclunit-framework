drop program cclutAssertTestEqual go
create program cclutAssertTestEqual
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 f8Equal[1]
 *      2 value = f8
 *  1 f8Inequal[1]
 *      2 expected = f8
 *      2 actual = f8
 *  1 i4Equal[1]
 *      2 value = i4
 *  1 i4Inequal[1]
 *      2 expected = i4
 *      2 actual = i4
 *  1 i2Equal[1]
 *      2 value = i2
 *  1 i2Inequal[1]
 *      2 expected = i2
 *      2 actual = i2
 *  1 vcEqual[1]
 *      2 value = vc
 *  1 vcInequal[1]
 *      2 expected = vc
 *      2 actual = vc
 *  1 dateEqual[1]
 *      2 value = dq8
 *  1 dateInequal[1]
 *      2 expected = dq8
 *      2 actual = dq8
 * )
 */

/*
 * reply (
 *  1 f8EqualResponse = i2
 *  1 f8InequalResponse = i2
 *  1 i4EqualResponse = i2
 *  1 i4InequalResponse = i2
 *  1 i2EqualResponse = i2
 *  1 i2InequalResponse = i2
 *  1 vcEqualResponse = i2
 *  1 vcInequalResponse = i2
 *  1 dateEqualResponse = i2
 *  1 dateInequalResponse = i2
 * )
 */

set reply->f8EqualResponse = cclutAssertf8Equal(1, "f8Equal", 
    request->f8Equal.value, request->f8Equal.value)
set reply->f8InequalResponse = cclutAssertf8Equal(2, "f8Inequal",
    request->f8Inequal.expected, request->f8Inequal.actual)
    
set reply->i4EqualResponse = cclutAsserti4Equal(3, "i4Equal",
    request->i4Equal.value, request->i4Equal.value)
set reply->i4InequalResponse = cclutAsserti4Equal(4, "i4Inequal",
    request->i4Inequal.expected, request->i4Inequal.actual)

set reply->i2EqualResponse = cclutAsserti2Equal(5, "i2Equal",
    request->i2Equal.value, request->i2Equal.value)
set reply->i2InequalResponse = cclutAsserti2Equal(4, "i2Inequal",
    request->i2Inequal.expected, request->i2Inequal.actual)

set reply->vcEqualResponse = cclutAssertvcEqual(6, "vcEqual",
    request->vcEqual.value, request->vcEqual.value)
set reply->vcInequalResponse = cclutAssertvcEqual(7, "vcInequal",
    request->vcInequal.expected, request->vcInequal.actual)

set reply->dateEqualResponse = cclutAssertdatetimeEqual(8, "dateEqual",
    request->dateEqual.value, request->dateEqual.value)
set reply->dateInequalResponse = cclutAssertdatetimeEqual(9, "dateInequal",
    request->dateInequal.expected, request->dateInequal.actual)

end
go