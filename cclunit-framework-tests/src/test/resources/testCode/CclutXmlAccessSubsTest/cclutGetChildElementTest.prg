drop program cclutGetChildElementTest go
create program cclutGetChildElementTest

%i cclsource:cclut_xml_access_subs.inc

declare dt_file = i4 with protect, noconstant(0)

set dt_request->elementHandle = cclut::parseXMLBuffer(dt_request->xmlBuffer, dt_file)

set dt_reply->handle = cclut::getXmlListItemHandle(dt_request->elementHandle, dt_request->childName, dt_request->index)

end go