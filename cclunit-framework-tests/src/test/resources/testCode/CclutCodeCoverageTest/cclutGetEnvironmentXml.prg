drop program cclutGetEnvironmentXml go
create program cclutGetEnvironmentXml

%i cclsource:cclut_code_coverage.inc

/*
 * record reply (
 *  1   actual[1]
 *      2 currdb = vc
 *      2 currdbname = vc
 *      2 currdbuser = vc
 *      2 currdbsys = vc
 *      2 ccl_major_version = i4
 *      2 ccl_minor_version = i4
 *      2 ccl_revision = i4
 *      2 cursys = vc
 *      2 curlocale = vc
 *      2 curuser = vc
 *      2 curutc = i4
 *      2 curutcdiff = i4
 *      2 curtimezone = c30
 *      2 curtimezoneapp = i4
 *      2 curtimezonesys = vc
 *      2 currevafd = i4
 *      2 curgroup = i4
 *      2 dboptmode = vc
 *      2 dbversion = vc
 *  1   retrievedXml = vc
 * )
 */

set reply->actual.currdb = currdb
set reply->actual.currdbname = currdbname
set reply->actual.currdbuser = currdbuser
set reply->actual.currdbsys = currdbsys
set reply->actual.ccl_major_version = currev
set reply->actual.ccl_minor_version = currevminor
set reply->actual.ccl_revision = currevminor2
set reply->actual.cursys = cursys
set reply->actual.curlocale = curlocale
set reply->actual.curuser = curuser
set reply->actual.curutc = curutc
set reply->actual.curutcdiff = curutcdiff
set reply->actual.curtimezone = curtimezone
set reply->actual.curtimezoneapp = curtimezoneapp
set reply->actual.curtimezonesys = curtimezonesys
set reply->actual.currevafd = currevafd
set reply->actual.curgroup = curgroup
 
select into 'nl:'
     p.value
from v$parameter p
where p.name = 'optimizer_mode'
detail
    reply->actual.dboptmode = p.value
with nocounter
 
select into 'nl:'
    v.banner
from v$version v
where v.banner = 'Oracle Database*'
detail
    reply->actual.dbversion = v.banner
with nocounter

set reply->retrievedXml = cclut::getEnvironmentDataXml(null)

end go
