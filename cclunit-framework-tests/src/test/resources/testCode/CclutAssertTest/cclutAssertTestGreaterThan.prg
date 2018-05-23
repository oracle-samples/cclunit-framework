drop program cclutAssertTestGreaterThan go
create program cclutAssertTestGreaterThan
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 f8GreaterThan[1]
 *      2 value = f8
 *  1 f8NotGreaterThan[1]
 *      2 value = f8
 *      2 comparisonValue = f8
 *  1 i4GreaterThan[1]
 *      2 value = i4
 *  1 i4NotGreaterThan[1]
 *      2 value = i4
 *      2 comparisonValue = i4
 *  1 i2GreaterThan[1]
 *      2 value = i2
 *  1 i2NotGreaterThan[1]
 *      2 value = i2
 *      2 comparisonValue = i2
 *  1 vcGreaterThan[1]
 *      2 value = vc
 *  1 vcNotGreaterThan[1]
 *      2 value = vc
 *      2 comparisonValue = vc
 *  1 dateGreaterThan[1]
 *      2 value = dq8
 *  1 dateNotGreaterThan[1]
 *      2 value = dq8
 *      2 comparisonValue = dq8
 * )
 */

/*
 * reply (
 *  1 f8GreaterThanResponse = i2
 *  1 f8NotGreaterThanResponse = i2
 *  1 i4GreaterThanResponse = i2
 *  1 i4NotGreaterThanResponse = i2
 *  1 i2GreaterThanResponse = i2
 *  1 i2NotGreaterThanResponse = i2
 *  1 vcGreaterThanResponse = i2
 *  1 vcNotGreaterThanResponse = i2
 *  1 dateGreaterThanResponse = i2
 *  1 dateNotGreaterThanResponse = i2
 * )
 */

set reply->f8GreaterThanResponse = cclutAssertf8GreaterThan(1, "f8GreaterThan", 
    request->f8GreaterThan.value + 1, request->f8GreaterThan.value)
set reply->f8NotGreaterThanResponse = cclutAssertf8GreaterThan(2, "f8NotGreaterThan",
    request->f8NotGreaterThan.value, request->f8NotGreaterThan.comparisonValue)
    
set reply->i4GreaterThanResponse = cclutAsserti4GreaterThan(3, "i4GreaterThan",
    request->i4GreaterThan.value + 1, request->i4GreaterThan.value)
set reply->i4NotGreaterThanResponse = cclutAsserti4GreaterThan(4, "i4NotGreaterThan",
    request->i4NotGreaterThan.value, request->i4NotGreaterThan.comparisonValue)

set reply->i2GreaterThanResponse = cclutAsserti2GreaterThan(5, "i2GreaterThan",
    request->i2GreaterThan.value + 1, request->i2GreaterThan.value)
set reply->i2NotGreaterThanResponse = cclutAsserti2GreaterThan(4, "i2NotGreaterThan",
    request->i2NotGreaterThan.value, request->i2NotGreaterThan.comparisonValue)

set reply->vcGreaterThanResponse = cclutAssertvcGreaterThan(6, "vcGreaterThan",
    concat(request->vcGreaterThan.value, "a"), request->vcGreaterThan.value)
set reply->vcNotGreaterThanResponse = cclutAssertvcGreaterThan(7, "vcNotGreaterThan",
    request->vcNotGreaterThan.value, request->vcNotGreaterThan.comparisonValue)

set reply->dateGreaterThanResponse = cclutAssertdatetimeGreaterThan(8, "dateGreaterThan",
    datetimeadd(request->dateGreaterThan.value, 1), request->dateGreaterThan.value)
set reply->dateNotGreaterThanResponse = cclutAssertdatetimeGreaterThan(9, "dateNotGreaterThan",
    request->dateNotGreaterThan.value, request->dateNotGreaterThan.comparisonValue)

end
go