drop program cclutCCGetTestCaseListing go
create program cclutCCGetTestCaseListing

%i cclsource:cclut_code_coverage.inc

/*
 * record dt_request (
 *     1 program_name = vc
 *     1 path = vc
 *     1 filename = vc
 * )
 */

set dt_reply->xml = cclut::getTestCaseListingXml(dt_request->program_name, dt_request->path, dt_request->filename)

end go