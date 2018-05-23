drop program cclutGetElementValTest go
create program cclutGetElementValTest

%i cclsource:cclut_xml_access_subs.inc

declare dt_handle = i4 with protect, noconstant(0)

set dt_handle = cclut::parseXMLBuffer(dt_request->xmlBuffer, dt_handle)

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->rootName, dt_request->index) ;<TEST>

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->childName, dt_request->index) ;<TAGS>

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->child2Name, dt_request->index) ;<TAG>

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->child3Name, dt_request->index) ;<Value>

set dt_reply->val = cclut::getXmlElementValue(dt_handle)

end go