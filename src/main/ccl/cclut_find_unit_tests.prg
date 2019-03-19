drop program cclut_find_unit_tests:dba go
create program cclut_find_unit_tests:dba
/**
  Discovers the unit tests contained in a given [test case] program, where 'unit test' means any subroutine whose name
  starts with "test". Parameters and return values not not checked. All subroutines with name matching "test*" are returned.
  The consumer is expected to provide the cclutRequest and cclutReply structures.
  */


%i cclsource:cclut_get_file_as_string.inc
%i cclsource:cclut_xml_access_subs.inc


declare cclut::findUnitTests(cclutProgramName = vc, cclutUnitTests = vc(ref)) = i2 with protect


/**
  @request
  @field programName
    The name of the program to be scanned for unit tests.
*/
/*
record cclutRequest (
  1 programName = vc
) with protect
*/

/**
  @reply
  @field tests
  A list of tests that were discovered within the given program.
  @field testName
    The name of the discovered test.

  record cclutReply (
    1 tests[*]
      2 testName = vc
  %i cclsource:status_block.inc
  ) with protect
*/


/**
  Identifies all subrouties of a CCL program whose name begins with "test".

  @param cclutProgramName
    The name of the program that contains the unit tests.
  @param cclutUnitTests
    A return buffer for the identified list of subroutines.
  @returns
    Returns FALSE if an error occurred or TRUE if successful
*/
subroutine cclut::findUnitTests(cclutProgramName, cclutUnitTests)
  declare cclutXmlFileName       = vc with protect, constant(concat(trim(cnvtlower(cclutProgramName), 3), ".xml"))
  declare cclutPathToXmlFile     = vc with protect, constant(concat(trim(logical("ccluserdir"), 3), "/", cclutXmlFileName))
  declare cclutProgramXml        = vc with protect, noconstant("")
  declare cclutHXmlRoot          = h with protect, noconstant(0)
  declare cclutHXmlFile          = h with protect, noconstant(0)
  declare cclutUnitTestCount     = i4 with protect, noconstant(0)
  declare cclutHProgram          = h with protect, noconstant(0)
  declare cclutHSubroutine       = h with protect, noconstant(0)
  declare cclutHName             = h with protect, noconstant(0)
  declare cclutHNamespace        = h with protect, noconstant(0)
  declare cclutSubroutineName    = vc with protect, noconstant("")
  declare cclutNamespacedName    = vc with protect, noconstant("")
  declare cclutSubroutineCount   = i4 with protect, noconstant(1)
  declare cclutErrorMessage      = vc with protect, noconstant("")
  declare cclStat                = i4 with protect, noconstant(0)

  call parser(concat('translate into "ccluserdir:', cclutXmlFileName, '"', cclutProgramName, ' with xml go'))

  set cclutProgramXml = cclut::getFileAsString(cclutPathToXmlFile)

  if (textlen(trim(cclutProgramXml)) = 0)
    set cclutErrorMessage = build2("Failed to translate program. Listing file ccluserdir:", cclutXmlFileName, " is empty.")
    set cclutReply->status_data.subeventstatus[1].operationName = "TRANSLATE"
    set cclutReply->status_data.subeventstatus[1].operationStatus = "F"
    set cclutReply->status_data.subeventstatus[1].targetObjectName = cclutProgramName
    set cclutReply->status_data.subeventstatus[1].targetObjectValue = cclutErrorMessage
    return (FALSE)
  endif

  set cclStat = remove(cclutXmlFileName)

  set cclutHXmlRoot = cclut::parseXmlBuffer(cclutProgramXml, cclutHXmlFile)
  ;note that cclut::parseXmlBuffer is more likely to hang than return zeros if the xml is bad.
  if (cclutHXmlRoot = 0 or cclutHXmlFile = 0)
    set cclutErrorMessage = build2("Failed to parse xml from listing file ccluserdir:", cclutXmlFileName, ".")
    set cclutReply->status_data.subeventstatus[1].operationName = "PARSE"
    set cclutReply->status_data.subeventstatus[1].operationStatus = "F"
    set cclutReply->status_data.subeventstatus[1].targetObjectName = cclutProgramName
    set cclutReply->status_data.subeventstatus[1].targetObjectValue = cclutErrorMessage
    return (FALSE)
  endif

  set cclutHProgram = cclut::getXmlListItemHandle(cclutHXmlRoot, "ZC_PROGRAM.", 1)
  set cclutHSubroutine = cclut::getXmlListItemHandle(cclutHProgram, "SUBROUTINE.", cclutSubroutineCount)

  while (cclutHSubroutine != 0)
    set cclutSubroutineName = ""
    set cclutHNamespace = cclut::getXmlListItemHandle(cclutHSubroutine, "NAMESPACE.", 1)
    if (cclutHNamespace != 0)
      set cclutHName = cclut::getXmlListItemHandle(cclutHNamespace, "NAME", 1)
      set cclutNamespacedName = cclut::getXmlAttributeValue(cclutHName, "text")
      set cclutHName = cclut::getXmlListItemHandle(cclutHNamespace, "NAME", 2)
      set cclutSubroutineName = cclut::getXmlAttributeValue(cclutHName, "text")
      set cclutNamespacedName = concat(cclutNamespacedName, "::", cclutSubroutineName)
    else
      set cclutHName = cclut::getXmlListItemHandle(cclutHSubroutine, "NAME", 1)
      if (cclutHName != 0)
        set cclutSubroutineName = cclut::getXmlAttributeValue(cclutHName, "text")
        set cclutNamespacedName = cclutSubroutineName
      endif
    endif

    if (substring(1, 4, cclutSubroutineName) = "TEST")
      set cclutUnitTestCount = cclutUnitTestCount + 1
      set cclStat = alterlist(cclutUnitTests->tests, cclutUnitTestCount)
      set cclutUnitTests->tests[cclutUnitTestCount].testName = cclutNamespacedName
    endif
    set cclutSubroutineCount = cclutSubroutineCount + 1
    set cclutHSubroutine = cclut::getXmlListItemHandle(cclutHProgram, "SUBROUTINE.", cclutSubroutineCount)
  endwhile

  call cclut::releaseXmlResources(cclutHXmlFile)
  return (TRUE)
end ;;;findUnitTests


if (cclut::findUnitTests(cclutRequest->programName, cclutReply))
  set cclutReply->status_data.status = "S"
else
  set cclutReply->status_data.status = "F"
endif


#exit_script

if (validate(cclut::debug, FALSE) = TRUE)
  call echorecord(cclutRequest) ;intentional
  call echorecord(cclutReply) ;intentional
endif

end go
