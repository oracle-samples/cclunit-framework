drop program cclut_ff:dba go
create program cclut_ff:dba
/**
  A prompt program for compiling and executing the manual test runner which fails fast when an error or assertion failure
  occurs and simply echos the results to stdout.
  @arg (vc) The output destination for the results: stdout (MINE), a file, or a print queue.
    @default MINE
  @arg (vc) A case-insensitive logical for the directory where the test case file is located.
    @default CCLUSERDIR
  @arg (vc) The name of the test case file with or without the .inc extension. 
  @arg (vc) A case-insensitive regular expression for limiting, by name, which tests within the test case get executed.
    @default .*
  @arg The case-insensitive optimizer mode (CBO, RBO) to set for the current session before runnning the tests. 
  The session optimizer mode is left unchanged if any other value is provided. 
    @default current
  @arg (vc) The deprecated severity to apply.
    @default E
*/
prompt 
  "Ouput Destination [MINE]: " = "MINE",
  "Test Case Directory [cclsource]: " = "cclsource",
  "Test Case File Name: " = "",
  "Test Name Pattern [.*]: " = "",
  "Optimizer Mode (CBO, RBO) [current]: " = "", 
  "Deprecated Flag (E, W, L, I, D) [E]: " = ""
with outdev, testCaseDirectory, testCaseFileName, testNamePattern, optimizerMode, deprecatedFlag

  declare cclut_ff::doOutput(cclutMsg = vc) = null with protect
  declare cclut_ff::errorCheck(cclutErrorOption = i4, cclutDoExit = i2) = i2 with protect

  declare cclut_test_runner::FUN_TYPE_SUBROUTINE = i4 with protect, constant(7)
  declare cclut_ff::testCaseDirectory = vc with protect, noconstant(trim(cnvtlower($testCaseDirectory), 3))
  declare cclut_ff::testCaseFileName = vc with protect, noconstant(trim(cnvtlower($testCaseFileName), 3))
  declare cclut_ff::testCaseFileLocation = vc with protect, noconstant("")
  declare cclut_ff::testProgramName = vc with protect, noconstant("")
  declare cclut_ff::testProgramFile = vc with protect, noconstant("")
  declare cclut_ff::testProgramListingFile = vc with protect, noconstant("")
  declare cclut_ff::testNamePattern = vc with protect, noconstant(trim($testNamePattern, 3))
  declare cclut_ff::deprecatedFlag = vc with protect, noconstant(trim($deprecatedFlag, 3))
  declare cclut_ff::outputLine = vc with protect, noconstant("")
  declare cclut_ff::testCaseId = vc with protect, noconstant(trim(currdbhandle, 3))
  declare cclut_ff::stat = i4 with protect, noconstant(0)
  declare cclut_ff::output = vc with protect, noconstant(trim(""))

  if (validate(_memory_reply_string) = FALSE)
    declare _memory_reply_string = vc with protect, noconstant("")
  endif  

  /**
    Echoes text and append it to the cclut_ff::output variable which will set in _memory_reply_string on program exit.
    @param cclutMsg
      The text to echo/append.
  */
  subroutine cclut_ff::doOutput(cclutMsg)    
    call echo(cclutMsg) ;intentional
    if (cclut_ff::output = "")
      set cclut_ff::output = cclutMsg
    else
      set cclut_ff::output = concat(cclut_ff::output, char(10), char(13), cclutMsg)
    endif
  end ;;;doOutput

  /**
    Checks if a CCL error has occured and echoes the error message if one has
    @param cclutErrorOption
      The value to be passed into the error function indicating whether it should pull the first error or the last.
    @param cclutDoExit
      A boolean flag indicating whether or not to route control to exit_script if an error has occurred.
    @returns
      A boolean flag indicating whether or not an error has occured. TRUE: there was an error; FALSE there was not an error.
  */
  subroutine cclut_ff::errorCheck(cclutErrorOption, cclutDoExit)
    declare cclutffErrorMessage = vc with protect, noconstant("")
    declare cclutffErrorCode = i2 with protect, noconstant(0)
    set cclutffErrorCode = error(cclutffErrorMessage, cclutErrorOption)
    if (cclutffErrorCode != 0)
      call cclut_ff::doOutput(cclutffErrorMessage)
      if (cclutDoExit = TRUE)
        go to exit_script
      endif
      return (TRUE)
    endif
    return (FALSE)
  end ;;;errorCheck

  ;set the codecover trace here because CCL will error when the framework sets it if the session or top-level script did not.
  if (curimage = "CCL")
    set trace codecover 0; summary
  endif
  
  if (textlen(trim(cclut_ff::testCaseFileName)) = 0)
    call cclut_ff::doOutput("A test case name must be provided")
    go to exit_script
  endif
  if (cclut_ff::testCaseFileName = patstring("*.inc"))
    set cclut_ff::testCaseFileName = substring(1, textlen(cclut_ff::testCaseFileName)-4, cclut_ff::testCaseFileName)
  endif

  set cclut_ff::testProgramName = concat("cclut_", cclut_ff::testCaseId)
  set cclut_ff::testProgramFile = concat(cclut_ff::testProgramName, ".dat")
  set cclut_ff::testProgramListingFile = concat(cclut_ff::testProgramName, ".lis")

  set cclut_ff::testCaseFileLocation = concat(cclut_ff::testCaseFileName, ".inc")
  if (textlen(cclut_ff::testCaseDirectory) = 0)
    set cclut_ff::testCaseDirectory = "cclsource"
  endif
  set cclut_ff::testCaseFileLocation = concat(cclut_ff::testCaseDirectory, ":", cclut_ff::testCaseFileLocation)
  call cclut_ff::errorCheck(1, TRUE)

  select into cclut_ff::testProgramName from dual detail
    cclut_ff::outputLine = concat("drop program ", cclut_ff::testProgramName, " go")
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = concat("create program ", cclut_ff::testProgramName)
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = "%i cclsource:cclut_test_runner.inc"
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = "%i cclsource:cclutmock.inc"
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = concat("%i ", cclut_ff::testCaseFileLocation)
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = \
      concat("declare cclut_test_runner::deprecatedFlag = vc with protect, noconstant('", cclut_ff::deprecatedFlag, "')")
    cclut_ff::outputLine row+1
    if (textlen(cclut_ff::testNamePattern) > 0)
      cclut_ff::outputLine = concat("call cclut_test_runner::runMatchingUnitTests('", cclut_ff::testNamePattern, "')")
    else
      cclut_ff::outputLine = "call cclut_test_runner::runAllUnitTests(null)"
    endif
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = "%i cclsource:cclut_test_runner_end.inc"
    cclut_ff::outputLine row+1
    cclut_ff::outputLine = "end go"
    cclut_ff::outputLine row+1
  with nocounter

  call cclut_ff::errorCheck(1, TRUE)
  
  set COMPILE = DEBUG
  call compile(value(cclut_ff::testProgramFile), value(cclut_ff::testProgramListingFile), 1)
  set COMPILE = NODEBUG
  
  if (cclut_ff::errorCheck(0, FALSE) = FALSE)
    set cclut_ff::stat = remove(cclut_ff::testProgramFile)
    set cclut_ff::stat = remove(cclut_ff::testProgramListingFile)

    case (trim(cnvtupper($optimizerMode), 3))
      of "CBO":
        call parser("RDB ALTER SESSION SET OPTIMIZER_MODE = ALL_ROWS GO")
      of "RBO":
        call parser("RDB ALTER SESSION SET OPTIMIZER_MODE = RULE GO")
    endcase
      
    execute value(cnvtupper(cclut_ff::testProgramName)) with replace("ECHO", echo)
  else
    while (cclut_ff::errorCheck(0, FALSE) = TRUE)
      set cclut_ff::stat = 0; this is no-op to keep calling error check till the error queue is empty.
    endwhile
    call cclut_ff::doOutput("Failed to generate the test case program.") 
    call cclut_ff::doOutput(build2("Check the listing file ", value(cclut_ff::testProgramListingFile), "."))    
  endif  

#exit_script

call cclut_ff::errorCheck(1, FALSE)

set _memory_reply_string = cclut_ff::output

;prevent loads of useless code coverage data from being dumped at the end of the output.
if ($outdev = "MINE")
;;;CCLUNIT:OFF
  set trace nocost 
;;;CCLUNIT:ON
endif
  
end go
