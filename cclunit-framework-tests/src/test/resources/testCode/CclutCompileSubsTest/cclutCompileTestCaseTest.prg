drop program cclutCompileTestCaseTest go
create program cclutCompileTestCaseTest
%i cclsource:cclut_compile_subs.inc

/*
  request (
   1 incDirectory = vc
   1 incName = vc
   1 listingDirectory = vc
   1 listingName = vc
   1 prgName = vc
 */

/*
  reply (
   1 compileResponse = i2
   1 errorMessage = vc
   1 testCaseDirectory = vc
 */

declare errorMessage = vc with protect, noconstant("")
set reply->compileResponse = cclut::generateTestCaseProgram(request->incDirectory, request->incName,
request->listingDirectory, request->listingName, request->prgName, errorMessage)
set reply->errorMessage = errorMessage
set reply->testCaseDirectory = trim(cnvtlower(logical(request->incDirectory)), 3)
end go
