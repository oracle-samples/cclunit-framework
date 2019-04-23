drop program cclut_compile_test_case_file:dba go
create program cclut_compile_test_case_file:dba
/**
    This program will compile a specfied CCL Unit Test Case file and return the compiled object name if successful.
*/

set modify maxvarlen 20000000

/**
  The primary input for the program.

  @request
  @field testCaseDirectory
    (optional) A logical for the directory where the test case file resides, cer_temp by default.
  @field testCaseFileName
    The name of the test case file (.inc) file to be executed.

  record cclutRequest (
    1 testCaseDirectory = vc
    1 testCaseFileName = vc
  ) with protect
*/

/**
  The primary output for the CCL Testing Framework.

  @reply
  @field testCaseObjectName
    The object name of the compiled test case file.

  record cclutReply (
    1 testCaseObjectName = vc
%i cclsource:status_block.inc
  ) with protect
*/


declare cclut::CCLSOURCE                = vc with protect, constant("CCLSOURCE")
declare cclut::CCLUSERDIR               = vc with protect, constant("CCLUSERDIR")
declare cclut::CER_TEMP                 = vc with protect, constant("cer_temp")
declare cclut::outputDirectory          = vc with protect, constant(concat(trim(logical(cclut::CCLUSERDIR), 3), "/"))
declare cclut::testCaseFileName         = vc with protect, noconstant("")
declare cclut::testCaseDirectory        = vc with protect, noconstant("")
declare cclut::testCaseId               = vc with protect, noconstant("")
declare cclut::testCaseObjectName       = vc with protect, noconstant("")
declare cclut::testCaseListingName      = vc with protect, noconstant("")
declare cclut::errorMessage             = vc with protect, noconstant("")
declare cclut::stat                     = i4 with protect, noconstant(0)

%i cclsource:cclut_compile_subs.inc

;create a test program object from the test case file that executes the tests in the test case when it is executed.
set cclut::testCaseId = concat(trim(currdbhandle, 3), "_", trim(cnvtstring(cnvtint(curtime3)), 3))
set cclut::testCaseObjectName = concat("prg_", cclut::testCaseId)
set cclut::testCaseListingName = concat("cclut_inc_", cclut::testCaseId, ".lis")
set cclut::testCaseFileName = trim(cnvtlower(cclutRequest->testCaseFileName), 3)
set cclut::testCaseDirectory = trim(validate(cclutRequest->testCaseDirectory, cclut::CER_TEMP), 3)

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
  set cclutReply->status_data.subeventstatus[1].targetObjectValue = concat("Test case program ",
      cclut::testCaseObjectName," for ",
      cclut::testCaseFileName, " not in CCL dictionary after compilation.")
  go to exit_script
endif

set cclutReply->testCaseObjectName = cclut::testCaseObjectName
set cclutReply->status_data.status = "S"

# exit_script
set cclut::stat = remove(cclut::testCaseListingName)

end go