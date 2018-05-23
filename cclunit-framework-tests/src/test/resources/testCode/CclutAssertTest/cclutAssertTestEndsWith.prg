drop program cclutAssertTestEndsWith go
create program cclutAssertTestEndsWith
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

set reply->endsWithResponse = cclutAssertEndsWith(1, "endsWith",
    request->endsWith.substring, request->endsWith.string)
set reply->notEndsWithResponse = cclutAssertEndsWith(2, "notEndsWith",
    request->notEndsWith.substring, request->notEndsWith.string)

end
go