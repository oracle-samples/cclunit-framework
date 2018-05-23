drop program cclutXmlTransformTest go
create program cclutXmlTransformTest
%i cclsource:cclut_xml_functions.inc

/*
 * request (
 *  1 xmlValue = vc
 *  1 xmlTagName = vc
 * )
 */

/*
 * reply (
 *  1 xml = vc
 * )
 */

set reply->xml = cclut::createXmlElement(request->xmlTagName, request->xmlValue) 

end go