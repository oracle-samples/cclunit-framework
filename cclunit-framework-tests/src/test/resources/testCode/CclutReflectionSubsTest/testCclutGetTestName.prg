drop program testCclutGetTestName go
create program testCclutGetTestName

/*
 * This test confirms that cclut::getTestName(null) returns the current value of cclut::UnitTestName.
 */

declare cclut::testName = vc with protect, noconstant("i.am.a.test.name")

%i cclsource:cclut_reflection_subs.inc

/*
 * reply (
 *  1 testName = vc
 * )
 */

set reply->testName = cclut::getTestName(null)

end go
