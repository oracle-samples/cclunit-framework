drop program cclutGetFileAsStringTest go
create program cclutGetFileAsStringTest
%i cclsource:cclut_get_file_as_string.inc

/*
 * record request (
 *  1 filename = vc
 * )
 */

/*
 * reply (
 *  1 text = vc
 * )
 */

set reply->text = cclut::getFileAsString(request->filename)
 
end go