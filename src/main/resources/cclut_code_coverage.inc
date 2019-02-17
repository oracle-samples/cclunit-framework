%#ifndef CCLUT_CODE_COVERAGE_INC
%#def CCLUT_CODE_COVERAGE_INC

%i cclsource:cclut_constants.inc
;**********************************************************************************************************************************
;
; Contains subroutines for obtaining code coverage information
;
;**********************************************************************************************************************************

declare cclut::getCoverageXml(null) = vc
declare cclut::filterCoverageXml(cclutCoverageXml = vc, cclutProgramList = vc(ref)) = null
declare cclut::getEnvironmentDataXml(null) = vc
declare cclut::getListingXml(cclutProgramName = vc, cclutListingDirectory = vc, cclutListingFileName = vc) = vc
declare cclut::getTestCaseListingXml(cclutTestCaseProgramName = vc, cclutListingDirectory = vc, cclutListingFileName = vc) = vc
declare cclut::getTestCaseCoverageXml(cclutCoverageXml = vc, cclutTestCaseProgramName = vc, cclutTestCaseListingXml = vc) = vc
declare cclut::getCoverageExclusions(cclutListingXml = vc, cclutExclusions = vc(ref)) = null
declare cclut::getNextCoverageToggleLineNumber(
    cclutListingXml = vc, cclutSearchStart = i4, cclutToggleType = vc, cclutNextStart = i4(ref)) = i4
declare cclut::excludeCoverage(cclutCoverageXml = vc, cclutExclusions = vc(ref)) = vc

/**
  Obtains the coverage information of all coverage in CCLs memory bank for the current run. 
  Outputs to a file in $cer_temp as well as the return value.

  @returns
    The coverage as an XML string, or "" if not successful
*/    
subroutine cclut::getCoverageXml(null)
  declare cclutStat         = i4 with protect, noconstant(0)
  declare cclutGciPath      = vc with protect, constant(build(trim(logical("cer_temp")), "/"))
  declare cclutGciFilename  = vc with protect, constant(build("cclut_cvg_", currdbhandle, ".out"))
  declare cclutErrorMsg     = vc with protect, noconstant("")

  if (curimage = "CCL")
    set trace codecover 5; all lines
  endif
   
  ;Record to hold file descriptor information
  record frec (
    1 file_desc   = i4
    1 file_offset = i4
    1 file_dir    = i4
    1 file_name   = vc
    1 file_buf    = vc
  ) with protect
 
  record tcRequest (
    1 path = vc
    1 filename = vc
  ) with protect
 
  record tcReply (
    1 xml = vc
  ) with protect
 
  ;Initialize the return variable
  set frec->file_name = build(cclutGciPath, cclutGciFilename)
  set frec->file_buf  = "w+" ;Open for read/write, create new file
 
  set cclutStat = cclio("OPEN", frec)
  if (error(cclutErrorMsg, 1) > 0)
    return ("Error during open")
  endif
 
  set cclutStat = cclio("CODECOVER", frec)  ;write codecover info to user file
  if (error(cclutErrorMsg, 1) > 0)
    return ("Error during codecover")
  endif
 
  set cclutStat = cclio("CLOSE", frec)
  if (error(cclutErrorMsg, 1) > 0)
    return ("Error during close")
  endif
 
  set tcRequest->path = cclutGciPath
  set tcRequest->filename = cclutGciFilename
 
  call cclut::exitOnError("pre-xform-coverage", "cclut_parse_source", cclutReply)
  execute cclut_transform_coverage with replace("CCLUTREQUEST", tcRequest), replace("CCLUTREPLY", tcReply)
 
  if (error(cclutErrorMsg, 1) > 0)
    return ("Error during cclut_transform_coverage")
  endif
  set cclutStat = remove(frec->file_name)

  if (curimage = "CCL")
    set trace codecover 0; summary  
  endif
  return (tcReply->xml)
end ;;;getCoverageXml
 
 
 
/**
  Extracts the XML coverage data for a specified list of programs from the full XML coverage data for a test case execution.
  @param cclutCoverageXml 
    The full coverage data for the test case execution in XML format.
  @param cclutProgramList 
    A record structure specifing the list of programs for which coverage should be extracted along with their listings
    and into which the extracted coverage data will be placed.
    <pre>
    record cclutProgramList (
      1 programs[*]
        2 programName = vc
        2 listingXml = vc
        2 coverageXml = vc
    ) with protect
    </pre>
*/
subroutine cclut::filterCoverageXml(cclutCoverageXml, cclutProgramList)
  declare cclutXmlIndex = i4 with protect, noconstant(0)
 
  record pcRequest (
    1 programName = vc
    1 coverageXml = vc
  ) with protect
 
  record pcReply (
    1 coverageXml = vc
  ) with protect
  
  record coverageExclusions (
    1 block[*]
      2 startLine = i4
      2 endLine = i4
  ) with protect
 
  set pcRequest->coverageXml = cclutCoverageXml
 
  for (cclutXmlIndex = 1 to size(cclutProgramList->programs, 5))
    ;filter each program out of the coverage XML and place that programs coverage in the cclutProgramList->programs[].coverageXml
    set pcRequest->programName = cclutProgramList->programs[cclutXmlIndex].programName
    call cclut::exitOnError("pre-parse-coverages", "cclut_parse_source", cclutReply)
    execute cclut_parse_coverages with replace("CCLUTREQUEST", pcRequest), replace("CCLUTREPLY", pcReply)
    if (textlen(trim(pcReply->coverageXml, 3)) > 0)
      call cclut::getCoverageExclusions(cclutProgramList->programs[cclutXmlIndex].listingXml, coverageExclusions)
      set cclutProgramList->programs[cclutXmlIndex].coverageXml = cclut::excludeCoverage(pcReply->coverageXml, coverageExclusions)
    else
      set cclutProgramList->programs[cclutXmlIndex].coverageXml =
        concat("<COVERAGE><COVERAGE_NAME>", cclutProgramList->programs[cclutXmlIndex].programName, "</COVERAGE_NAME></COVERAGE>")
    endif
  endfor ;cclutXmlIndex
end ;;;filterCoverageXML
 
 
/**
  Returns the environment data in XML format
  
  @returns 
    An XML string containing information about the current environment.
*/
subroutine cclut::getEnvironmentDataXml(null)
  declare cclutEnvXml = vc with protect, noconstant(trim(""))
  
  set cclutEnvXml = concat(cclutEnvXml, "<ENVIRONMENT>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURNODE><![CDATA[", build(curnode), "]]></CURNODE>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURDOMAIN><![CDATA[", build(curdomain), "]]></CURDOMAIN>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURRDB><![CDATA[", build(currdb), "]]></CURRDB>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURRDBNAME><![CDATA[", build(currdbname), "]]></CURRDBNAME>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURRDBUSER><![CDATA[", build(currdbuser), "]]></CURRDBUSER>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURRDBSYS><![CDATA[", build(currdbsys), "]]></CURRDBSYS>")
  set cclutEnvXml = concat(cclutEnvXml, "<CCLVER><![CDATA[", build(currev, ".", currevminor, ".", currevminor2), "]]></CCLVER>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURSYS><![CDATA[", build(cursys), "]]></CURSYS>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURSYS2><![CDATA[", build(cursys), "]]></CURSYS2>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURLOCALE><![CDATA[", build(curlocale), "]]></CURLOCALE>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURUSER><![CDATA[", build(curuser), "]]></CURUSER>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURUTC><![CDATA[", build(curutc), "]]></CURUTC>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURUTCDIFF><![CDATA[", build(curutcdiff), "]]></CURUTCDIFF>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURTIMEZONE><![CDATA[", curtimezone, "]]></CURTIMEZONE>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURTIMEZONEAPP><![CDATA[", build(curtimezoneapp), "]]></CURTIMEZONEAPP>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURTIMEZONESYS><![CDATA[", build(curtimezonesys), "]]></CURTIMEZONESYS>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURREVAFD><![CDATA[", build(currevafd), "]]></CURREVAFD>")
  set cclutEnvXml = concat(cclutEnvXml, "<CURGROUP><![CDATA[", build(curgroup), "]]></CURGROUP>")
 
  ;Get database version
  select into "nl:"
      v.banner
  from v$version v
  where v.banner = "Oracle Database*"
  detail
      cclutEnvXml = concat(cclutEnvXml, "<DBVERSION><![CDATA[", build(v.banner), "]]></DBVERSION>")
  with nocounter
 
  ;Get database optimizer mode
  select into "nl:"
      p.value
  from v$parameter p
  where name = "optimizer_mode"
  detail
      cclutEnvXml = concat(cclutEnvXml, "<DBOPTMODE><![CDATA[", build(p.value), "]]></DBOPTMODE>")
  with nocounter
 
  ;Get current username
  select into "nl:"
      p.username
  from prsnl p
  where p.person_id = reqinfo->updt_id
  detail
      cclutEnvXml = concat(cclutEnvXml, "<CCLUSER><![CDATA[", build(p.username), "]]></CCLUSER>")
  with nocounter

  set cclutEnvXml = concat(cclutEnvXml, 
      "<FRAMEWORK_VERSION><![CDATA[", cclut::FRAMEWORK_VERSION, "]]></FRAMEWORK_VERSION>",
      "<REQUIRED_CCL><![CDATA[", cclut::MINIMUM_REQUIRED_CCL_VERSION, "]]></REQUIRED_CCL>")
  set cclutEnvXml = concat(cclutEnvXml, "</ENVIRONMENT>")
  return (cclutEnvXml)
end ;;;getEnvironmentDataXML
 
 
 
/**
  Construct the XML listing for a specified program from the compile listing for the program.

  @param cclutProgramName 
    The name of the program.
  @param cclutListingDirectory 
    The directory in which the program's compile listing is stored.
  @param cclutListingFileName 
    The name of the program's listing file.
  @returns
    The compile listing for the specified program in XML format.
*/
subroutine cclut::getListingXml(cclutProgramName, cclutListingDirectory, cclutListingFileName)
  record listingRequest (
    1 programName = vc
    1 path = vc
    1 filename = vc
  ) with protect
 
  record listingReply (
    1 programName = vc  /* cclut_transform_source request member */
    1 compileDate = vc  /* cclut_transform_source request member */
    1 source[*]
      2 line = vc
  ) with protect
 
  record transformReply (
    1 xml = vc
  ) with protect
 
  set listingRequest->programName = trim(cnvtupper(cclutProgramName), 3)
  set listingRequest->path = cclutListingDirectory
  set listingRequest->filename = cclutListingFileName

  call cclut::exitOnError("pre-parse-source", "cclut_parse_source", cclutReply)
  execute cclut_parse_source with replace("CCLUTREQUEST", listingRequest), replace("CCLUTREPLY", listingReply)
 
  set listingReply->programName = listingRequest->programName
  set listingReply->compileDate = format(cnvtdatetime(curdate, curtime3), ";;q")
 
  call cclut::exitOnError("pre-xform-source", "cclut_transform_source", cclutReply)
  execute cclut_transform_source with replace("CCLUTREQUEST", listingReply), replace("CCLUTREPLY", transformReply)
 
  return (transformReply->xml)
end ;;;getListingXML
 
 
/**
  Retrieves the listing in XML format for a test case file within the listing for its test case program. 

  @param cclutTestCaseProgramName 
    The name of the compiled test case program.
  @param cclutListingDirectory 
    The directory where the listing is stored.
  @param cclutListingFileName 
    The name of the listing file.
  @returns
    The listing information for the test case in XML format.
*/

subroutine cclut::getTestCaseListingXml(cclutTestCaseProgramName, cclutListingDirectory, cclutListingFileName)
  declare cclutTestCaseProgramListingXml = vc with protect, noconstant("")
  declare cclutListingXML = vc with protect, noconstant("")
  declare cclutStartMarkerPosition = i4 with protect, noconstant(0)
  declare cclutEndMarkerPosition = i4 with protect, noconstant(0)
  declare cclutTestCaseStartPosition = i4 with protect, noconstant(0)
  declare cclutTestCaseEndPosition = i4 with protect, noconstant(0)
  declare cclutHeadXmlEndPosition = i4 with protect, noconstant(0)
  declare cclutHeadXml = vc with protect, noconstant("")

  set cclutTestCaseProgramListingXml = cclut::getListingXml(cclutTestCaseProgramName, cclutListingDirectory, cclutListingFileName)
  set cclutHeadXmlEndPosition = findstring("</COMPILE_DATE>", cclutTestCaseProgramListingXml, 1, 0) + 14
  set cclutHeadXml = substring(1, cclutHeadXmlEndPosition, cclutTestCaseProgramListingXml)
 
  set cclutStartMarkerPosition = findstring(";;;CCLUT_START_TEST_INC_FILE", cclutTestCaseProgramListingXml, 1, 0)
  set cclutEndMarkerPosition = findstring(";;;CCLUT_END_TEST_INC_FILE", cclutTestCaseProgramListingXml, 1, 1)
 
  set cclutTestCaseStartPosition = findstring("<LINE>", cclutTestCaseProgramListingXml, cclutStartMarkerPosition, 0)
  set cclutListingXML = 
      substring(cclutTestCaseStartPosition, cclutEndMarkerPosition-cclutTestCaseStartPosition+1, cclutTestCaseProgramListingXml)
  set cclutTestCaseEndPosition = findstring("</LINE>", cclutListingXML, 1, 1) + 6
  set cclutListingXML = substring(1, cclutTestCaseEndPosition, cclutListingXML)
  set cclutListingXML = concat(cclutHeadXml, "<LINES>", cclutListingXML, "</LINES></LISTING>")
 
  return (cclutListingXML)
end ;;;getTestCaseListingXml
 
 
 
/**
  Retrieves the coverage data of a specified test case file.
  Note that getTestCaseListingXml should be called first to ensure the test case listing data is available.

  @param coverageXml 
    The coverage data for all scripts called during test execution in XML format.
  @param cclutTestCaseProgramName 
    The name of the compiled test case program.
  @param cclutTestCaseListingXml 
    The listing data for the test case in XML format.

  @returns
    The coverage data for the test case file.
*/
subroutine cclut::getTestCaseCoverageXml(cclutCoverageXml, cclutTestCaseProgramName, cclutTestCaseListingXml)
  declare cclutTestCaseCoverageXml = vc with protect, noconstant("")
  declare cclutTestCaseProgramCoverage = vc with protect, noconstant("")
  declare cclutHeadXml = vc with protect, noconstant("")
  declare cclutHeadXmlEndPosition = i4 with protect, noconstant(0)
  declare cclutStartingLineNumber = i4 with protect, noconstant(0)
  declare cclutEndingLineNumber = i4 with protect, noconstant(0)
  declare cclutCoverageStartPosition = i4 with protect, noconstant(0)
  declare cclutCoverageEndPosition = i4 with protect, noconstant(0)
  declare cclutLineNumberPosition = i4 with protect, noconstant(0)
  declare cclutLineNumberLength = i4 with protect, noconstant(0)

  record coverageFilter
  (
    1 programs[*]
      2 programName = vc
      2 coverageXml = vc
      2 listingXml = vc
  ) with protect

  set cclutStat = alterlist(coverageFilter->programs, 1)
  set coverageFilter->programs[1].programName = cclutTestCaseProgramName
  set coverageFilter->programs[1].listingXml = cclutTestCaseListingXml
  call cclut::filterCoverageXml(cclutCoverageXml, coverageFilter)
  set cclutTestCaseProgramCoverage = coverageFilter->programs[1].coverageXml

  set cclutHeadXmlEndPosition = findstring("</COVERAGE_NAME>", cclutTestCaseProgramCoverage, 1, 0) + 15
  set cclutHeadXml = substring(1, cclutHeadXmlEndPosition, cclutTestCaseProgramCoverage)
  set cclutLineNumberPosition = findstring("<LINE><NBR>", cclutTestCaseListingXml, 1, 0) + 10
  if (cclutLineNumberPosition > 10)
    set cclutLineNumberLength = 
        findstring("</NBR>", cclutTestCaseListingXml, cclutLineNumberPosition, 0) - cclutLineNumberPosition - 1
    set cclutStartingLineNumber = cnvtint(substring(cclutLineNumberPosition+1, cclutLineNumberLength, cclutTestCaseListingXml))
    set cclutCoverageStartPosition = findstring(build("<LINE><NBR>", cclutStartingLineNumber), cclutTestCaseProgramCoverage, 1, 0)
    if (cclutCoverageStartPosition > 0)
   
      set cclutLineNumberPosition = findstring("<LINE><NBR>", cclutTestCaseListingXml, 1, 1) + 10
      set cclutLineNumberLength = 
          findstring("</NBR>", cclutTestCaseListingXml, cclutLineNumberPosition, 0) - cclutLineNumberPosition - 1
      set cclutEndingLineNumber = cnvtint(substring(cclutLineNumberPosition+1, cclutLineNumberLength, cclutTestCaseListingXml))
    
      set cclutLineNumberPosition = findstring(build("<LINE><NBR>", cclutEndingLineNumber), cclutTestCaseProgramCoverage, 1, 0)
      set cclutCoverageEndPosition = findstring("</LINE>", cclutTestCaseProgramCoverage, cclutLineNumberPosition, 0) + 6
      set cclutTestCaseCoverageXml = 
          substring(cclutCoverageStartPosition, cclutCoverageEndPosition-cclutCoverageStartPosition+1, cclutTestCaseProgramCoverage)
    endif
  endif
  set cclutTestCaseCoverageXml = concat(cclutHeadXml, "<LINES>", trim(cclutTestCaseCoverageXml, 3), "</LINES></COVERAGE>")
 
  return (cclutTestCaseCoverageXml)
end ;;;getTestCaseCoverageXml

/**
  Retrieves the code coverage exclusions for a CCL program.
  
  @param cclutListingXml
    The listing xml for the program
  @param cclutExclusions 
    A return buffer for the exclusions 
*/
subroutine cclut::getCoverageExclusions(cclutListingXml, cclutExclusions)
  declare startPosition = i4 with protect, noconstant(0)
  declare nextStartPosition = i4 with protect, noconstant(1)
  declare startToggleLine = i4 with protect, noconstant(0)
  declare endToggleLine = i4 with protect, noconstant(0)
  declare exclusionCount = i4 with protect, noconstant(0)
  declare stat = i4 with protect, noconstant(0)

  set stat = initRec(cclutExclusions)  
  while (nextStartPosition > 0)
    set startPosition = nextStartPosition
    set nextStartPosition = 0
    set startToggleLine = cclut::getNextCoverageToggleLineNumber(cclutListingXml, startPosition, "OFF", nextStartPosition)
    if (startToggleLine > 0)
      set startPosition = nextStartPosition
      set nextStartPosition = 0
      set endToggleLine = cclut::getNextCoverageToggleLineNumber(cclutListingXml, startPosition, "ON", nextStartPosition)
      if (endToggleLine > 0)
        set exclusionCount = exclusionCount + 1
        set stat = alterlist(cclutExclusions->block, exclusionCount)
        set cclutExclusions->block[exclusionCount].startLine = startToggleLine
        set cclutExclusions->block[exclusionCount].endLine = endToggleLine
      endif
    endif
  endwhile
end ;;;getCoverageExclusions

/**
  Identifies the line number of the next code coverage exclusion toggle for a CCL program 
  and the location to begin seaching for any subsequent ones.
  
  @param cclutListingXml
    The listing xml for the program
  @param cclutSearchStart
    The position at which to start searching for the next code coverage exclusion toggle within the listing.
  @param cclutToggleType
    The type of toggle to search for.
    @value OFF
      A toggle that turns code coverage off.
    @value ON
      A toggle that turns code coverage on.
  @param cclutNextStart
    The position from which to start seaching for subsequent code coverage exclusion toggles within the listing.
*/
subroutine cclut::getNextCoverageToggleLineNumber(cclutListingXml, cclutSearchStart, cclutToggleType, cclutNextStart)
  declare position = i4 with protect, noconstant(0)
  declare toggleLine = i4 with protect, noconstant(0)
  declare toggleLineStart = i4 with protect, noconstant(0)
  declare toggleLineEnd = i4 with protect, noconstant(0)
  declare toggleString = vc with protect, constant(concat("<TEXT><![CDATA[;;;CCLUNIT:", cclutToggleType, "]]></TEXT>"))

  set position = findstring(toggleString, cclutListingXml, cclutSearchStart)
  if (position > 0)
    set position = findstring("<LINE>", substring(1, position, cclutListingXml), 1, TRUE)
    if (position > 0)
      set cclutNextStart = position + 6
      set toggleLineStart = findstring("<NBR>", cclutListingXml, position) + 5
      if (toggleLineStart > 0)
        set toggleLineEnd = findstring("</NBR>", cclutListingXml, toggleLineStart)
        if (toggleLineEnd > 0)
          set toggleLine = cnvtint(substring(toggleLineStart, toggleLineEnd-toggleLineStart, cclutListingXml))
        endif
      endif
    endif
  endif
  return (toggleLine)
end ;;;getNextCoverageToggleLineNumber

/**
  Excludes indicated lines from coverageXml data by marking them as non-executable.
  @param cclutCoverageXml
    The coverage xml data to update
  @param cclutExclusions
    The lines of code that should be excluded.
  @returns
    The updated coverage xml data.
*/
subroutine cclut::excludeCoverage(cclutCoverageXml, cclutExclusions)
  declare blockIndex = i4 with protect, noconstant(0)
  declare blockCount = i4 with protect, noconstant(0)
  declare lineNumber = i4 with protect, noconstant(0)
  declare lineNumberString = vc with protect, noconstant("")
  declare lineNumberPosition = i4 with protect, noconstant(1)
  declare typePosition = i4 with protect, noconstant(0)
  declare coverageLen = i4 with protect, constant(textlen(cclutCoverageXml))

  set blockCount = size(cclutExclusions->block, 5)
  for (blockIndex = 1 to blockCount)
    for (lineNumber = cclutExclusions->block[blockIndex].startLine to cclutExclusions->block[blockIndex].endLine)
      set lineNumberString = concat("<NBR>", trim(cnvtstring(lineNumber), 3), "</NBR>")
      set lineNumberPosition = findstring(lineNumberString, cclutCoverageXml, lineNumberPosition)
      if (lineNumberPosition > 0)
        set typePosition = findstring("<TYPE>", cclutCoverageXml, lineNumberPosition + textlen(lineNumberString)) + 6
        if (typePosition > 0)
          set cclutCoverageXml = concat(
            substring(1, typePosition-1, cclutCoverageXml), 
            CCLUT_NOT_EXECUTABLE, 
            substring(typePosition+1, coverageLen-typePosition, cclutCoverageXml)
          )
        endif
      endif
    endfor
  endfor
  return (cclutCoverageXml)
end ;;;excludeCoverage

%#endif CCLUT_CODE_COVERAGE_INC