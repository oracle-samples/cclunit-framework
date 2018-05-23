drop program cclutParseXmlTest go
create program cclutParseXmlTest
%i cclsource:cclut_xml_functions.inc

/*
 * request (
 *  1 xml = vc
 *  1 xmlTagName = vc
 *  1 startPos = i4
 * )
 */

/*
 * reply (
 *  1 xmlValue = vc
 *  1 foundInd = i2
 * )
 */

declare cpxt_foundInd = i2 with protect, noconstant(0)

set reply->xmlValue = cclut::retrieveXmlContent(request->xml, request->xmlTagName, request->startPos, cpxt_foundInd)
set reply->foundInd = cpxt_foundInd

end go
