drop program cclutAssertTestAlmostEqual go
create program cclutAssertTestAlmostEqual
%i cclsource:cclutassert_impl_nff.inc

/*
 * request (
 *  1 almostEqual[1]
 *      2 expected = f8
 *      2 actual = f8
 *      2 delta = f8
 *  2 notAlmostEqual[1]
 *      2 expected = f8
 *      2 actual = f8
 *      2 delta = f8
 * )
 */

/*
 * reply (
 *  1 almostEqualResponse = i2
 *  1 notAlmostEqualResponse = i2
 * )
 */

set reply->almostEqualResponse = cclutAssertf8AlmostEqual(1, "almostEqual",
    request->almostEqual.expected, request->almostEqual.actual,
    request->almostEqual.delta)
set reply->notAlmostEqualResponse = cclutAssertf8AlmostEqual(1, "notAlmostEqual",
    request->notAlmostEqual.expected, request->notAlmostEqual.actual,
    request->notAlmostEqual.delta)

end
go