%#ifndef CCLUT_COMPILE_SUBS_INC
%#def CCLUT_COMPILE_SUBS_INC
;**********************************************************************************************************************************
;
; Contains subroutines to compile scripts and include files
;
;**********************************************************************************************************************************
 
%i cclsource:cclut_error_handling.inc

declare cclut::generateTestCaseProgram(cclutDirectoryLogical = vc, cclutFileName = vc, cclutListingLogical = vc, 
    cclutListingName = vc, cclutProgramName = vc, cclutFailureMessage = vc(ref)) = i2
declare cclut::compileProgram(cclutDirectoryLogical = vc, cclutFileName = vc, cclutListingLogical = vc, 
    cclutListingName = vc, cclutFailureMessage = vc(ref)) = i2
declare cclut::listingHasErrors(cclutProgramFileName = vc, cclutListingLocation = vc) = i2   
 
/**
  Creates a program from a test case file.
  @param cclutDirectoryLogical
    A logical for the directory where the test case (.inc file) is located (e.g., CCLSOURCE, CCLUSERDIR).
  @param cclutFileName 
    The name of the test case file to compile with or without the extension which must be .inc.
  @param cclutListingLogical 
    A logical for the directory in which the listing file should be stored (e.g., cer_temp).
  @param cclutListingName
    The name to use for the listing file
  @param cclutProgramName
    The name that will be given to the compiled prorgram object
  @param cclutFailureMessage
    A explaining what went wrong if the return value is false.
  @returns
    A boolean flag indicating whether the compile was successful.
    @value TRUE 
      The test case compile was successful.
    @value FALSE
      The test case compile failed.
*/
subroutine cclut::generateTestCaseProgram(
    cclutDirectoryLogical, cclutFileName, cclutListingLogical, cclutListingName, cclutProgramName, cclutFailureMessage)
  declare CCLUT_CCLUSERDIR = vc with protect, constant("CCLUSERDIR")
  declare cclutCcluserDirectory = vc with protect, noconstant("")
  declare cclutProgramFileLocation = vc with protect, noconstant("")
  declare cclutTestCaseLocation = vc with protect, noconstant("")
  declare cclutTestCaseDirectory = vc with protect, noconstant("")
  declare cclutFileExtension = vc with protect, noconstant("")
  declare cclutFileNameLower = vc with protect, noconstant("")
  declare cclutStat = i4 with protect, noconstant(0)
   
  set cclutCcluserDirectory = cnvtlower(concat(trim(logical(CCLUT_CCLUSERDIR), 3), "/"))
  set cclutTestCaseDirectory = cnvtlower(concat(trim(logical(cclutDirectoryLogical), 3), "/"))
 
  ;Make sure files are lowercase
  set cclutFileNameLower = trim(cnvtlower(cclutFileName), 3)
  set cclutListingName = trim(cnvtlower(cclutListingName), 3)
  set cclutProgramName = trim(cnvtlower(cclutProgramName), 3)
 
  ;Add the .inc extension if not provided.
  if (textlen(cclutFileNameLower) > 4)
    set cclutFileExtension = substring(textlen(cclutFileNameLower)-3, 4, cclutFileNameLower)
  endif
 
  if (cclutFileExtension != ".inc")
    set cclutFileNameLower = concat(cclutFileNameLower, ".inc")
  endif
 
  ;Make sure the file exist
  set cclutTestCaseLocation = concat(cclutTestCaseDirectory, cclutFileNameLower)
  if (not findfile(cclutTestCaseLocation))
    set cclutFailureMessage = concat(cclutFileNameLower, " not found in ", cclutDirectoryLogical)
    return (FALSE)
  endif
 
  ;Create a program file for a program that executes the test case. A .dat extension will be added.
  select into value(cclutProgramName)
  from (dummyt d1 with seq = 1)
  plan d1
  head report
    value = fillstring(132, " ")
    value = concat("drop program ", cclutProgramName, " go")
    col 0 value row+1
    value = concat("create program ", cclutProgramName)
    col 0 value row+1
    value = "%i cclsource:cclutassert_impl_nff.inc"
    col 0 value row+1
    value = "%i cclsource:cclut_error_handling.inc"
    col 0 value row+1
    value = "%i cclsource:cclut_timers.inc"
    col 0 value row+1
    value = "%i cclsource:cclut_reflection_subs.inc"
    col 0 value row+1
    value = "%i cclsource:cclutmock.inc"
    col 0 value row+1
  detail
    col 0 ";;;CCLUT_START_TEST_INC_FILE" row+1
    value = concat("%i ", cclutDirectoryLogical, ":", cclutFileNameLower)
    col 0 value row+1
    col 0 ";;;CCLUT_END_TEST_INC_FILE" row+1
  foot report
    value = "%i cclsource:cclut_execute_test_logic.inc"
    col 0 value row+1
    value = "end go"
    col 0 value row+1
  with nocounter, maxrow = 500, maxcol = 133, compress, nullreport, noheading, format = variable
 
  set cclutProgramFileLocation = concat(cclutCcluserDirectory, cclutProgramName, ".dat")
  if (not findfile(cclutProgramFileLocation))
    set cclutFailureMessage = concat("generated file ", cclutProgramName, ".dat not found in ccluserdir")
    return (FALSE)
  endif
 
  if (not cclut::compileProgram(
      CCLUT_CCLUSERDIR, concat(cclutProgramName, ".dat"), cclutListingLogical, cclutListingName, cclutFailureMessage))
    set cclutStat = remove(cclutProgramFileLocation)
    return (FALSE)
  endif

  set cclutStat = remove(cclutProgramFileLocation)
  if (not cclut::errorOccurred(cclutFailureMessage))
    return (TRUE)
  endif
end ;;;generateTestCaseProgram
 
 
/**
  Compiles a CCL program creating a listing file in a specified location.
  @param cclutDirectoryLogical 
    A logical for the directory in which the progam file resides (e.g., CCLSOURCE, CCLUSERDIR).
  @param cclutFileName 
    The name of the program file to compile including the extension (e.g., myProgram.prg, myProgram.dat).
  @param cclutListingLogical 
    The logical for the directory in which the listing file should be stored (e.g., cer_temp)
  @param cclutListingName 
    The name to use for the listing file.
  @param cclutFailureMessage
    A explaining what went wrong if the return value is false.
  @returns
    A boolean flag indicating whether the compile was successful.
    @value TRUE 
      The program compile was successful.  
    @value FALSE 
      The program compile failed.
 */
subroutine cclut::compileProgram(cclutDirectoryLogical, cclutFileName, cclutListingLogical, cclutListingName, cclutFailureMessage)
  declare cclutProgramDirectory = vc with protect, noconstant("")
  declare cclutProgramFileLocation = vc with protect, noconstant("")
  declare cclutListingFileLocation = vc with protect, noconstant("")
  declare cclutListingDirectory = vc with protect, noconstant("")
 
  ;Set directories
  set cclutProgramDirectory = cnvtlower(concat(trim(logical(cclutDirectoryLogical), 3), "/"))
  set cclutListingDirectory = cnvtlower(concat(trim(logical(cclutListingLogical), 3), "/"))
 
  ;Make sure the all files are in lowercase
  set cclutFileName = trim(cnvtlower(cclutFileName), 3)
  set cclutListingName = trim(cnvtlower(cclutListingName), 3)
 
  ;Make sure the PRG file exist in specified location
  set cclutProgramFileLocation = concat(cclutProgramDirectory, cclutFileName)
  if (not findfile(cclutProgramFileLocation))
    set cclutFailureMessage = concat(cclutFileName, " not found in ", cclutDirectoryLogical)
    return (FALSE)
  endif
 
  ;Compile the program
  set cclutListingFileLocation = concat(cclutListingDirectory, cclutListingName)
  set COMPILE = DEBUG
  call compile(value(cclutProgramFileLocation), value(cclutListingFileLocation), 1)
  set COMPILE = NODEBUG
 
  if (cclut::listingHasErrors(cclutProgramFileLocation, cclutListingFileLocation) = FALSE)
    set cclutFailureMessage = concat("compile errors detected in ", cclutListingFileLocation,
      ". Search for a line that does not start with a number using ^[^\d] or an equivalent regular expression.")
    return (FALSE)
  endif
  if (cclut::errorOccurred(cclutFailureMessage))
    set cclutFailureMessage = concat(cclutFailureMessage, "  Compile errors detected in ", cclutListingFileLocation)
    return (FALSE)
  endif
  return (TRUE)
end ;;;compileProgram


/**
  This covers for a CCL compile issue where something like "set rec->item[idx]. = value" does not generate an error.
  The problen is fixed in 8.13.3 
*/
subroutine cclut::listingHasErrors(cclutProgramFileName, cclutListingLocation)
  declare cclutCommand = vc with protect, noconstant("")
  declare cclutCommandLen = i4 with protect, noconstant(0)
  declare cclutStatus = i4 with protect, noconstant(0)
  declare cclutGrepOutputLocation = vc with protect, noconstant("")
  declare cclutReturnValue = i2 with protect, noconstant(TRUE)
  
  if (currev > 8 or (currev = 8 and currevminor > 13) or (currev = 8 and currevminor = 13 and currevminor2 >= 3))
    return (TRUE)
  endif
  
  set cclutProgramFileName = substring(1, findstring(".", cclutProgramFileName, 1)-1, cclutProgramFileName)
  set cclutGrepOutputLocation = concat(cclutProgramFileName, ".chk")
  
  set cclutCommand = concat("grep -v [0-9][0-9]*\) ", cclutListingLocation, " > ", cclutGrepOutputLocation)
  set cclutCommandLen = textlen(cclutCommand)
  call dcl(cclutCommand, cclutCommandLen, cclutStatus)
  
  free define rtl2
  set logical file_location cclutGrepOutputLocation
  define rtl2 is "file_location"
   
  select into "nl:"
      r.line
  from rtl2t r
  detail
    cclutReturnValue = FALSE
  with nocounter
  set cclutStatus = remove(cclutGrepOutputLocation)
  return(cclutReturnValue)
end ;;;listingHasErrors
 
%#endif CCLUT_COMPILE_SUBS_INC