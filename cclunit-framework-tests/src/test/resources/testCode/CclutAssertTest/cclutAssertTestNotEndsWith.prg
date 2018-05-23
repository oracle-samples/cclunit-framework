drop program cclutAssertTestNotEndsWith go
create program cclutAssertTestNotEndsWith
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 endsWith[1]
 *      2 string = vc
 *      2 substring = vc
 *  1 notEndsWith[1]
 *      2 string = vc
 *      2 substring = vc
 * )
 */

/*
 * reply (
 *  1 endsWithResponse = i2
 *  1 notEndsWithResponse = i2
 * )
 */

set reply->endsWithResponse = cclutAssertNotEndsWith(1, "endsWith",
    request->endsWith.substring, request->endsWith.string)
set reply->notEndsWithResponse = cclutAssertNotEndsWith(2, "notEndsWith",
    request->notEndsWith.substring, request->notEndsWith.string)

end
go