drop program cclutCCGetListing go
create program cclutCCGetListing

/*
 * record dt_request (
 *     1 program_name = vc
 *     1 path = vc
 *     1 filename = vc
 * )
 */
 
%i cclsource:cclut_code_coverage.inc


set dt_reply->xml = cclut::getListingXml(dt_request->program_name, dt_request->path, dt_request->filename)


#EXIT_TEST

end go