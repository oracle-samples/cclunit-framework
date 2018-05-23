drop program cclutTestIsCclVersionLessThan go
create program cclutTestIsCclVersionLessThan

%i cclsource:cclut_env_utils.inc

/*
 * request (
 *  1 lhs = vc
 *  1 rhs = vc
 * )
 */

/*
 * reply (
 *  1 isLessThan = i2
 * )
 */

set reply->isLessThan = cclut::compareCclVersion(request->lhs, request->rhs)

end go
