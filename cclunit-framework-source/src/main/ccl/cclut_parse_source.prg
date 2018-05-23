drop program cclut_parse_source:dba go
create program cclut_parse_source:dba
/**
  Converts the output of a compile listing into a list of lines representing the raw source code. 
  The consumer is expected to provide the cclutRequest and cclutReply structures.
*/
 
/**
  @request
  @field programName
    The name of the program that was compiled.
  @field path
    The directory in which the compile listing is located.
  @field filename
    The name of the listing file to be converted.

  record cclutRequest(
    1 programName = vc
    1 path = vc
    1 filename = vc
  ) with protect
*/
 
/**
  @reply
  @field source
    A list of strings representing the compilation output form the requested file.
    @field line
      A single line of the compilation output.

  record cclutReply(
    1 source[*]
      2 line = vc
  )  with protect
*/

declare cclutProgramName     = vc with protect, noconstant("")
declare cclutFileLocation    = vc with protect, noconstant("")
declare cclutCurrLine        = vc with protect, noconstant("")
declare cclutLineNumberText  = vc with protect, noconstant("")
declare cclutLineNumber      = i4 with protect, noconstant(0)
declare cclutUpperLine       = vc with protect, noconstant("")
declare cclutInProgressInd   = i2 with protect, noconstant(FALSE)
declare cclutLineCount       = i4 with protect, noconstant(0)
declare cclutStat            = i4 with protect, noconstant(0)
 
set cclutFileLocation = cnvtlower(build(cclutRequest->path, cclutRequest->filename))
set cclutProgramName = trim(cnvtupper(cclutRequest->programName), 3)
 
free define rtl2
set logical file_location cclutFileLocation
define rtl2 is "file_location"
 
select into "nl:"
    r.line
from rtl2t r
head report
    parenPos = 0
detail
    cclutCurrLine = r.line
    cclutUpperLine = trim(cnvtupper(cclutCurrLine), 3)
 
    ;If the program has not yet been encountered in the output
    if (cclutInProgressInd = FALSE)
        ;If the CREATE PROGRAM line has been encountered, begin parsing the source out
        if (cclutUpperLine = patstring(concat("*CREATE PROGRAM*", cclutProgramName, ":*")) or
                cclutUpperLine = patstring(concat("*CREATE PROGRAM*", cclutProgramName, " *")) or
                cclutUpperLine = patstring(concat("*CREATE PROGRAM*", cclutProgramName)))
           cclutInProgressInd = TRUE
        endif
    endif
 
    if (cclutInProgressInd = TRUE)
        ;If the compilation has been reached, terminate
        if (cclutUpperLine = patstring("COMMAND EXECUTED!*"))
            cclutInProgressInd = FALSE
        else
            ;The expected format is: ####) <source>
            ;Find where the line number begins and take everything after that
            parenPos = findstring(")", cclutCurrLine)
            if (parenPos > 0)
                cclutLineNumberText = substring(1, parenPos - 1, cclutCurrLine)
                ;If a number was found in front of the parenthesis, pad out the cclutReply record structure
                ;to that number and then set the last line in the cclutReply structure to the current line
                if (isnumeric(cclutLineNumberText) = TRUE)
                    ;If the line number is somehow less than the current number of recorded lines,
                    ;something is wrong - exit out
                    cclutLineNumber = cnvtint(cclutLineNumberText)
                    if (cclutLineNumber < cclutLineCount)
                        cclutInProgressInd = FALSE
                    else
                        cclutLineCount = cclutLineNumber
                        cclutStat = alterlist(cclutReply->source, cclutLineCount)
                        cclutReply->source[cclutLineCount].line = substring(parenPos + 1, textlen(cclutCurrLine), cclutCurrLine)
                    endif
                endif
            endif
        endif
    endif
with nocounter

end go