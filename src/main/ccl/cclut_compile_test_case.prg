drop program cclut_compile_test_case:dba go
create program cclut_compile_test_case:dba
/**
    A prompt program for generating a test program from a test case file.
*/

prompt
    "Output Destination [MINE]: " = "MINE",
    "Test Case Directory Logical [CCLSOURCE]: " = "CCLSOURCE",
    "Test Case File Name: " = ""
with outdev, testCaseDirectoryLogical, testCaseFileName

    if (not validate(_memory_reply_string))
        declare _memory_reply_string = vc
    endif

    record cclutRequest (
        1 testCaseDirectory = vc
        1 testCaseFileName = vc
    ) with protect
    set cclutRequest->testCaseDirectory = $testCaseDirectoryLogical
    if (textlen(trim(cclutRequest->testCaseDirectory)) = 0)
        set cclutRequest->testCaseDirectory = "CCLSOURCE"
    endif
    set cclutRequest->testCaseFileName = $testCaseFileName

    if (textlen(trim(cclutRequest->testCaseFileName)) = 0)
        set _memory_reply_string = "A test case file name must be provided"
        call echo(_memory_reply_string)
        go to exit_script
    endif

    record cclutReply (
        1 testCaseObjectName = vc
%i cclsource:status_block.inc
    ) with protect

    execute cclut_compile_test_case_file
    call echorecord(cclutReply)
    set _memory_reply_string = cnvtrectojson(cclutReply)

#exit_script
end go
