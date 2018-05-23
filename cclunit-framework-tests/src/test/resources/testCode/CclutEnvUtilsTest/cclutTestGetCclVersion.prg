drop program cclutTestGetCclVersion go
create program cclutTestGetCclVersion

%i cclsource:cclut_env_utils.inc

/*
 * reply (
 *  1 expected = vc
 *  1 actual = vc
 * )
 */

set reply->expected = concat(trim(cnvtstring(currev)), ".", 
                             trim(cnvtstring(currevminor)), ".", trim(cnvtstring(currevminor2)))

set reply->actual = cclut::getCCLVersion(null)

end go
