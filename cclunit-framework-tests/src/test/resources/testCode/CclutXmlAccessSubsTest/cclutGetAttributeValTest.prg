drop program cclutGetAttributeValTest go
create program cclutGetAttributeValTest

%i cclsource:cclut_xml_access_subs.inc
%i cclsource:cclut_get_file_as_string.inc

declare dt_file = i4 with protect, noconstant(0)
declare dt_handle = i4 with protect, noconstant(0)
declare cclutHName = i4 with protect, noconstant(0)

set stat = alterlist(dt_reply->replys, dt_reply->listSize)

set dt_file = cclut::parseXMLBuffer(dt_request->xmlBuffer, dt_file)

set dt_file = dt_file

set dt_file = cclut::getXmlListItemHandle(dt_file, dt_request->rootName, dt_request->index) ;<TEST>

set dt_handle = cclut::getXmlListItemHandle(dt_file, dt_request->childName, dt_request->index) ;<TAGS>

set dt_file = cclut::getXmlListItemHandle(dt_handle, dt_request->child2Name, dt_request->index) ;<TAG>

while(dt_file > 0)
    set cclutHName = cclut::getXmlListItemHandle(dt_file, dt_request->child3Name, 1);<VALUE>

    set dt_reply->replys[dt_request->index].val = cclut::getXmlAttributeValue(cclutHName, dt_request->attrName);text

    set dt_request->index = dt_request->index + 1
    set dt_file = cclut::getXmlListItemHandle(dt_handle, dt_request->child2Name, dt_request->index)   
endwhile

#EXIT_TEST

end go