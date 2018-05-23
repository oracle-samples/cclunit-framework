drop program cclutTestGetCoverage go
create program cclutTestGetCoverage
%i cclsource:cclut_code_coverage.inc
%i cclsource:cclut_compile_subs.inc

declare dt_prgLoc = vc with protect, noconstant("")
declare dt_prgDir = vc with protect, noconstant("")
declare dt_prgName = vc with protect, noconstant("")
declare dt_listLoc = vc with protect, noconstant("")
declare dt_listDir = vc with protect, noconstant("")
declare dt_listName = vc with protect, noconstant("")
declare errmsg = vc with protect, noconstant("")
declare dummyvar = i2 with noconstant, protect

/*
 * reply (
 *      1 xml = vc
 * )
 */

set dt_prgDir = concat(trim(logical("CCLSOURCE"),3), "/")
set dt_prgDir = cnvtlower(trim(dt_prgDir,3))
set dt_listDir = concat(trim(logical("CCLSOURCE"),3),"/")
set dt_listDir = cnvtlower(trim(dt_listDir,3))

;Make sure the all files are in lowercase
set dt_prgName = "testprg.prg"
set dt_Name = "testprg"
set dt_listName = "testprg.lis"

;Make sure the PRG file exist in specified location
set dt_prgLoc = concat(dt_prgDir, dt_prgName)
if (not findfile(trim(dt_prgLoc,3)))
    set reply->xml = concat("Could not find program at specified location [", trim(dt_prgLoc,3), "]")
    go to EXIT_TEST
endif

set dt_listLoc = cnvtlower(concat(dt_listDir, dt_listName))

set trace codecover 5

set COMPILE = DEBUG

call compile(dt_prgLoc, dt_listName, 1)

set COMPILE = NODEBUG

if(error(errmsg,0) > 0)
    set reply->xml = concat("HERE->" ,errmsg)
    go to EXIT_TEST
endif

call parser(concat("execute", dt_Name, "go"))

if(error(errmsg,0) > 0)
    set reply->xml = errmsg
    go to EXIT_TEST
endif

set reply->xml = "TESTING"

set reply->xml = cclut::getCoverageXml(0)

#EXIT_TEST

end go