drop program cclutParseBufferTest go
create program cclutParseBufferTest

%i cclsource:cclut_xml_access_subs.inc

declare fileHandle = i4 with protect, noconstant(0)

set fileHandle = dt_request->fileHandle

set dt_reply->root = cclut::parseXMLBuffer(dt_request->buffer, fileHandle)

set dt_request->fileHandle = fileHandle

end go
