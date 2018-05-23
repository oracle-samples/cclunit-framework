drop program cclutAssertTestContains go
create program cclutAssertTestContains
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 contains[1]
 *      2 string = vc
 *      2 substring = vc
 *  1 notContains[1]
 *      2 string = vc
 *      2 substring = vc
 * )
 */

/*
 * reply (
 *  1 containsResponse = i2
 *  1 notContainResponse = i2
 * )
 */

set reply->containsResponse = cclutAssertContains(1, "contains",
    request->contains.substring, request->contains.string)
set reply->notContainsResponse = cclutAssertContains(2, "notContains",
    request->notContains.substring, request->notContains.string)

end
go
