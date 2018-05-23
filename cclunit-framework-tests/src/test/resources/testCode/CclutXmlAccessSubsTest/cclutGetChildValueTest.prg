drop program cclutGetChildValueTest go
create program cclutGetChildValueTest

%i cclsource:cclut_xml_access_subs.inc

declare dt_handle = i4 with protect, noconstant(0)

set stat = alterlist(dt_reply->replys, dt_reply->listSize)

set dt_handle = cclut::parseXMLBuffer(dt_request->xmlBuffer, dt_handle)

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->rootName, dt_request->index) ;<TEST>

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->childName, dt_request->index) ;<TAGS>

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->child2Name, dt_request->index) ;<TAG>

set dt_reply->val = cclut::getXmlChildNodeValue(dt_handle, dt_request->child3Name)

end go