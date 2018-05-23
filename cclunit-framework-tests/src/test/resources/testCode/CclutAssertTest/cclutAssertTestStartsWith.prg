drop program cclutAssertTestStartsWith go
create program cclutAssertTestStartsWith
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 startsWith[1]
 *      2 string = vc
 *      2 substring = vc
 *  1 notStartsWith[1]
 *      2 string = vc
 *      2 substring = vc
 * )
 */

/*
 * reply (
 *  1 startsWithResponse = i2
 *  1 notStartsWithResponse = i2
 * )
 */

set reply->startsWithResponse = cclutAssertStartsWith(1, "startsWith",
    request->startsWith.substring, request->startsWith.string)
set reply->notStartsWithResponse = cclutAssertStartsWith(2, "notStartsWith",
    request->notStartsWith.substring, request->notStartsWith.string)

end
go