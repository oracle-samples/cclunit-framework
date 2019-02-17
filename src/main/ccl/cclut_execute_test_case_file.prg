drop program cclut_execute_test_case_file:dba go
create program cclut_execute_test_case_file:dba
/**
  This program is responsible for executing a CCL Unit test case file and returning the test results and code coverage data.
  The consumer is expected to provide the cclutRequest and cclutReply structures. Structured results can be obtained by
  providing the cclutTestCaseResults structure. 
*/

%i cclsource:cclut_error_handling.inc
%i cclsource:cclut_compile_subs.inc
%i cclsource:cclut_code_coverage.inc
%i cclsource:cclut_env_utils.inc
%i cclsource:cclut_framework_version.inc
%i cclsource:cclut_get_file_as_string.inc
    
declare cclut::getTestCaseResultsXml(
    cclutTestCaseResults = vc(ref), cclutTestCaseFileName = vc, cclutLegacyResultsFormat = i2) = vc with protect
declare cclut::tagProgramIncludeFiles(cclutProgramDirectoryLogical = vc, cclutProgramName = vc) = vc with protect
declare cclut::setOptimizerMode(cclutOptimizerMode = vc) = i2 with protect
declare cclut::escapeCData(cclutSource = vc) = vc with protect
declare cclut::writeFileData(cclutDirectory = vc, cclutFileName = vc, cclutData = vc) = i4 with protect
declare cclut::retrieveListingData(cclutDirectory = vc, cclutProgramName = vc) = vc with protect


declare cclut::CCLSOURCE                = vc with protect, constant("CCLSOURCE")
declare cclut::CCLUSERDIR               = vc with protect, constant("CCLUSERDIR")
declare cclut::CER_TEMP                 = vc with protect, constant("cer_temp")
declare cclut::outputDirectory          = vc with protect, constant(concat(trim(logical(cclut::CCLUSERDIR), 3), "/"))
declare cclut::testCaseFileName         = vc with protect, noconstant("")
declare cclut::testCaseDirectory        = vc with protect, noconstant("")
declare cclut::testCaseId               = vc with protect, noconstant("")
declare cclut::testCaseObjectName       = vc with protect, noconstant("")
declare cclut::testCaseListingName      = vc with protect, noconstant("")
declare cclut::listingName              = vc with protect, noconstant("")
declare cclut::programIndex             = i4 with protect, noconstant(0)
declare cclut::programCount             = i4 with protect, noconstant(0)
declare cclut::coverageXml              = vc with protect, noconstant("")
declare cclut::xmlBeginPos              = i4 with protect, noconstant(0)
declare cclut::xmlEndPos                = i4 with protect, noconstant(0)
declare cclut::errorMessage             = vc with protect, noconstant("")
declare cclut::tempProgramName          = vc with protect, noconstant("")
declare cclut::currentCclVersion        = vc with protect, noconstant(cclut::getCclVersion(null))
declare cclut::deprecatedFlag           = vc with protect, noconstant(validate(cclutRequest->deprecatedFlag, "E"))
declare cclut::enforcePredeclare        = i2 with protect, noconstant(validate(cclutRequest->enforcePredeclare, TRUE))
declare cclut::legacyResultsFormat      = i2 with protect, noconstant(validate(cclutRequest->legacyResultsFormat, FALSE))
declare cclut::stat                     = i2 with protect, noconstant(0) 


/**
  The primary input for the CCL Testing Framework.
  
  @request
  @field testCaseDirectory
    (optional) A logical for the directory where the test case file resides, cer_temp by default.
  @field testCaseFileName
    The name of the test case file (.inc) file to be executed.
  @field testNamePattern
    (optional) A case-insensitive regular expression to limit which tests are executed. 
      Only tests with a matching name will be executed.
  @field programs
    A list of programs whose code coverage is to be recorded after each test execution.
    @field programName
      The name of the program whose code coverage data is to be retrieved.
    @field compile
      A boolean flag indicating whether the program should be compiled in debug mode.
      This is mostly useful when performing multiple tests against the same program. In such situations, 
      set this to 1 on the first execution and then 0 in subsequent executions.
      @value 0 Do not compile the program in debug mode.
      @value 1 Compile the program in debug mode.
  @field optimizerMode
    (optional: system default) The Oracle optimizer mode under which the tests will be executed.
  @field enforcePredeclare
    (optional: TRUE) A boolean indicating whether <tt>SET MODIFY PREDECLARE</tt> should be issued 
    prior to executing the test case.
    @value TRUE
      All variables encountered during test execution must be declared prior to use. By default an error will occur, but
      the severity is controlled by the deprecatedFlag. 
    @value FALSE
      Variable do not need to be predeclared.
  @field deprecatedFlag
    (optional: E) The severity for deprecated constructs encoutered while executing the tests.
    @value E Error
    @value W Warning
    @value L Log
    @value I Info
    @value D Debug
  @field legacyResultsFormat
    (optional) A boolean flag indidcating whether the results should be returned using the legacy format.

  record cclutRequest (
    1 testCaseDirectory = vc
    1 testCaseFileName = vc
    1 testNamePattern = vc
    1 programs[*]
      2 programName = vc
      2 compile = i2
    1 optimizerMode = vc
    1 enforcePredeclare = i2
    1 deprecatedFlag = vc
    1 legacyResultsFormat = i2
  ) with protect
*/

/**
  The primary output for the CCL Testing Framework.

  @reply
  @field environmentXml
    Information about the environment in which the test case was executed.
  @field listingXml
    The results from compiling the test case file into a test program object.
  @field coverageXml
    The coverage data for the test case code during execution of the test case.
  @field resultsXml
    The results from executing the test case.
  @field programs
    The list of programs that were checked for code coverage during test execution.
    @field programName
      The name of the program.
    @field listingXml
      The results from compiling the program.
    @field coverageXml
      The code coverage data for the program.

  record cclutReply (
    1 environmentXml = vc
    1 listingXml = vc
    1 coverageXml = vc
    1 resultsXml = vc
    1 programs[*]
      2 programName = vc
      2 listingXml = vc
      2 coverageXml = vc
    %i cclsource:status_block.inc
  ) with protect
*/

/**
 The request structure for executing a test program generated from a test case file.
 
 @field testNamePattern
   A case-insensitive regular expression for filtering which tests within the test case file to execute. 
   Only tests with a matching name will be executed.
*/
record cclutTestCaseRequest (
  1 testNamePattern = vc
) with protect

/**
  The reply structure for executing a test program generated from a test case file.

  @field resultInd 
    The overall testing result
      @value TRUE
        All assertion passed and no test caused errors.
      @value FALSE
        Some assert failed or some test casued errors.
  @field tests
    The list of tests that were executed
    @field name
      The name of the test.
    @field asserts
      The list of the assertions executed by the test.
      @field lineNumber
        The line number associated to this assertion.
      @field context
        The context associated to the assertion.
      @field resultInd
        The result of this assertion.
        @value TRUE
          The assertion passed.
        @value FALSE
          The assertion failed.
      @field condition
        The condition that was evaluted by this assertion.
    @field errors
      The list of CCL errors that occurred during the test execution.
      @field lineNumber
        The line number where the error occurred.
      @field errorText
        The error message for the error that occurred.
*/
;allow the calling program access to the structured results.
if (validate(cclutTestCaseResults) = FALSE)
  record cclutTestCaseResults (
    1 resultInd = i2
    1 tests[*]
      2 name = vc
      2 asserts[*]
        3 lineNumber = i4
        3 context = vc
        3 resultInd = i2 ;TRUE = PASS, FALSE = FAIL
        3 condition = vc
      2 errors[*]
        3 lineNumber = i4
        3 errorText = vc        
%i cclsource:status_block.inc
  ) with protect
endif

/**
  Retrieves the formated XML for a set of test case results.

  @param cclutTestCaseResults
    The results from executing a test case in the following record structre format:
    <pre>
      record cclutTestCaseResults (
        1 resultInd = i2
        1 tests[*]
          2 name = vc
          2 asserts[*]
            3 lineNumber = i4
            3 context = vc
            3 resultInd = i2 ;TRUE = PASS, FALSE = FAIL
            3 condition = vc
          2 errors[*]
            3 lineNumber = i4
            3 errorText = vc
      %i cclsource:status_block.inc
      ) with protect
    </pre>
  @param cclutTestCaseFileName
    The test case file name.
  @param cclutLegacyResultsFormat
    A boolean flag indicating whether to return the results in the legacy format which uses the tag name
    TEST rather than CONDITION
  @returns
    The test case results in XML format.
*/
subroutine cclut::getTestCaseResultsXml(cclutTestCaseResults, cclutTestCaseFileName, cclutLegacyResultsFormat)
  declare cclutReturnVal = vc with protect, noconstant("")
  declare cclutTestIndex = i4 with protect, noconstant(0)
  declare cclutAssertIndex = i4 with protect, noconstant(0)
  declare cclutErrorIndex = i4 with protect, noconstant(0)
  declare cclutOverallResultInd = i2 with protect, noconstant(TRUE)
  declare cclutEscapedCondition = vc with protect, noconstant("")
  declare cclutConditionStartTag = vc with protect, noconstant("<CONDITION>")
  declare cclutConditionEndTag = vc with protect, noconstant("</CONDITION>")
  
  if (cclutLegacyResultsFormat)
    set cclutConditionStartTag = "<TEST>"
    set cclutConditionEndTag = "</TEST>"
  endif
  
  set cclutReturnVal = "<TESTS>"
  for (cclutTestIndex = 1 to size(cclutTestCaseResults->tests, 5))
    set cclutReturnVal = 
        build(cclutReturnVal, 
          "<TEST>", 
          "<NAME>", cclutTestCaseResults->tests[cclutTestIndex].name, "</NAME>"
        )
    set cclutReturnVal = build(cclutReturnVal, "<ASSERTS>")
    set cclutOverallResultInd = TRUE
    for (cclutAssertIndex = 1 to size(cclutTestCaseResults->tests[cclutTestIndex].asserts, 5))
      if (cclutTestCaseResults->tests[cclutTestIndex].asserts[cclutAssertIndex].resultInd = FALSE)
        set cclutOverallResultInd = FALSE
      endif
      ;add assert information
      set cclutEscapedCondition = 
          cclut::escapeCData(cclutTestCaseResults->tests[cclutTestIndex].asserts[cclutAssertIndex].condition)
      set cclutReturnVal =
          build(cclutReturnVal, 
            "<ASSERT>", 
              "<LINENUMBER>", 
                  cclutTestCaseResults->tests[cclutTestIndex].asserts[cclutAssertIndex].lineNumber, 
              "</LINENUMBER>", 
              "<CONTEXT>",
                  "<![CDATA[", cclutTestCaseResults->tests[cclutTestIndex].asserts[cclutAssertIndex].context, "]]>",
              "</CONTEXT>", 
              "<RESULT>", 
                  evaluate(cclutTestCaseResults->tests[cclutTestIndex].asserts[cclutAssertIndex].resultInd, 
                      TRUE, "PASSED", "FAILED"), 
              "</RESULT>", 
              cclutConditionStartTag,
                  "<![CDATA[", cclutEscapedCondition, "]]>",
              cclutConditionEndTag, 
            "</ASSERT>"
          )
    endfor ;;;cclutAssertIndex
    set cclutReturnVal = build(cclutReturnVal, "</ASSERTS>")
 
    set cclutReturnVal = build(cclutReturnVal, "<ERRORS>")
    for (cclutErrorIndex = 1 to size(cclutTestCaseResults->tests[cclutTestIndex].errors, 5))
      ;add error information.
      set cclutReturnVal =
          build(cclutReturnVal, 
            "<ERROR>", 
              "<LINENUMBER>", 
                  cclutTestCaseResults->tests[cclutTestIndex].errors[cclutErrorIndex].lineNumber, 
              "</LINENUMBER>", 
              "<ERRORTEXT>", 
                  "<![CDATA[", cclutTestCaseResults->tests[cclutTestIndex].errors[cclutErrorIndex].errorText, "]]>",
              "</ERRORTEXT>",
            "</ERROR>"
          )
    endfor ;;;cclutErrorIndex
    set cclutReturnVal = build(cclutReturnVal, "</ERRORS>")
 
    ;add the overall result
    if (size(cclutTestCaseResults->tests[cclutTestIndex].errors, 5) > 0)
      set cclutReturnVal = build(cclutReturnVal, "<RESULT>ERRORED</RESULT>")
    else
      set cclutReturnVal = build(cclutReturnVal, "<RESULT>", evaluate(cclutOverallResultInd, TRUE, "PASSED", "FAILED"), "</RESULT>")
    endif
 
    set cclutReturnVal = build(cclutReturnVal, "</TEST>")
  endfor ;;;cclutTestIndex
 
  set cclutReturnVal = build(cclutReturnVal, "</TESTS>")
  set cclutReturnVal = build("<TESTCASE><NAME>", cclutTestCaseFileName, "</NAME>", cclutReturnVal, "</TESTCASE>")
  return (cclutReturnVal)
end ;;;getTestCaseResultsXml

/**
  Parses a specified program file to identify all included files, tags each one with ";;;;CCLUT_START_INC_FILE"
  and ";;;;CCLUT_END_INC_FILE" and writes the result to a temporary file in CCLUSERDIR.  The name
  of the temporary file is returned.
  <p />
  <b>Note</b>: The program file cannot have lines that exceed 132 characters.
 
  @param cclutProgramDirectoryLogical
    A logical for the directory in which the .prg file resides
  @param cclutProgramName
    The name of the .prg file to be parsed.
  @returns
    The name of the temporary file that contains the original .prg file contents with the include file tags.
*/
subroutine cclut::tagProgramIncludeFiles(cclutProgramDirectoryLogical, cclutProgramName)
  declare cclutProgramFileLocation = vc with protect, noconstant("")
  declare cclutModifiedProgramName = vc with protect, noconstant("")
 
  set cclutProgramFileLocation = concat(trim(logical(cnvtupper(cclutProgramDirectoryLogical)), 3), "/")
  set cclutProgramFileLocation = cnvtlower(concat(cclutProgramFileLocation, cclutProgramName, ".prg"))
  set cclutModifiedProgramName = concat("temp", cnvtlower(cclutProgramName), ".dat")
 
  free define rtl2
  set logical file_location cclutProgramFileLocation
  define rtl2 is "file_location"
 
  select into value(cclutModifiedProgramName)
      r.line
  from rtl2t r
  head report
  value = FILLSTRING(132, " ")
  detail
      if (cnvtlower(r.line) = "%i *")
          value = trim(concat(";;;;CCLUT_START_INC_FILE ", r.line), 3)
          col 0 value
          row + 1
          value = r.line
          col 0 value
          row + 1
          value = trim(concat(";;;;CCLUT_END_INC_FILE ", r.line), 3)
          col 0 value
          row + 1
      else
          value = r.line
          col 0 value
          row + 1
      endif
  with nocounter, maxcol = 133, formfeed = none
  return (cclutModifiedProgramName)
end ;;;tagProgramIncludeFiles

/**
  Sets the optimizer mode as specified. If no optimizer mode is specified, then the current session 
  optimizer mode is used.
 
  @param cclutOptimizerMode
    The optimizer mode to set. Ignoring case, this must be "CBO", "RBO" or "".
  @returns A boolean flag indicating whether a valid optimizer mode was passed in.
*/
subroutine cclut::setOptimizerMode(cclutOptimizerMode)
  case (trim(cnvtupper(cclutOptimizerMode), 3))
    of "CBO":
      call parser("RDB ALTER SESSION SET OPTIMIZER_MODE = ALL_ROWS GO")
    of "RBO":
      call parser("RDB ALTER SESSION SET OPTIMIZER_MODE = RULE GO")
    else
      return (FALSE)
  endcase
  
  return (TRUE)
end ;;;setOptimizerMode

/**
  Replaces the < from all <![CDATA[ with &lt; and the > from all ]]> with &gt;
*/
subroutine cclut::escapeCData(cclutSource)
  declare cclutRetVal = vc with protect, noconstant("")

  set cclutRetVal = replace(cclutSource, "<![CDATA[", "&lt;![CDATA[")
  set cclutRetVal = replace(cclutRetVal, "]]>", "]]&gt;")
  return (cclutRetVal)
end ;;;escapeCData


/**
  Writes data to a file either creating the file or completely overwriting it.
  @param cclutDirectory
    The directory to house the file.
  @param cclutFileName
    The name of the file.
  @param cclutData
    The data to write to the file.
*/
subroutine cclut::writeFileData(cclutDirectory, cclutFileName, cclutData)
  declare errorMessage = vc with protect, noconstant("")
  declare stat = i4 with protect, noconstant(0)
  declare bytesWritten = i4 with protect, noconstant(0)
  
  record frec(
    1 file_desc = i4
    1 file_offset = i4
    1 file_dir = i4
    1 file_name = vc
    1 file_buf = vc
  ) with protect
     
  set frec->file_name = concat(cnvtlower(cclutDirectory), "/", cnvtlower(cclutFileName))
  set frec->file_buf  = "w+" 
  set stat = cclio("OPEN", frec)
  if (error(errorMessage, 1) > 0)
    return (0)
  endif
  set frec->file_buf  = cclutData 
  set bytesWritten = cclio("WRITE", frec)
  set stat = cclio("CLOSE", frec)
  return (bytesWritten)
end ;;;writeFileData


/**
  Retrieves the xml listing data for a program if it exists and was generated since the program was last compiled and 
  if the program was last compiled by the current user. Returns null otherwise.
  @param cclutDirectory
    The directory where the listing is located.
  @param cclutProgramName
    The name of the program to retrieve the listing for.
  @retrns
    The program's xml listing data or null.
*/
subroutine cclut::retrieveListingData(cclutDirectory, cclutProgramName)
  declare listingData = vc with protect, noconstant("")
  declare listingDateStart = i4 with protect, noconstant(0)
  declare listingDateEnd = i4 with protect, noconstant(0)
  declare listingDateSring = vc with protect, noconstant("")
  declare listingDate = dq8 with protect, noconstant(0)
  declare compileDate = dq8 with protect, noconstant(cnvtdatetime(curdate, curtime3))
  
  set listingData = trim(cclut::getFileAsString(concat(cclutDirectory, "/", cclutProgramName, ".listing.xml")))
  set listingDateStart = findstring("<COMPILE_DATE>", listingData) + 14
  if (listingDateStart > 14)
    set listingDateEnd = findstring("</COMPILE_DATE>", listingData, listingDateStart)
  endif
  if (listingDateEnd > 0)
    set listingDateSring = substring(listingDateStart, listingDateEnd - listingDateStart, listingData)
    set listingDate = cnvtdatetime(listingDateSring)
    select into 'nl:' from dprotect d where d.platform = 'H0000' and d.rcode = '5' and d.group = curgroup and d.object = 'P'
      and d.object_name = cnvtupper(cclutProgramName) and d.user_name = curuser 
    detail
      compileDate = cnvtdatetime(d.datestamp, d.timestamp)
    with nocounter
    if (compileDate <= listingDate)
      return (listingData)
    endif
  endif
  return ("")
end ;;;retrieveListingData

set modify maxvarlen 10000000
set cclutReply->status_data.status = "S"

if (cclut::compareCclVersion(cclut::currentCclVersion, cclut::MINIMUM_REQUIRED_CCL_VERSION) = TRUE)
  set cclutReply->status_data.status = "F"
  set cclutReply->status_data.subeventstatus[1].targetObjectValue =
    concat("The CCL version [", cclut::currentCclVersion, "] does not meet the minimum version required [", 
           cclut::MINIMUM_REQUIRED_CCL_VERSION, "] for this version of the CCL Unit Testing Framework")
  go to exit_script
endif

set cclutReply->environmentXml = cclut::getEnvironmentDataXml(null)

set cclut::testCaseId = concat(trim(currdbhandle, 3), "_", trim(cnvtstring(cnvtint(curtime3)), 3))
set cclut::testCaseObjectName = concat("prg_", cclut::testCaseId)
set cclut::testCaseListingName = concat("cclut_inc_", cclut::testCaseId, ".lis")

set cclut::stat = alterlist(cclutReply->programs, size(cclutRequest->programs, 5))
for (cclut::programCount = 1 to size(cclutRequest->programs, 5))
  set cclutReply->programs[cclut::programCount].programName = cclutRequest->programs[cclut::programCount].programName
 
  if (cclutRequest->programs[cclut::programCount].compile = TRUE)
    ;Identify the .inc files included by the program.
    
    set cclut::tempProgramName = cclut::tagProgramIncludeFiles(cclut::CCLSOURCE, 
        cclutReply->programs[cclut::programCount].programName)
    set cclut::tempProgramName = cnvtlower(cclut::tempProgramName)
 
    set cclut::listingName = concat("cclut_prg_", cclut::testCaseId, trim(cnvtstring(cclut::programCount), 3), ".lis")
    ;;prevent error mis-interpretation by compileProgram
    call cclut::exitOnError("pre-compile", cclutReply->programs[cclut::programCount].programName, cclutReply)
    if (cclut::compileProgram(
        cclut::CCLUSERDIR, cclut::tempProgramName, cclut::CCLUSERDIR, cclut::listingName, cclut::errorMessage) = TRUE)
      set cclutReply->programs[cclut::programCount].listingXml =
          cclut::getListingXml(cclutReply->programs[cclut::programCount].programName, cclut::outputDirectory, cclut::listingName)
      call cclut::writeFileData(trim(logical(cclut::CER_TEMP),3), 
          concat(cclutReply->programs[cclut::programCount].programName, ".listing.xml"), 
          concat(cclutReply->programs[cclut::programCount].listingXml, char(10), char(13)))
      set cclut::stat = remove(concat(cclut::outputDirectory, cclut::listingName))
      set cclut::stat = remove(concat(cclut::outputDirectory, cclut::tempProgramName))
    else
      set cclutReply->status_data.status = "F"
      set cclutReply->status_data.subeventstatus[1].operationName = "compileProgram"
      set cclutReply->status_data.subeventstatus[1].operationStatus = "F"
      set cclutReply->status_data.subeventstatus[1].targetObjectName = cclut::tempProgramName
      set cclutReply->status_data.subeventstatus[1].targetObjectValue =
            concat("compileProgram ", cclutReply->programs[cclut::programCount].programName, " failed: ", cclut::errorMessage)
      go to exit_script
    endif
  else
    set cclutReply->programs[cclut::programCount].listingXml = 
        cclut::retrieveListingData(trim(logical(cclut::CER_TEMP),3), cclutReply->programs[cclut::programCount].programName)
  endif
endfor

;create a test program object from the test case file that executes the tests in the test case when it is executed.
set cclut::testCaseFileName = trim(cnvtlower(cclutRequest->testCaseFileName), 3)
set cclut::testCaseDirectory = trim(validate(cclutRequest->testCaseDirectory, cclut::CER_TEMP), 3)
;;prevent error mis-interpretation by generateTestCaseProgram
call cclut::exitOnError("pre-generate", cclut::testCaseFileName, cclutReply)
if (cclut::generateTestCaseProgram(cclut::testCaseDirectory, cclut::testCaseFileName, 
    cclut::CCLUSERDIR, cclut::testCaseListingName, cclut::testCaseObjectName, cclut::errorMessage) = FALSE)
  set cclutReply->status_data.status = "F"
  set cclutReply->status_data.subeventstatus[1].operationName = "generateTestCaseProgram"
  set cclutReply->status_data.subeventstatus[1].operationStatus = "F"
  set cclutReply->status_data.subeventstatus[1].targetObjectName = cclut::testCaseFileName
  set cclutReply->status_data.subeventstatus[1].targetObjectValue = cclut::errorMessage
  go to exit_script
elseif (checkprg(cnvtupper(cclut::testCaseObjectName)) = 0)
  set cclutReply->status_data.status = "F"
  set cclutReply->status_data.subeventstatus[1].operationName = "CHECKPRG"
  set cclutReply->status_data.subeventstatus[1].operationStatus = "F"
  set cclutReply->status_data.subeventstatus[1].targetObjectName = cclut::testCaseObjectName
  set cclutReply->status_data.subeventstatus[1].targetObjectValue = concat("Test case program for ", 
      cclut::testCaseFileName, " not in CCL dictionary after compilation.")
  go to exit_script
endif
 
if (validate(cclutRequest->optimizerMode, "") != "")
  if (not cclut::setOptimizerMode(cclutRequest->optimizerMode))
    set cclutReply->status_data.status = "F"
    set cclutReply->status_data.subeventstatus[1].targetObjectValue = "Invalid optimizer mode specified."
    go to exit_script
  endif
endif

;set the severity for deprecated constructs: E=Error, W=Warning, L=Log, I=Info, D=Debug
if (textlen(trim(cclut::deprecatedFlag)) = 0)
  set cclut::deprecatedFlag = "E"
endif
set trace deprecated value(cclut::deprecatedFlag)
if (cclut::enforcePredeclare = TRUE)
  ;causes CCL to require variables to be declared before use. requires deprecated severity E.
  set modify predeclare
endif

set cclutTestCaseRequest->testNamePattern = validate(cclutRequest->testNamePattern, "")

call cclut::exitOnError("pre-execute", cclut::testCaseObjectName, cclutReply)
execute value(cnvtupper(cclut::testCaseObjectName)) with 
    replace ("CCLUTREQUEST", cclutTestCaseRequest), 
    replace ("CCLUTREPLY", cclutTestCaseResults)

set modify nopredeclare
set trace nodeprecated

if (cclutTestCaseResults->status_data.status = "F")
  set cclut::stat = moverec(cclutTestCaseResults->status_data, cclutReply->status_data)
  go to clean_up
endif

set cclutReply->resultsXml = 
    cclut::getTestCaseResultsXml(cclutTestCaseResults, cclut::testCaseFileName, cclut::legacyResultsFormat)
 
call cclut::exitOnError("pre-coverage", cclut::testCaseObjectName, cclutReply)
set cclut::coverageXml = cclut::getCoverageXml(null)
call cclut::filterCoverageXml(cclut::coverageXml, cclutReply)

for (cclut::programCount = 1 to size(cclutRequest->programs, 5))
  if (cclutRequest->programs[cclut::programCount].compile = FALSE)
    set cclutReply->programs[cclut::programCount].listingXml = ""
  endif
endfor

set cclutReply->listingXml = 
    cclut::getTestCaseListingXml(cclut::testCaseObjectName, cclut::outputDirectory, cclut::testCaseListingName)
 
set cclutReply->coverageXml = 
    cclut::getTestCaseCoverageXml(cclut::coverageXml, cclut::testCaseObjectName, cclutReply->listingXml)
 
;Replace the name in the test case listing with the original test case name
set cclut::xmlBeginPos = findstring("<LISTING_NAME>", cclutReply->listingXml, 1, 0)
set cclut::xmlEndPos = findstring("</LISTING_NAME>", cclutReply->listingXml, 1, 0)
set cclutReply->listingXml = 
    concat(
      substring(1, cclut::xmlBeginPos+13, cclutReply->listingXml), 
      trim(cnvtupper(cclutRequest->testCaseFileName), 3), 
      substring(cclut::xmlEndPos, textlen(cclutReply->listingXml)-cclut::xmlEndPos+1, 
      cclutReply->listingXml)
    )
 
set cclut::xmlBeginPos = findstring("<COVERAGE_NAME>", cclutReply->coverageXml, 1, 0)
set cclut::xmlEndPos = findstring("</COVERAGE_NAME>", cclutReply->coverageXml, 1, 0)
set cclutReply->coverageXml = 
    concat(
      substring(1, cclut::xmlBeginPos+14, cclutReply->coverageXml), 
      trim(cnvtupper(cclutRequest->testCaseFileName), 3),
      substring(cclut::xmlEndPos, textlen(cclutReply->coverageXml)-cclut::xmlEndPos+1, 
      cclutReply->coverageXml)
    )

#clean_up
if (validate(cclut::noRemove, FALSE) = FALSE)
  set cclut::stat = remove(concat(cclut::outputDirectory, cclut::testCaseListingName))
endif

#exit_script

if (validate(cclut::noDrop, FALSE) = FALSE)
  call parser(concat("drop program ", cclut::testCaseObjectName, " go"))
endif

if (cclutReply->status_data.status = "S")
  if (cclut::errorOccurred(cclut::errorMessage))
    set cclutReply->status_data.status = "F"
    set cclutReply->status_data.subeventstatus[1].operationStatus = "F"
    set cclutReply->status_data.subeventstatus[1].targetObjectValue = cclut::errorMessage
  endif
endif

if (validate(cclut::debug, FALSE) = TRUE)
  call echorecord(cclutReply)
endif
   
end go
