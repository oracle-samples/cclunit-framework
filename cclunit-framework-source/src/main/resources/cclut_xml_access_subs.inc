%#ifndef CCLUT_XML_ACCESS_SUBS_INC
%#def CCLUT_XML_ACCESS_SUBS_INC
;**********************************************************************************************************************************
;
; Provides subroutines for accessing and traversing xml structures.
;
;**********************************************************************************************************************************

;------------------------------  XML Access Subs  ---------------------------------
if (validate(uar_xml_readfile) = FALSE)
  declare uar_xml_readfile(source=vc, fileHandle=i4(ref))=i4
endif
if (validate(uar_xml_closefile) = FALSE)
  declare uar_xml_closefile(fileHandle=i4(ref))=null
endif
if (validate(uar_xml_geterrormsg) = FALSE)
  declare uar_xml_geterrormsg(errorCode=i4(ref))=vc
endif
if (validate(uar_xml_listtree) = FALSE)
  declare uar_xml_listtree(fileHandle=i4(ref))=vc
endif
if (validate(uar_xml_getroot) = FALSE)
  declare uar_xml_getroot(fileHandle=i4(ref), nodeHandle=i4(ref))=i4
endif
if (validate(uar_xml_findchildnode) = FALSE)
  declare uar_xml_findchildnode(nodeHandle=i4(ref), nodeName=vc, childHandle=i4(ref))=i4
endif
if (validate(uar_xml_getchildcount) = FALSE)
  declare uar_xml_getchildcount(nodeHandle=i4(ref)) = i4
endif
if (validate(uar_xml_getchildnode) = FALSE)
  declare uar_xml_getchildnode(nodeHandle=i4(ref), nodeNO=i4(ref), childNode=i4(ref))=i4
endif
if (validate(uar_xml_getparentnode) = FALSE)
  declare uar_xml_getparentnode(nodeHandle=i4(ref), parentNode=i4(ref))=i4
endif
if (validate(uar_xml_getnodename) = FALSE)
  declare uar_xml_getnodename(nodeHandle=i4(ref))=vc
endif
if (validate(uar_xml_getnodecontent) = FALSE)
  declare uar_xml_getnodecontent(nodeHandle=i4(ref))=vc
endif
if (validate(uar_xml_getattrbyname) = FALSE)
  declare uar_xml_getattrbyname(nodeHandle=i4(ref), attrName=vc, attributeHandle=i4(ref))=i4
endif
if (validate(uar_xml_getattrbypos) = FALSE)
  declare uar_xml_getattrbypos(nodeHandle=i4(ref), ndx=i4(ref), attributeHandle=i4(ref))=i4
endif
if (validate(uar_xml_getattrname) = FALSE)
  declare uar_xml_getattrname(attributeHandle=i4(ref))=vc
endif
if (validate(uar_xml_getattrvalue) = FALSE)
  declare uar_xml_getattrvalue(attributeHandle=i4(ref))=vc
endif
if (validate(uar_xml_getattributevalue) = FALSE)
  declare uar_xml_getattributevalue(nodeHandle=i4(ref), attrName=vc)=vc
endif
if (validate(uar_xml_getattrcount) = FALSE)
  declare uar_xml_getattrcount(nodeHandle=i4(ref))=i4
endif
if (validate(uar_xml_parsestring) = FALSE)
  declare uar_xml_parsestring(xmlString=vc, fileHandle=i4(ref))=i4
endif

declare cclut::parseXmlBuffer(cclutPXMLBuffer = vc, cclutPRXMLFileHandle = i4(ref)) = i4 with protect
declare cclut::releaseXmlResources(cclutPRXMLFileHandle = i4(ref)) = i2 with protect
declare cclut::getXmlChildNodeValue(cclutPParentHandle = i4, cclutPChildName = vc) = vc with protect
declare cclut::getXmlAttributeValue(cclutPElementHandle = i4, cclutPAttrName = vc) = vc with protect
declare cclut::getXmlChildNodeAttributeValue(cclutPParentHandle = i4, cclutPChildName = vc, cclutPAttrName = vc) = vc with protect
declare cclut::writeXmlElement(cclutHElement = i4) = vc with protect
declare cclut::getXmlListItemHandle(cclutPElementHandle = i4, cclutPChildName = vc, cclutPOccurrenceIndex = i4) = i4 with protect
declare cclut::getXmlElementValue(cclutPElementHandle = i4) = vc with protect
declare cclut::evaluateXmlXPath(cclutHXMLContext = i4, cclutXPathExpr = vc) = vc with protect

;------------------------------  XML Status Codes  ---------------------------------
if (not(validate(CCLUT_XML_SC_UNKSTAT)))    declare CCLUT_XML_SC_UNKSTAT           = i4 with protect, constant(0)  endif
if (not(validate(CCLUT_XML_SC_OK)))         declare CCLUT_XML_SC_OK                = i4 with protect, constant(1)  endif
if (not(validate(CCLUT_XML_SC_PARSERROR)))  declare CCLUT_XML_SC_PARSERROR         = i4 with protect, constant(2)  endif
if (not(validate(CCLUT_XML_SC_NOFILE)))     declare CCLUT_XML_SC_NOFILE            = i4 with protect, constant(3)  endif
if (not(validate(CCLUT_XML_SC_NONODE)))     declare CCLUT_XML_SC_NONODE            = i4 with protect, constant(4)  endif
if (not(validate(CCLUT_XML_SC_NOATTR)))     declare CCLUT_XML_SC_NOATTR            = i4 with protect, constant(5)  endif
if (not(validate(CCLUT_XML_SC_BADOBJREF)))  declare CCLUT_XML_SC_BADOBJREF         = i4 with protect, constant(6)  endif
if (not(validate(CCLUT_XML_SC_INVINDEX)))   declare CCLUT_XML_SC_INVINDEX          = i4 with protect, constant(7)  endif
if (not(validate(CCLUT_XML_SC_NOTFOUND)))   declare CCLUT_XML_SC_NOTFOUND          = i4 with protect, constant(8)  endif

 
/**
  This routine takes in an XML String Buffer and parses it. It returns in the cclutPRXMLFileHandle parameter 
  the handle to the file that is needed to free resources.
  @param cclutPXMLBuffer
    The XML string to be parsed.
  @param cclutPRXMLFileHandle
    The XML handle to the XML File.
  @returns
    The XML handle to the root of the XML tree.
*/
subroutine cclut::parseXmlBuffer(cclutPXMLBuffer, cclutPRXMLFileHandle)
  set cclutPRXMLFileHandle = 0
  if (uar_xml_parsestring(nullterm(cclutPXMLBuffer), cclutPRXMLFileHandle) != CCLUT_XML_SC_OK)
    return (0)
  endif

  declare cclutHXMLRoot  = i4 with protect, noconstant(0)
  if (uar_xml_getroot(cclutPRXMLFileHandle, cclutHXMLRoot) != CCLUT_XML_SC_OK)
    return (0)
  endif

  return (cclutHXMLRoot)
end ;;;cclut::parseXmlBuffer

/**
  Releases the resourses used with an XML file handle.
  @param cclutPRXMLFileHandle
    The XML file handle.
  @returns
    TRUE
*/
subroutine cclut::releaseXmlResources(cclutPRXMLFileHandle)
  call uar_xml_closefile(cclutPRXMLFileHandle)
  return (TRUE)
end ;;;cclut::releaseXmlResources

/**
  Retrieves the value of an XML child node specified by name for an XML element. Embedded escape values
  are converted back to the original form.
  @param cclutPParentHandle 
    A handle to the parent XML element.
  @param cclutPChildName 
    The name of the child node.
  @returns
    The value of the child node if found; "" otherwise.
*/ 
subroutine cclut::getXmlChildNodeValue(cclutPParentHandle, cclutPChildName)
  if (cclutPParentHandle = 0)
    return (nullterm(""))
  endif

  set hItem = 0
  if (uar_xml_findchildnode(cclutPParentHandle, nullterm(cclutPChildName), hItem) != CCLUT_XML_SC_OK)
    return (nullterm(""))
  endif
  declare cclutTmpString = vc with protect, noconstant("")
  set cclutTmpString = nullterm(uar_xml_getnodecontent(hItem))
  set cclutTmpString = replace(cclutTmpString, "&apos;", Char(39), 0)
  set cclutTmpString = replace(cclutTmpString, "&quot;", Char(34), 0)
  set cclutTmpString = replace(cclutTmpString, "&gt;", ">", 0)
  set cclutTmpString = replace(cclutTmpString, "&lt;", "<", 0)
  set cclutTmpString = replace(cclutTmpString, "&amp;", "&", 0)
  return (nullterm(cclutTmpString))
end ;;;cclut::getXmlChildNodeValue

/**
  Retrieves the value of an XML attribute specified by name for an XML element. Embedded escape values
  are converted back to the original form.
  @param cclutPElementHandle
    A handle to the XML element.
  @param cclutPAttrName
    The name of the attribute.
  @returns
    The attribue value if found; "" otherwise.
*/
subroutine cclut::getXmlAttributeValue(cclutPElementHandle, cclutPAttrName)
  declare cclutTmpString = vc with protect, noconstant("")
  if (cclutPElementHandle != 0)
    set cclutTmpString = nullterm(uar_xml_getattributevalue(cclutPElementHandle, nullterm(cclutPAttrName)))
    set cclutTmpString = replace(cclutTmpString, "&apos;", Char(39), 0)
    set cclutTmpString = replace(cclutTmpString, "&quot;", Char(34), 0)
    set cclutTmpString = replace(cclutTmpString, "&gt;", ">", 0)
    set cclutTmpString = replace(cclutTmpString, "&lt;", "<", 0)
    set cclutTmpString = replace(cclutTmpString, "&amp;", "&", 0)
  endif
  return (nullterm(cclutTmpString))
end ;;;cclut::getXmlAttributeValue

/**
  Retrieves the value of an XML attribute specied by name of an XML child element specified by name of an XML element. 
  Embedded escape values are converted back to the original form.
  @param cclutPParentHandle
    A handle to the XML parent element
  @param cclutPChildName
    The name of the child element.
  @param cclutPAttrName
    The name of the attribute.
  @return
    The value of the specified attribute from the specified child node if found; "" otherwise.
*/
subroutine cclut::getXmlChildNodeAttributeValue(cclutPParentHandle, cclutPChildName, cclutPAttrName)
  if (cclutPParentHandle = 0)
    return (nullterm(""))
  endif

  set hItem = 0
  if (uar_xml_findchildnode(cclutPParentHandle, nullterm(cclutPChildName), hItem) != CCLUT_XML_SC_OK)
    return (nullterm(""))
  endif

  declare cclutTmpString = vc with protect, noconstant("")
  set cclutTmpString = nullterm(uar_xml_getattributevalue(hItem, nullterm(cclutPAttrName)))
  set cclutTmpString = replace(cclutTmpString, "&apos;", Char(39), 0)
  set cclutTmpString = replace(cclutTmpString, "&quot;", Char(34), 0)
  set cclutTmpString = replace(cclutTmpString, "&gt;", ">", 0)
  set cclutTmpString = replace(cclutTmpString, "&lt;", "<", 0)
  set cclutTmpString = replace(cclutTmpString, "&amp;", "&", 0)
  return (nullterm(cclutTmpString))
end ;;;cclut::getXmlChildNodeAttributeValue

/**
  Retrieves the string representation for an XML element.
  @param cclutHElement
    A handle for the XML element.
  @return
    The string representation of the element.
*/
subroutine cclut::writeXmlElement(cclutHElement)
  declare cclutChildIdx          = i4 with protect, noconstant(0)
  declare cclutChildCnt          = i4 with protect, noconstant(uar_xml_getchildcount(cclutHElement))
  declare cclutAttrIdx           = i4 with protect, noconstant(0)
  declare cclutAttrCnt           = i4 with protect, noconstant(uar_xml_getattrcount(cclutHElement))
  declare cclutMyContent         = vc with protect, noconstant(nullterm(uar_xml_getnodecontent(cclutHElement)))
  declare cclutHAttr             = i4 with protect, noconstant(0)
  declare cclutAttrName          = vc with protect, noconstant("")
  declare cclutAttrValue         = vc with protect, noconstant("")
  declare cclutTheElementString  = vc with protect, 
      noconstant(nullterm(concat(nullterm("<"), nullterm(uar_xml_getnodename(cclutHElement)))))
  declare cclutTheChildElementString = vc with protect, noconstant(nullterm(""))

  if ((cclutAttrCnt = 0) and (cclutChildCnt = 0) and (cclutMyContent = ""))
    return (nullterm(concat(cclutTheElementString, "/>")))
  endif

  for (cclutAttrIdx = 1 to cclutAttrCnt)
    call uar_xml_getattrbypos(cclutHElement, cclutAttrIdx - 1, cclutHAttr)
    set cclutAttrName nullterm(uar_xml_getattrname(cclutHAttr))
    set cclutAttrValue = nullterm(uar_xml_getattrvalue(cclutHAttr))
    set cclutTheElementString = concat(cclutTheElementString, ' ', cclutAttrName, '="', cclutAttrValue, '"')
  endfor

  if ((cclutChildCnt = 0) and (cclutMyContent = ""))
    return (concat(cclutTheElementString, "/>"))
  elseif (cclutChildCnt = 0)
    return (nullterm(concat(cclutTheElementString, ">", cclutMyContent, "</", nullterm(uar_xml_getnodename(cclutHElement)), ">")))
  endif

  set cclutTheElementString = concat(cclutTheElementString, ">", cclutMyContent)
  for (cclutChildIdx = 1 to cclutChildCnt)
    declare cclutHChildNode = i4 with protect, noconstant(0)
    call uar_xml_getchildnode(cclutHElement, cclutChildIdx - 1, cclutHChildNode)
    set cclutTheChildElementString = cclut::writeXmlElement(cclutHChildNode)
    set cclutTheElementString = concat(cclutTheElementString, cclutTheChildElementString)
  endfor

  return (nullterm(concat(cclutTheElementString, "</", nullterm(uar_xml_getnodename(cclutHElement)), ">")))
end ;;;cclut::writeXmlElement

/**
  Retrieves a handle to a named XML element at a specified position within an XML list.
  @param cclutPElementHandle
    A handle to the XML list element.
  @param cclutPChildName
    The name of the child element to be retrieved.
  @param cclutPOccurrenceIndex
    The one-based index of the position in the XML list from which to retrieve the XML element.
  @returns
    A handle to the indicated XML element if found; 0 otherwise.
*/
subroutine cclut::getXmlListItemHandle(cclutPElementHandle, cclutPChildName, cclutPOccurrenceIndex)
  if (cclutPElementHandle != 0)
    declare cclutChIdx       = i4 with protect, noconstant(0)
    declare cclutChCnt       = i4 with protect, noconstant(0)
    declare cclutOsNumber    = i4 with protect, noconstant(0)
    declare cclutTmpNode     = i4 with protect, noconstant(0)

    set cclutChCnt = uar_xml_getchildcount(cclutPElementHandle)

    for (cclutChIdx = 1 to cclutChCnt)
      set cclutTmpNode = 0
      if (uar_xml_getchildnode(cclutPElementHandle, cclutChIdx - 1, cclutTmpNode) != CCLUT_XML_SC_OK)
        return (0)
      endif

      if (uar_xml_getnodename(cclutTmpNode) = cclutPChildName)
        set cclutOsNumber = cclutOsNumber + 1
        if (cclutOsNumber = cclutPOccurrenceIndex)
          return (cclutTmpNode)
        endif
      endif
    endfor
  endif
  return (0)
end ;cclut::getXmlListItemHandle
 
/**
  Retrieves the value of an XML element. Embedded escape values are converted back to the original form. 
  @param cclutPElementHandle
    A handle to the XML element.
  @returns
    The value of the element if found; "" otherwise.
*/
subroutine cclut::getXmlElementValue(cclutPElementHandle)
  declare cclutTmpString = vc with protect, noconstant("")
  if (cclutPElementHandle != 0)
    set cclutTmpString = nullterm(uar_xml_getnodecontent(cclutPElementHandle))
    set cclutTmpString = replace(cclutTmpString, "&apos;", Char(39), 0)
    set cclutTmpString = replace(cclutTmpString, "&quot;", Char(34), 0)
    set cclutTmpString = replace(cclutTmpString, "&gt;", ">", 0)
    set cclutTmpString = replace(cclutTmpString, "&lt;", "<", 0)
    set cclutTmpString = replace(cclutTmpString, "&amp;", "&", 0)
  endif
  return (nullterm(cclutTmpString))
end ;cclut::getXmlElementValue
 
/**
  Evaluates a given XPath expression from a given XML context element.  
  Embedded escape values are converted back to the original form.
  <pre>
  Currently this function only supports a limited XPath expression set:
    1. /claimData/charges/charge[4]/@id
    2. /lineItems/lineItem/revenueCode
    3. @meaning
  </pre>
  
  @param cclutHXMLContext
    A handle to the context element from which the XPath expression is evaluated.
  @param cclutXPathExpr
    The XPath expression to evaluate. 
  @returns
    The evaluation of the XPath expression.
*/
subroutine cclut::evaluateXmlXPath(cclutHXMLContext, cclutXPathExpr)
  declare cclutExpr          = vc with protect, noconstant(trim(cclutXPathExpr))
  declare cclutExprLen       = i4 with protect, noconstant(size(cclutExpr))
  declare cclutHXMLParent    = i4 with protect, noconstant(cclutHXMLContext)

  ;hXMLElement is always the root of the XPath expression, so just add the root '/' if it doesn't exist
  if (substring(1, 1, cclutExpr) != "/")
    set cclutExpr    = concat("/", cclutExpr)
    set cclutExprLen = cclutExprLen + 1
  endif

  declare cclutFirstSlashPos   = i4 with protect, noconstant(1)
  declare cclutSecondSlashPos  = i4 with protect, noconstant(findstring("/", cclutExpr, 2))
  declare cclutBetweenSlashes  = vc with protect, noconstant("")
  declare cclutOpenBracketPos  = i4 with protect, noconstant(0)
  declare cclutCloseBracketPos = i4 with protect, noconstant(0)
  declare cclutBetweenBrackets = vc with protect, noconstant("")

  while (cclutSecondSlashPos != 0)
    set cclutBetweenSlashes  = substring(cclutFirstSlashPos + 1, cclutSecondSlashPos - cclutFirstSlashPos - 1, cclutExpr)

    set cclutOpenBracketPos = findstring("[", cclutBetweenSlashes)
    if (cclutOpenBracketPos > 0)
      ;This element is a list so we need to determine what index they requested and if it exists...
      set cclutCloseBracketPos = findstring("]", cclutBetweenSlashes)
      set cclutBetweenBrackets = 
          substring(cclutOpenBracketPos + 1, cclutCloseBracketPos - cclutOpenBracketPos - 1, cclutBetweenSlashes)
      set cclutBetweenSlashes  = substring(1, cclutOpenBracketPos - 1, cclutBetweenSlashes)

      if (cclutCloseBracketPos = 0)
        call logMsg("cclut::evaluateXmlXPath", concat("Expected ']' for element '", cclutBetweenSlashes, "'"), LOG_ERROR)
        return ("")
      elseif ((cclutBetweenBrackets = "") or (not (isNumeric(cclutBetweenBrackets))))
        call logMsg("cclut::evaluateXmlXPath", concat("Expected ( [0-9]+ ) between bracket for element '", 
                    cclutBetweenSlashes, "'"), LOG_ERROR)
        return ("")
      endif

      set cclutHXMLParent = cclut::getXmlListItemHandle(cclutHXMLParent, trim(cclutBetweenSlashes), cnvtint(cclutBetweenBrackets))
    else
      set cclutHXMLParent = cclut::getXmlListItemHandle(cclutHXMLParent, trim(cclutBetweenSlashes), 1)
    endif

    set cclutExpr            = substring(cclutSecondSlashPos, cclutExprLen, cclutExpr)
    set cclutExprLen         = size(cclutExpr)
    set cclutFirstSlashPos   = 1
    set cclutSecondSlashPos  = findstring("/", cclutExpr, cclutFirstSlashPos + 1)
  endwhile

  ;Remove the leading slash
  set cclutExpr    = substring(2, cclutExprLen, cclutExpr)
  set cclutExprLen = cclutExprLen - 1

  ;At this point we have no more slashes... so cclutExpr must contain the value we want to get
  ;and cclutHXMLParent must be the handle of the parent element. So now just get the value
  if (substring(1, 1, cclutExpr) = "@")
    set cclutExpr    = substring(2, cclutExprLen, cclutExpr)
    set cclutExprLen = cclutExprLen - 1
    return (cclut::getXmlAttributeValue(cclutHXMLParent, trim(cclutExpr)))
  endif

  return (cclut::getXmlChildNodeValue(cclutHXMLParent, trim(cclutExpr)))
end ;cclut::evaluateXmlXPath

%#endif CCLUT_XML_ACCESS_SUBS_INC