drop program cclutCompilePrgTest go
create program cclutCompilePrgTest
%i cclsource:cclut_compile_subs.inc

/*
  request (
   1 listingDirectory = vc
   1 listingName = vc
   1 prgDirectory = vc
   1 prgName = vc
 */

/*
  reply (
   1 compileResponse = i2
   1 errorMessage = vc
   1 programDirectory = vc
 */
declare errorMessage = vc
set reply->compileResponse = cclut::compileProgram(request->prgDirectory, request->prgName, 
    request->listingDirectory, request->listingName, errorMessage)
set reply->programDirectory = trim(cnvtlower(logical(request->prgDirectory)), 3)
set reply->errorMessage = errorMessage
end go