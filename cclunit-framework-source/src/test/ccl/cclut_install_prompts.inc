/**
  This is not really a test, but it will install the discern propmts for cclut and cclut_ff
*/
declare testInstallPrompts(null) = null
subroutine testInstallPrompts(null) 
  declare cer_install = vc with protect, noconstant("")
  declare promptFileLocation = vc with protect, noconstant("")
  
  set cer_install = logical("cer_install")
  if (substring(textlen(cer_install)-1, 1, cer_install) != "/")
    set cer_install = concat(cer_install, "/")
  endif
  
  set trace deprecated W
  set promptFileLocation = concat(cer_install, "cclut.dpb")
  execute ccl_prompt_importform promptFileLocation

  set promptFileLocation = concat(cer_install, "cclut_ff.dpb")
  execute ccl_prompt_importform promptFileLocation  
end ;;;testInstallPrompts
