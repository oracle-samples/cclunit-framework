drop program cclutTestXMLGetNoCoverage go
create program cclutTestXMLGetNoCoverage
%i cclsource:cclut_code_coverage.inc

/*
 * reply (
 *      1 xml = vc
 * )
 */
 
declare dummyvar = i2 with noconstant, protect

set reply->xml = "TESTING"

set reply->xml = cclut::getCoverageXml(dummyvar)

end go