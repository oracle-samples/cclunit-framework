%#ifndef CCLUT_GET_FILE_AS_STRING_INC
%#def CCLUT_GET_FILE_AS_STRING_INC

;**********************************************************************************************************************************
;
; Defines the cclut::getFileAsString subroutine used to obtain the contents of a file as a string.
;
;**********************************************************************************************************************************
 
if (validate(CCLUT_GET_FILE_AS_STRING_INC) = FALSE)
  declare CCLUT_GET_FILE_AS_STRING_INC = i2 with protect, constant(TRUE)
  declare uar_fopen(p1=vc(ref), p2=vc(ref)) = h
    with image_axp="decc$shr", uar_axp="decc$fopen", image_aix="libc.a(shr.o)",  uar_aix="fopen"
  declare uar_fread(p1=vc(ref), p2=i4(value), p3=i4(value), p4=i4(value)) = i4
    with image_axp="decc$shr", uar_axp="decc$fread", image_aix="libc.a(shr.o)", uar_aix="fread"
  declare uar_fseek(p1=i4(value), p2=i4(value), p3=i4(value)) = i4
    with image_axp="decc$shr", uar_axp="decc$fseek", image_aix="libc.a(shr.o)", uar_aix="fseek"
  declare uar_ftell(p1=i4(value)) = i4
    with image_axp="decc$shr", uar_axp="decc$ftell", image_aix="libc.a(shr.o)", uar_aix="ftell"
  declare uar_fclose(p1=i4(value)) = i4
    with image_axp="decc$shr", uar_axp="decc$fclose", image_aix="libc.a(shr.o)", uar_aix="fclose"
endif

set modify maxvarlen 10000000


/**
  Retrieves the contents of a file as a vc.
  @param  cclutFullyQualifiedFileName  
    The fully qualified name for the file to retrieve (direcotry + fileName).
  @returns
    The file contents as a string if successful, "" otherwise.
*/
declare cclut::getFileAsString(cclutFullyQualifiedFileName  = vc) = vc with protect
subroutine cclut::getFileAsString(cclutFullyQualifiedFileName )
  declare CCLUT_SEEK_SET = i4 with protect, constant(0)
  declare CCLUT_SEEK_END = i4 with protect, constant(2)

  declare cclutReturnValue = vc with protect, noconstant(nullterm(""))
  declare cclutFileHandle = i4 with protect, noconstant(0)
  declare cclutSize = i4 with protect, noconstant(0)
  declare cclutOriginalSize = i4 with protect, noconstant(0)
  declare cclutAdditionalSize = i4 with protect, noconstant(1)
  declare cclutDone = i2 with protect, noconstant(FALSE)
  declare cclutStat = i4 with protect, noconstant(0)
  declare vcMore = c10000 with protect
  
  set cclutFileHandle = uar_fopen(nullterm(cclutFullyQualifiedFileName ), "r")

  if (cclutFileHandle != 0)
    set cclutStat = uar_fseek(cclutFileHandle, 0, CCLUT_SEEK_END)
    set cclutSize = uar_ftell(cclutFileHandle) + 512;
    set cclutOriginalSize = cclutSize

    ; fillstring limits to 65536 so we must use concat to create a buffer > 65536
    while (cclutDone = FALSE)
      if (cclutSize > 65536)
        set cclutReturnValue =
        notrim(concat(notrim(cclutReturnValue), notrim(fillstring(value(65536), ""))))
        set cclutSize = cclutSize - 65536
      else
        set cclutReturnValue =
        notrim(concat(notrim(cclutReturnValue), notrim(fillstring(value(cclutSize), ""))))
        set cclutDone = TRUE
      endif
    endwhile

    set cclutStat = uar_fseek(cclutFileHandle, 0, CCLUT_SEEK_SET)
    set cclutSize = uar_fread(cclutReturnValue, 1, cclutOriginalSize, cclutFileHandle)

    while (cclutAdditionalSize > 0 and cursys = "AXP")
      set vcMore = notrim(fillstring(10000, ""))
      set cclutAdditionalSize = uar_fread(vcMore, 1, 10000, cclutFileHandle)
      if (cclutAdditionalSize)
        set cclutOriginalSize = cclutOriginalSize + cclutAdditionalSize
        set cclutReturnValue = notrim(concat(notrim(cclutReturnValue), notrim(vcMore)))
      endif
    endwhile

    set cclutStat = uar_fclose(cclutFileHandle)

    if (cclutStat = 0)
      if (cclutOriginalSize >= 100000000)
        set cclutReturnValue = nullterm("")
      endif
    else
      set cclutReturnValue = nullterm("")
    endif
  else
    set cclutReturnValue = nullterm("")
  endif

  return (cclutReturnValue)
end ;;;cclut::getFileAsString

%#endif CCLUT_GET_FILE_AS_STRING_INC
 