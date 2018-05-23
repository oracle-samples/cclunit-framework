drop program cclutCclErrorLineNumberTest go
create program cclutCclErrorLineNumberTest
%i cclsource:cclut_error_handling.inc

/*
 * request (
 *  1 error_msg = vc
 * )
 */
 
/*
 * reply (
 *  1 line_number = vc
 * )
 */
 
set reply->line_number = cclut::getCCLErrorLineNumber(request->error_msg)

end
go