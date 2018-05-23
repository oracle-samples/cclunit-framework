/**
  Unit Tests for cclut_parse_coverages
*/

declare testParseCoverage(null) = null with protect
declare testParseCoverages(null) = null with protect
declare testParseCoverage::lomain(null) = null with protect

declare stat = i4 with protect, noconstant(0)

declare coverageData = vc with protect, noconstant("")
set coverageData = concat(^<?xml version="1.0" encoding="UTF-8"?>^,
  "<COVERAGES>",
  "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>6</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>7</NBR><TYPE>N</TYPE></LINE>",
  "</LINES></COVERAGE>",

  "<COVERAGE><COVERAGE_NAME>PROG_2</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
  "</LINES></COVERAGE>",

  "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>7</NBR><TYPE>C</TYPE></LINE>",
  "</LINES></COVERAGE>",

  "<COVERAGE><COVERAGE_NAME>PROG_3</COVERAGE_NAME>",
  "<LINES>",
  "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
  "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>7</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>8</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>9</NBR><TYPE>C</TYPE></LINE>",
  "<LINE><NBR>10</NBR><TYPE>N</TYPE></LINE>",
  "<LINE><NBR>11</NBR><TYPE>N</TYPE></LINE>",
  "</LINES></COVERAGE>",

  "</COVERAGES>")


/**
  Validates the parseCoverage subroutine
*/
subroutine testParseCoverage(null)
  execute cclut_parse_coverages with replace("MAIN", lomain), curnamespace="testParseCoverage"
end 
subroutine testParseCoverage::lomain(null)
  declare resumePosition = i4 with protect, noconstant(0)
  declare programCoverage = vc with protect, noconstant("")

  set programCoverage = parseCoverage("prog_1", coverageData, 3, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_1 coverage 1", resumePosition, 396)
  set stat = cclutAssertVCEqual(CURREF, "prog_1 coverage 1", programCoverage, concat(
    "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
    "<LINES>",
    "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>3</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
    "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>6</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>7</NBR><TYPE>N</TYPE></LINE>",
    "</LINES></COVERAGE>"))
  
  set programCoverage = parseCoverage("prog_1", coverageData, 396, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_1 coverage 2", resumePosition, 1010)
  set stat = cclutAssertVCEqual(CURREF, "prog_1 coverage 2", programCoverage, concat(
    "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
    "<LINES>",
    "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>2</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>4</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>5</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>7</NBR><TYPE>C</TYPE></LINE>",
    "</LINES></COVERAGE>"))

  set programCoverage = parseCoverage("prog_1", coverageData, 1010, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_1 coverage 3", resumePosition, 1526)
  set stat = cclutAssertVCEqual(CURREF, "prog_1 coverage 3", programCoverage, "")

  set programCoverage = parseCoverage("prog_2", coverageData, 3, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_2 coverage 1", resumePosition, 664)
  set stat = cclutAssertVCEqual(CURREF, "prog_2 coverage 1", programCoverage, concat(
    "<COVERAGE><COVERAGE_NAME>PROG_2</COVERAGE_NAME>",
    "<LINES>",
    "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
    "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
    "</LINES></COVERAGE>"))

  set programCoverage = parseCoverage("prog_2", coverageData, 664, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_2 coverage 2", resumePosition, 1526)
  set stat = cclutAssertVCEqual(CURREF, "prog_2 coverage 2", programCoverage, "")

  set programCoverage = parseCoverage("prog_3", coverageData, 3, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_3 coverage 1", resumePosition, 1514)
  set stat = cclutAssertVCEqual(CURREF, "prog_3 coverage 1", programCoverage, concat(
    "<COVERAGE><COVERAGE_NAME>PROG_3</COVERAGE_NAME>",
    "<LINES>",
    "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
    "<LINE><NBR>5</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>6</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>7</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>8</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>9</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>10</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>11</NBR><TYPE>N</TYPE></LINE>",
    "</LINES></COVERAGE>"))

  set programCoverage = parseCoverage("prog_3", coverageData, 1514, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_3 coverage 2", resumePosition, 1526)
  set stat = cclutAssertVCEqual(CURREF, "prog_3 coverage 2", programCoverage, "")

  set programCoverage = parseCoverage("prog_4", coverageData, 3, resumePosition)
  set stat = cclutAssertI4Equal(CURREF, "end of prog_4 coverage 1", resumePosition, 1526)
  set stat = cclutAssertVCEqual(CURREF, "prog_4 coverage 1", programCoverage, "")
end ;;;testParseCoverage


subroutine testParseCoverages(null)
  record request (
    1 programName = vc
    1 coverageXml = vc
  ) with protect
    
  record reply (
    1 coverageXml = vc
  ) with protect

  set request->programName = "prog_1"
  set request->coverageXml = coverageData
  execute cclut_parse_coverages with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)
  
  set stat = cclutAssertVCEqual(CURREF, "prog_1 coverage", reply->coverageXml, concat(
    "<COVERAGE><COVERAGE_NAME>PROG_1</COVERAGE_NAME>",
    "<LINES>",
    "<LINE><NBR>1</NBR><TYPE>N</TYPE></LINE>",
    "<LINE><NBR>2</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>3</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>4</NBR><TYPE>U</TYPE></LINE>",
    "<LINE><NBR>5</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>6</NBR><TYPE>C</TYPE></LINE>",
    "<LINE><NBR>7</NBR><TYPE>C</TYPE></LINE>",
    "</LINES></COVERAGE>"))
    
end ;;;testParseCoverages

