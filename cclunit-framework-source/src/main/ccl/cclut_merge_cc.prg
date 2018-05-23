drop program cclut_merge_cc:dba go
create program cclut_merge_cc:dba
/**
  Generates a code coverage report for a program by merging the data for covered lines from a source report into a target 
  report. A line will be covered in the final report if it is covered in either the source or target report. The coverage 
  type for all other lines will be the coverage type from the target report. It is expected that both the source and target 
  reports have the same format and provide coverage for all lines of the program in sequential order except that either of 
  the source or target report can be empty in which case the other report is returned. 
  The consumer is expected to provide the cclutRequest and cclutReply structures.
*/


/**
  @request
  @field sourceXml
    The XML to be merged into the given original XML.
  @field targetXml
    The original XML to be merged into.

  record cclutRequest (
    1 sourceXml = vc
    1 targetXml = vc
  ) with protect
*/

/**
  @reply
  @field mergedXml
    The two XML documents, merged into one.

  record cclutReply (
    1 mergedXml = vc
  ) with protect
*/


%i cclsource:cclut_constants.inc
%i cclsource:cclut_xml_functions.inc


declare CCLUT_TYPE_START      = vc with protect, constant("<TYPE>")
declare CCLUT_TYPE_END        = vc with protect, constant("</TYPE>")

declare cclutLineNumber      = i4 with protect, noconstant(1)

declare cclutPreCoverFrag    = vc with protect, noconstant("")
declare cclutPostCoverFrag   = vc with protect, noconstant("")
declare cclutPostCoverPos    = i4 with protect, noconstant(0)
declare cclutNbrTag          = vc with protect, noconstant("")

declare cclutNbrStartPos     = i4 with protect, noconstant(0)
declare cclutTypeStartPos    = i4 with protect, noconstant(0)
declare cclutTypeEndPos      = i4 with protect, noconstant(0)

declare cclutType            = c1 with protect, noconstant("")
declare cclutFinalXml        = vc with protect, noconstant("")
declare cclutTypeTag         = vc with protect, noconstant("")

if (textlen(trim(cclutRequest->targetXml, 3)) = 0)
  set cclutReply->mergedXml = cclutRequest->sourceXml
  go to exit_script
elseif (textlen(trim(cclutRequest->sourceXml, 3)) = 0)
  set cclutReply->mergedXml = cclutRequest->targetXml
  go to exit_script
endif

set cclutPreCoverFrag = substring(1, findstring("<LINES>", cclutRequest->targetXml) + 6, cclutRequest->targetXml)
set cclutPostCoverPos = findstring("</LINES>", cclutRequest->targetXml, 1, 1)
set cclutPostCoverFrag = substring(cclutPostCoverPos, textlen(cclutRequest->targetXml), cclutRequest->targetXml)

set cclutNbrTag = cclut::createXmlElement("NBR", "1")
set cclutNbrStartPos = findstring(cclutNbrTag, cclutRequest->targetXml, cclutNbrStartPos)

while (cclutNbrStartPos > 0)
  set cclutTypeStartPos = findstring(CCLUT_TYPE_START, cclutRequest->targetXml, cclutNbrStartPos) + 6
  if (cclutTypeStartPos > 6)
    set cclutTypeEndPos = findstring(CCLUT_TYPE_END, cclutRequest->targetXml, cclutTypeStartPos)
    set cclutType = substring(cclutTypeStartPos, (cclutTypeEndPos - cclutTypeStartPos), cclutRequest->sourceXml)
    if (cclutType != CCLUT_COVERED)
        set cclutType = substring(cclutTypeStartPos, (cclutTypeEndPos - cclutTypeStartPos), cclutRequest->targetXml)
    endif
    set cclutTypeTag = cclut::createXmlElement("TYPE", cclutType)
    set cclutFinalXml = concat(cclutFinalXml, cclut::createXmlElement("LINE", concat(cclutNbrTag, cclutTypeTag)))
  endif 
  set cclutLineNumber = cclutLineNumber + 1
  set cclutNbrTag = cclut::createXmlElement("NBR", trim(build(cclutLineNumber)))
  set cclutNbrStartPos = findstring(cclutNbrTag, cclutRequest->targetXml, cclutNbrStartPos)
endwhile

set cclutReply->mergedXml = concat(cclutPreCoverFrag, trim(cclutFinalXml, 3), cclutPostCoverFrag)

#exit_script

end go
