drop program cclutCCGetTestCaseCoverage go
create program cclutCCGetTestCaseCoverage

/*
 * record dt_request (
 *     1 coverage_xml = vc
 *     1 program_name = vc
 *     1 listing = vc
 * )
 */

%i cclsource:cclut_code_coverage.inc


set dt_reply->xml = cclut::getTestCaseCoverageXml(dt_request->coverage_xml, dt_request->program_name, dt_request->listing)

end go