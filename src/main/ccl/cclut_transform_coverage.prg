drop program cclut_transform_coverage:dba go
create program cclut_transform_coverage:dba
/**
  Parses a CCL code coverage report and returns it in XML format.  The structure of the XML is
  <pre>
  &lt;COVERAGES&gt;
    &lt;COVERAGE&gt;
      &lt;COVERAGE_NAME&gt;program name&lt;/COVERAGE_NAME&gt;
      &lt;LINES&gt;
        &lt;LINE&gt;
          &lt;NBR&gt;###&lt;/NBR&gt;
          &lt;TYPE&gt;coverage type&lt;/TYPE&gt;
        &lt;/LINE&gt;
      &lt;/LINES&gt;
    &lt;/COVERAGE&gt;
    [... additional &lt;COVERAGE /&gt; tags ...]
  &lt;/COVERAGES&gt;
  </pre>
  The consumer is expected to provide the cclutRequest and cclutReply structures.
*/

/**
  @request
  @field path
    The directory in which the file containing the coverage data lives.
  @field filename
    The name of the code coverage data file to be read.

  record cclutRequest (
    1 path = vc
    1 filename = vc
  ) with protect
*/
 
/**
  @reply
  @field xml
    XML data representing the code coverage data file that was parsed.

  record cclutReply (
    1 xml = vc
  ) with protect
*/

/**
  Houses a list of collections of CCL coverage data.
  @field coverages
    A collection of coverage data.
    @field programName
      The name of the CCL program to which the coverage data pertains.
    @field lines
      A line of coverage data for the CCL program.
      @field lineNumber
        The line number of the line to which the coverage data pertains.
      @field type
        The type of the coverage
        @value C
          The line was covered.
        @value N
          The line is not executable.
        @value U
          The line was not covered.
*/
record coverageData (
  1 coverages[*]
    2 programName = vc
    2 lines[*]
      3 lineNumber = i4
      3 type = c1
) with protect


%i cclsource:cclut_xml_functions.inc


declare public::transformCoverage(cclutTarget = vc(ref), cclutCoverageIndex = i4, cclutRawCoverageData = vc) = null with protect
declare public::nextDelimitedItem(cclutSource = vc (ref), cclutDelimiter = vc) = vc with protect
declare public::readCoverageFile(cclutDirectory = vc, cclutFileName = vc, cclutPTarget = vc(ref)) = null with protect
declare public::generateCoverageXml(cclutSource = vc(ref)) = vc with protect
declare public::main(null) = null with protect


/**
  Parses a raw CCL coverage data item and stores it in a provided coverage data structure.

  @param cclutTarget
    The coverage data structure into which the data will be stored.
  @param cclutCoverageIndex
    The index within the coverageData list at which the coverage data will be added.
  @param cclutRawCoverageData
    The raw CCL coverage data to be parsed. The expected format is {coverage type}:[line number#count}[,line number#count]
    For example
    <pre>
      C:7#1,9#3,10#2,11,12#2
    </pre>
*/
subroutine public::transformCoverage(cclutTarget, cclutCoverageIndex, cclutRawCoverageData)
  declare coverageType    = c1 with protect, noconstant("")
  declare lineCoverage    = vc with protect, noconstant("")
  declare lineNumber      = vc with protect, noconstant("")
  declare cclutLineIndex  = i4 with protect, noconstant(0)
  declare cclutStat       = i4 with protect, noconstant(0)

  set cclutRawCoverageData = trim(cclutRawCoverageData, 3)
  set coverageType = substring(1, 1, cclutRawCoverageData)

  ;remove the coverage type and colon from the front of cclutRawCoverageData
  set cclutRawCoverageData = substring(3, textlen(cclutRawCoverageData), cclutRawCoverageData)
  set cclutLineIndex = size(cclutTarget->coverages[cclutCoverageIndex].lines, 5)
  
  while (textlen(trim(cclutRawCoverageData, 3)) > 0)
    set lineCoverage = nextDelimitedItem(cclutRawCoverageData, ",")
    set lineNumber = nextDelimitedItem(lineCoverage, "#")
    if (isnumeric(lineNumber))
      set cclutLineIndex = cclutLineIndex + 1
      set cclutStat = alterlist(cclutTarget->coverages[cclutCoverageIndex].lines, cclutLineIndex)
      set cclutTarget->coverages[cclutCoverageIndex].lines[cclutLineIndex].type = coverageType
      set cclutTarget->coverages[cclutCoverageIndex].lines[cclutLineIndex].lineNumber = cnvtint(lineNumber)
    endif
  endwhile 
end ;;;transformCoverage

/**
  Removes, trims and returns the first item from a delimited string. 
  @param cclutSource
    The delimited string. The first item will be removed from it.
  @param cclutDelimiter
    The delimiter.
  @returns
    The trimmed first delimited item from the provided string.
*/
subroutine public::nextDelimitedItem(cclutSource, cclutDelimiter)
  declare cclutItem = vc with protect, noconstant("")
  declare cclutDelimLen = i4 with protect, noconstant(textlen(cclutDelimiter))
  declare cclutDelimPos = i4 with protect, noconstant(0)
  declare cclutSourceLen = i4 with protect, noconstant(textlen(cclutSource))
  
  set cclutDelimPos = findstring(cclutDelimiter, cclutSource)
  if (cclutDelimPos > 0)
    set cclutItem = substring(1, cclutDelimPos - 1, cclutSource)
    set cclutSource = substring(cclutDelimPos + cclutDelimLen, 1 + cclutSourceLen - cclutDelimPos - cclutDelimLen, cclutSource)
  else
    set cclutItem = cclutSource
    set cclutSource = ""
  endif
  return (trim(cclutItem, 3))
end ;;;nextDelimitedItem

/**
  Reads the data from a coverage file into a target coverageData structure.
  @param cclutDirectory
    The directory containing the coverage data file.
  @param cclutFileName
    The name of the coverage data file.
  @param cclutPTarget
    The coverageData strcture into which the coverage data will be stored.
*/
subroutine public::readCoverageFile(cclutDirectory, cclutFileName, cclutPTarget)
  declare cclutFileLocation    = vc with protect, noconstant("")
  declare cclutCurrentLine     = vc with protect, noconstant("")
  declare cclutCoverageCount   = i4 with protect, noconstant(0)
  declare cclutRawCoverageData = vc with protect, noconstant("")
  declare cclutStat            = i4 with protect, noconstant(0)

  set cclutFileLocation = cnvtlower(build(cclutDirectory, cclutFileName))
  free define rtl2
  set logical file_location cclutFileLocation
  define rtl2 is "file_location"
   
  select into "nl:"
      r.line
  from rtl2t r
  detail
      cclutCurrentLine = trim(r.line, 3)
      if (cclutCurrentLine = patstring("COVER:: *"))
          cclutCurrentLine = substring(9, textlen(cclutCurrentLine), cclutCurrentLine)
          if (cclutCurrentLine = patstring("Prg(*"))
              cclutCoverageCount = cclutCoverageCount + 1
              cclutStat = alterlist(cclutPTarget->coverages, cclutCoverageCount)
              cclutCurrentLine = substring(5, textlen(cclutCurrentLine), cclutCurrentLine)
              cclutPTarget->coverages[cclutCoverageCount].programName = nextDelimitedItem(cclutCurrentLine, ")")
          else
              while (textlen(trim(cclutCurrentLine, 3)) > 0)
                  cclutRawCoverageData = nextDelimitedItem(cclutCurrentLine, " ")
                  if (textlen(cclutRawCoverageData) > 0)
                      call transformCoverage(cclutPTarget, cclutCoverageCount, cclutRawCoverageData)
                  endif
              endwhile
          endif
      endif
  with nocounter
end ;;;readCoverageFile

/**
  Generate coverage XML from a coverageData strucutre.
  @param cclutSource
    The coverage data strucutre
  @return 
    The coverage data in XML format.
*/
subroutine public::generateCoverageXml(cclutSource)
  declare cclutCoverageCount   = i4 with protect, noconstant(size(cclutSource->coverages, 5))
  declare cclutCoverageIndex   = i4 with protect, noconstant(0)
  declare cclutLineIndex       = i4 with protect, noconstant(0)
  declare cclutCurrentXml      = vc with protect, noconstant(trim(""))
  declare cclutCoverageXml     = vc with protect, noconstant(trim(""))
  declare cclutLineXml         = vc with protect, noconstant(trim(""))
  declare cclutNbrXml          = vc with protect, noconstant(trim(""))
  declare cclutTypeXml         = vc with protect, noconstant(trim(""))

  for (cclutCoverageIndex = 1 to cclutCoverageCount)
    set cclutCoverageXml = cclut::createXmlElement("COVERAGE_NAME", cclutSource->coverages[cclutCoverageIndex].programName)
    set cclutLineXml = trim("")
    for (cclutLineIndex = 1 to size(cclutSource->coverages[cclutCoverageIndex].lines, 5))
      set cclutNbrXml = 
          cclut::createXmlElement("NBR", trim(build(cclutSource->coverages[cclutCoverageIndex].lines[cclutLineIndex].lineNumber)))
      set cclutTypeXml = cclut::createXmlElement("TYPE", cclutSource->coverages[cclutCoverageIndex].lines[cclutLineIndex].type)
      set cclutLineXml = concat(cclutLineXml, cclut::createXmlElement("LINE", concat(cclutNbrXml, cclutTypeXml)))
    endfor
    set cclutLineXml = cclut::createXmlElement("LINES", cclutLineXml)
    set cclutCurrentXml = concat(cclutCurrentXml, cclut::createXmlElement("COVERAGE", concat(cclutCoverageXml, cclutLineXml)))
  endfor
  return (cclutCurrentXml)
end ;;;generateCoverageXml


/**
  main 
*/
subroutine public::main(null)
  call readCoverageFile(cclutRequest->path, cclutRequest->filename, coverageData)
  set cclutReply->xml = generateCoverageXml(coverageData)
end ;;;main

call main(null)


#exit_script

end go
