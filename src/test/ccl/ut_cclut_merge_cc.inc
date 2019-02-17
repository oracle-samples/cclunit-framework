/**
  Unit Tests for cclut_merge_cc
*/

declare testMergeSomeToNone(null) = null with protect
declare testMergeNoneToSome(null) = null with protect
declare testMergeSomeToSome(null) = null with protect


declare stat = i4 with protect, noconstant(0)

declare coverageData1 = vc with protect, noconstant("")
set coverageData1 = concat(
  "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>6</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>7</NBR><TYPE>N</TYPE></LINE>",
  "</LINES></COVERAGE>")

declare coverageData2 = vc with protect, noconstant("")
set coverageData2 = concat(
  "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>7</NBR><TYPE>U</TYPE></LINE>",
  "</LINES></COVERAGE>")

declare coverageDataCombined = vc with protect, noconstant("")
set coverageDataCombined = concat(
  "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>6</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>7</NBR><TYPE>U</TYPE></LINE>",
  "</LINES></COVERAGE>")

record request (
  1 sourceXml = vc
  1 targetXml = vc
) with protect

record reply (
  1 mergedXml = vc
) with protect  

subroutine testMergeSomeToNone(null)
  set stat = initrec(request)
  set request->sourceXml = coverageData1
  set reply->mergedXml = "not empty"
  execute cclut_merge_cc with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)
  set stat = cclutAssertVCEqual(CURREF, "some.to.none", reply->mergedXml, coverageData1)
end ;;;mergeSomeToNone

subroutine testMergeNoneToSome(null)
  set stat = initrec(request)
  set request->targetXml = coverageData2
  set reply->mergedXml = "not empty"
  execute cclut_merge_cc with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)
  set stat = cclutAssertVCEqual(CURREF, "none.to.some", reply->mergedXml, coverageData2)
end ;;;mergeNoneToSome

subroutine testMergeSomeToSome(null)
  set stat = initrec(request)
  set request->sourceXml = coverageData1
  set request->targetXml = coverageData2
  set reply->mergedXml = "not empty"
  execute cclut_merge_cc with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)
  set stat = cclutAssertVCEqual(CURREF, "some.to.some", reply->mergedXml, coverageDataCombined)
end ;;;mergeSomeToSome
