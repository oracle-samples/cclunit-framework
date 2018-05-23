drop program cclutAssertTestNotLessThan go
create program cclutAssertTestNotLessThan
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 f8LessThan[1]
 *      2 value = f8
 *  1 f8NotLessThan[1]
 *      2 value = f8
 *      2 comparisonValue = f8
 *  1 i4LessThan[1]
 *      2 value = i4
 *  1 i4NotLessThan[1]
 *      2 value = i4
 *      2 comparisonValue = i4
 *  1 i2LessThan[1]
 *      2 value = i2
 *  1 i2NotLessThan[1]
 *      2 value = i2
 *      2 comparisonValue = i2
 *  1 vcLessThan[1]
 *      2 value = vc
 *  1 vcNotLessThan[1]
 *      2 value = vc
 *      2 comparisonValue = vc
 *  1 dateLessThan[1]
 *      2 value = dq8
 *  1 dateNotLessThan[1]
 *      2 value = dq8
 *      2 comparisonValue = dq8
 * )
 */

/*
 * reply (
 *  1 f8LessThanResponse = i2
 *  1 f8NotLessThanResponse = i2
 *  1 i4LessThanResponse = i2
 *  1 i4NotLessThanResponse = i2
 *  1 i2LessThanResponse = i2
 *  1 i2NotLessThanResponse = i2
 *  1 vcLessThanResponse = i2
 *  1 vcNotLessThanResponse = i2
 *  1 dateLessThanResponse = i2
 *  1 dateNotLessThanResponse = i2
 * )
 */

set reply->f8LessThanResponse = cclutAssertf8NotLessThan(1, "f8LessThan", 
    request->f8LessThan.value, request->f8LessThan.value + 1)
set reply->f8NotLessThanResponse = cclutAssertf8NotLessThan(2, "f8NotLessThan",
    request->f8NotLessThan.value, request->f8NotLessThan.comparisonValue)
    
set reply->i4LessThanResponse = cclutAsserti4NotLessThan(3, "i4LessThan",
    request->i4LessThan.value, request->i4LessThan.value + 1)
set reply->i4NotLessThanResponse = cclutAsserti4NotLessThan(4, "i4NotLessThan",
    request->i4NotLessThan.value, request->i4NotLessThan.comparisonValue)

set reply->i2LessThanResponse = cclutAsserti2NotLessThan(5, "i2LessThan",
    request->i2LessThan.value, request->i2LessThan.value + 1)
set reply->i2NotLessThanResponse = cclutAsserti2NotLessThan(4, "i2NotLessThan",
    request->i2NotLessThan.value, request->i2NotLessThan.comparisonValue)

set reply->vcLessThanResponse = cclutAssertvcNotLessThan(6, "vcLessThan",
    request->vcLessThan.value, concat(request->vcLessThan.value, "a"))
set reply->vcNotLessThanResponse = cclutAssertvcNotLessThan(7, "vcNotLessThan",
    request->vcNotLessThan.value, request->vcNotLessThan.comparisonValue)

set reply->dateLessThanResponse = cclutAssertdatetimeNotLessThan(8, "dateLessThan",
    request->dateLessThan.value, datetimeadd(request->dateLessThan.value, 1))
set reply->dateNotLessThanResponse = cclutAssertdatetimeNotLessThan(9, "dateNotLessThan",
    request->dateNotLessThan.value, request->dateNotLessThan.comparisonValue)

end
go