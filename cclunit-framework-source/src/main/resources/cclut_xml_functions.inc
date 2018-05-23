%#ifndef CCLUT_XML_FUNCTIONS_INC
%#define CCLUT_XML_FUNCTIONS_INC
;**********************************************************************************************************************************
;
; Defines XML functions to be used by the CCLUT framework
;
;**********************************************************************************************************************************
 
declare cclut::createXmlElement(cclutElementName = vc, cclutContent = vc) = vc with protect
declare cclut::retrieveXmlContent(
    cclutXml = vc, cclutElementName = vc, cclutStartPos = i4, cclutFoundInd = i2(ref)) = vc with protect
 
 
/**
  Creates an XML element with a given tag name and textual content.
 
  @param cclutElementName
    The name of the XML element to create.
  @param cclutContent
    The textual content for the element. Note that this could itself be an XML elemen.
  @return
    An XML tag with the given name and content
*/
subroutine cclut::createXmlElement(cclutElementName, cclutContent)
  declare cclutXmlTag           = vc with protect, noconstant("")
  declare cclutNormalizedName   = vc with protect, noconstant(trim(cnvtupper(cclutElementName), 3))
  if (cclutContent = null)
    set cclutXmlTag = concat("<", cclutNormalizedName, "></", cclutNormalizedName, ">")
  else
    set cclutXmlTag = concat("<", cclutNormalizedName, ">", cclutContent, "</", cclutNormalizedName, ">")
  endif
  return (cclutXmlTag)
end ;;;createXmlElement
 
/**
  Locates the first XML element with a given name beyond a given starting point within an XML fragment and retrieves its content.
  If the content is enclosed in a CDATA tag, the CDATA tag is removed. The XML tag names must be upper case, but the 
  provided element name need not be.
 
  @param cclutXml
    The XML fragment to be searched.
  @param cclutElementName
    The name of the element to locate.
  @param cclutStartPos
    The position at which to start searching within the XML fragment.
  @param cclutFoundInd
    A buffer for a boolean flag indicating whether the element was found: TRUE if found; FALSE if not found.
  @return
    The content of the identified element if found, "" otherwise.
*/
subroutine cclut::retrieveXmlContent(cclutXml, cclutElementName, cclutStartPos, cclutFoundInd)
  declare cclutOpenTag        = vc with protect, noconstant("")
  declare cclutCloseTag       = vc with protect, noconstant("")
  declare cclutOpenLen        = i4 with protect, noconstant(0)
  declare cclutCloseLen       = i4 with protect, noconstant(0)
  declare cclutOpenCount      = i4 with protect, noconstant(0)
  declare cclutCloseCount     = i4 with protect, noconstant(0)
  declare cclutDataStartPos   = i4 with protect, noconstant(0)
  declare cclutLastOpenPos    = i4 with protect, noconstant(0)
  declare cclutDataClosePos   = i4 with protect, noconstant(0)
  declare cclutElementData    = vc with protect, noconstant("")
  declare cclutNormalizedName = vc with protect, constant(trim(cnvtupper(cclutElementName), 3))

  set cclutOpenTag = concat("<", cclutNormalizedName, ">")
  set cclutCloseTag = concat("</", cclutNormalizedName, ">")
  set cclutOpenLen = textlen(cclutOpenTag)
  set cclutCloseLen = cclutOpenLen + 1
  set cclutLastOpenPos = findstring(cclutOpenTag, cclutXml, cclutStartPos)
  if (cclutLastOpenPos > 0)
    set cclutOpenCount = 1
     ;subsequent searches start at cclutDataClosePos + cclutCloseLen. The -1 accounts for the / that is not in the open tag.
    set cclutDataClosePos = cclutLastOpenPos - 1
    set cclutDataStartPos = cclutLastOpenPos + cclutOpenLen
    while (cclutCloseCount < cclutOpenCount)
      set cclutDataClosePos = findstring(cclutCloseTag, cclutXml, cclutDataClosePos + cclutCloseLen)
      if (cclutDataClosePos > 0)
        set cclutCloseCount = cclutCloseCount + 1
        set cclutLastOpenPos = findString(cclutOpenTag, cclutXml, cclutLastOpenPos + cclutOpenLen) 
        if (cclutLastOpenPos > 0 and cclutLastOpenPos < cclutDataClosePos)
          ;we haven't found the close for cclutDataStartPos yet
          set cclutOpenCount = cclutOpenCount + 1
        endif
      else
        ;there are not as many close tags as there open tags.
        set cclutFoundInd = FALSE
        return ("")
      endif                
    endwhile    
    set cclutFoundInd = TRUE
    set cclutElementData = substring(cclutDataStartPos, cclutDataClosePos-cclutDataStartPos, cclutXml)
    ; Strip the CDATA tag if it's present
    if (cclutElementData = patstring("<![CDATA[*]]>"))
      set cclutElementData = substring(10, textlen(cclutElementData) - 12, cclutElementData)
    endif
    return (cclutElementData)
  endif
  set cclutFoundInd = FALSE
  return ("")
end ;;;cclut::retrieveXmlContent

%#endif CCLUT_XML_FUNCTIONS_INC
