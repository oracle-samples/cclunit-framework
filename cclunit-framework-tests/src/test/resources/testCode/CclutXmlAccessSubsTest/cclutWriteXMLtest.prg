drop program cclutWriteXMLtest go
create program cclutWriteXMLtest

%i cclsource:cclut_xml_access_subs.inc

declare dt_handle = i4 with protect, noconstant(0)

set dt_handle = cclut::parseXMLBuffer(dt_request->xmlBuffer, dt_handle)

set dt_handle = cclut::getXmlListItemHandle(dt_handle, dt_request->rootName, dt_request->index) ;<TEST>

set dt_reply->val = cclut::writeXMLElement(dt_handle)

end go