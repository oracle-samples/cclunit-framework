drop program cclut_parse_coverages:dba go
create program cclut_parse_coverages:dba
/**
 Parses all COVERAGE blocks for a given program from XML coverage data and returns them merged together in a single 
 XML coverage block. The consumer is expected to provide the cclutRequest and cclutReply structures.
*/

/**
  @request
  @field programName
    The name of the program for which code coverage data is to be parsed.
  @field coverageXml
    The code coverage XML from which the given program's data is to be parsed.

  record cclutRequest (
    1 programName = vc
    1 coverageXml = vc
  ) with protect
*/

/**
  @reply
  @field coverageXml
    The coverage XML for the given program name.

  record cclutReply (
    1 coverageXml = vc
  ) with protect
*/

declare public::main(null) = null with protect
declare public::parseCoverage(
    cclutProgramName = vc, cclutXml = vc, cclutStartPos = i4, cclutResumePos = i4(ref)) = vc  with protect

%i cclsource:cclut_xml_functions.inc


/**
  Identifies the first COVERAGE tag in a coverage XML report beyond a specified starting point.
 
  @param cclutProgramName
    The name of the program to be searched.  This value is case-insensitive.
  @param cclutXml
    The XML containing the coverage report data.
  @param cclutStartPos
    The position within the XML data to start searching.
  @param cclutResumePos
    A return buffer for the position within the XML of the end of the identified COVERAGE tag, 
    i.e., the position where a search for additional COVERAGE tags should resume.  
    If no COVERAGE tag is found, this will be set the to the length of the given XML string + 1.
  @returns
    The identified COVERAGE tag, if one is found.  Otherwise, a blank string is returned.
*/
subroutine public::parseCoverage(cclutProgramName, cclutXml, cclutStartPos, cclutResumePos)
  declare cclutCoverageXML     = vc with protect, noconstant("")
  declare cclutCoveragePos     = i4 with protect, noconstant(0)
  declare cclutFoundInd        = i2 with protect, noconstant(0)
  declare cclutCoverageName    = vc with protect, noconstant("")

  set cclutCoverageName = cclut::createXmlElement("COVERAGE_NAME", trim(cnvtupper(cclutProgramName), 3))
  set cclutCoveragePos = findstring(concat("<COVERAGE>", cclutCoverageName), cclutXml, cclutStartPos)
  if (cclutCoveragePos > 0)
    set cclutCoverageXML = cclut::retrieveXmlContent(cclutXml, "COVERAGE", cclutCoveragePos, cclutFoundInd)
    if (cclutFoundInd = 1)
      set cclutCoverageXML = cclut::createXmlElement("COVERAGE", cclutCoverageXML)
      set cclutResumePos = cclutCoveragePos + textlen(cclutCoverageXML)
      return (cclutCoverageXML)
    endif
  endif
  set cclutResumePos = textlen(cclutXml) + 1
  return ("")
end ;;;parseCoverage

/**
  The main subroutine.
*/
subroutine public::main(null)
  declare CCLUT_PARSE_LIMIT     = i4 with protect, constant(textlen(cclutRequest->coverageXml) + 1)
  declare cclutStartPos        = i4 with protect, noconstant(1)
  declare cclutResumePos       = i4 with protect, noconstant(0)
  declare cclutCurrentXml      = vc with protect, noconstant("")
  declare cclutFinalXml        = vc with protect, noconstant("")

  record cclut_mergeRequest (
    1 sourceXml = vc
    1 targetXml = vc
  ) with protect
  
  record cclut_mergeReply (
    1 mergedXml = vc
  ) with protect
  
  while (cclutStartPos < CCLUT_PARSE_LIMIT)
    set cclutCurrentXml = parseCoverage(cclutRequest->programName, cclutRequest->coverageXml, cclutStartPos, cclutResumePos)
    if (cclutResumePos <= CCLUT_PARSE_LIMIT)
      set cclut_mergeRequest->targetXml = cclutFinalXml
      set cclut_mergeRequest->sourceXml = cclutCurrentXml
      
      execute cclut_merge_cc with replace("CCLUTREQUEST", cclut_mergeRequest), REPLACE("CCLUTREPLY", cclut_mergeReply)
      set cclutFinalXml = cclut_mergeReply->mergedXml
    endif
    set cclutStartPos = cclutResumePos
  endwhile
  
  set cclutReply->coverageXml = trim(cclutFinalXml, 3)
end ;;;main

call main(null)

#exit_script

end go