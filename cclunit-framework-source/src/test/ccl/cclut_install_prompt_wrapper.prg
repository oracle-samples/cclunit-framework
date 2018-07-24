drop program cclut_install_prompt_wrapper go
create program cclut_install_prompt_wrapper 
/**
  Used to invoke ccl_prompt_importform as a prompt program with an output device.
*/
  prompt 
    "output device" = "MINE",
    "prompt file location" = ""
  with outdev, promptFileLocation
  
  execute ccl_prompt_importform value($promptFileLocation)
  set _memory_reply_string = "done"
end go
