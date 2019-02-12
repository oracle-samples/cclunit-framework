/**
  This is not really a test, but it will install the discern propmts for cclut and cclut_ff
*/
declare testInstallPrompts(null) = null
subroutine testInstallPrompts(null)
  declare cer_install = vc with protect, noconstant("")
  declare promptFileLocation = vc with protect, noconstant("")
  declare stat = i4 with protect, noconstant(0)
  
  set cer_install = logical("cer_install")
  if (substring(textlen(cer_install)-1, 1, cer_install) != "/")
    set cer_install = concat(cer_install, "/")
  endif

  record promptRequest(
    1 program_name = vc
    1 output_device = vc
    1 params = vc) with protect

  set promptRequest->program_name = "cclut_install_prompt_wrapper"
  set promptRequest->output_device = "noforms"
  
  set promptFileLocation = concat(cer_install, "cclut.dpb")
  set promptRequest->params = build2(^"MINE", "^, promptFileLocation, ^"^)
  set stat = TdbExecute(3202004, 3202004, 3050012, "REC", promptRequest, "REC", promptReply) 

  set promptFileLocation = concat(cer_install, "cclut_ff.dpb")
  set promptRequest->params = build2(^"MINE", "^, promptFileLocation, ^"^)
  set stat = TdbExecute(3202004, 3202004, 3050012, "REC", promptRequest, "REC", promptReply) 
end ;;;testInstallPrompts
