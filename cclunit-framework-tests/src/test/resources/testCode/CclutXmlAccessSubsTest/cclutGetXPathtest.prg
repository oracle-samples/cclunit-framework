drop program cclutGetXPathtest go
create program cclutGetXPathtest

%i cclsource:cclut_xml_access_subs.inc

declare dt_file = i4 with protect, noconstant(0)

set dt_file = cclut::parseXMLBuffer(dt_request->xmlBuffer, dt_file)

set dt_reply->val = cclut::evaluateXmlXPath(dt_file, dt_request->expr)

end go
