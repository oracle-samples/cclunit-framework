/**
  Unit Tests for cclut_transform_source
*/

declare testEscapeCData(null) = null with protect
declare testTransformSource(null) = null with protect
declare testEscapeCData::lomain(null) = null


/**
  Validates the behavior of the testEscapeCData function.
*/
subroutine testEscapeCData(null)
  execute cclut_transform_source with replace("MAIN", lomain), curnamespace="testEscapeCData"
end ;;;testCreateXmlElement
subroutine testEscapeCData::lomain(null)
  declare data = vc with protect, noconstant("")
  declare escapedData = vc with protect, noconstant("")
  declare stat = i4 with protect, noconstant(0)

  set data = "i am a sting that contains some ]]> strings see? ]]> ]]>"
  set escapedData = escapeCData(data)
  set stat = cclutAssertVCEqual(CURREF, "check 1", escapedData, 
      "i am a sting that contains some ]]&gt; strings see? ]]&gt; ]]&gt;")

  set data = "here is a string with a cdata startoff <![CDATA[ to be replaced"
  set escapedData = escapeCData(data)
  set stat = cclutAssertVCEqual(CURREF, "check 1", escapedData, 
      "here is a string with a cdata startoff &lt;![CDATA[ to be replaced")
end

/**
  Validates the behavior of cclut_transform_source
*/
subroutine testTransformSource(null)
  declare stat = i4 with protect, noconstant(0)

  record request (
    1 programName = vc
    1 compileDate = vc
    1 source[*]
      2 line = vc
  ) with protect

  record reply (
    1 xml = vc
  ) with protect
  
  set request->programName = "some_prog"
  set request->compileDate = "long ago"
  set stat = alterlist(request->source, 22)
  set request->source[1].line = "create program some_prog ;this is the create line"
  set request->source[2].line = "/**"
  set request->source[3].line = "  this is the documentation for some_prog  "
  set request->source[4].line = "*/"
  set request->source[5].line = "   "
  set request->source[6].line = ";;;;CCLUT_START_INC_FILE %i cclsource:include_1.inc"
  set request->source[7].line = "the first line of content from include_1"
  set request->source[8].line = "the last line of content from include_1"
  set request->source[9].line = ";;;;CCLUT_END_INC_FILE %i cclsource:include_1.inc"
  set request->source[10].line = ";;;;CCLUT_START_INC_FILE %i cclsource:include_2.inc"
  set request->source[11].line = "the first line of content from include_2"
  set request->source[12].line = "the last line of content from include_2"
  set request->source[13].line = ";;;;CCLUT_END_INC_FILE %i cclsource:include_2.inc"
  set request->source[14].line = trim("")
  set request->source[15].line = ^declare some_vc = vc with protect, noconstant("")^
  set request->source[16].line = ^set some_vc = "hello_world"^
  set request->source[17].line = "call echo(some_vc)" ;intentional
  set request->source[18].line = "; a comment with a cdata tag <![CDATA[ not sure why this needs to be here but it is]]>"
  set request->source[19].line = trim("")
  set request->source[20].line = "end"
  set request->source[21].line = trim("")
  set request->source[22].line = trim("")
  set request->source[22].line = "go"
  
  execute cclut_transform_source with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)
  
  set stat = cclutAssertVCEqual(CURREF, "reply 1", reply->xml, concat(
    "<LISTING><LISTING_NAME>SOME_PROG</LISTING_NAME><COMPILE_DATE>LONG AGO</COMPILE_DATE><LINES>",
    "<LINE><NBR>1</NBR><TEXT><![CDATA[create program some_prog ;this is the create line]]></TEXT></LINE>",
    "<LINE><NBR>2</NBR><TEXT><![CDATA[/**]]></TEXT></LINE>",
    "<LINE><NBR>3</NBR><TEXT><![CDATA[  this is the documentation for some_prog]]></TEXT></LINE>",
    "<LINE><NBR>4</NBR><TEXT><![CDATA[*/]]></TEXT></LINE>",
    "<LINE><NBR>5</NBR><TEXT><![CDATA[]]></TEXT></LINE>",
    "<LINE><NBR>6</NBR><TEXT></TEXT><START_OF_INC><![CDATA[%i cclsource:include_1.inc]]></START_OF_INC></LINE>",
    "<LINE><NBR>7</NBR><TEXT><![CDATA[the first line of content from include_1]]></TEXT></LINE>",
    "<LINE><NBR>8</NBR><TEXT><![CDATA[the last line of content from include_1]]></TEXT></LINE>",
    "<LINE><NBR>9</NBR><TEXT></TEXT><END_OF_INC><![CDATA[%i cclsource:include_1.inc]]></END_OF_INC></LINE>",
    "<LINE><NBR>10</NBR><TEXT></TEXT><START_OF_INC><![CDATA[%i cclsource:include_2.inc]]></START_OF_INC></LINE>",
    "<LINE><NBR>11</NBR><TEXT><![CDATA[the first line of content from include_2]]></TEXT></LINE>",
    "<LINE><NBR>12</NBR><TEXT><![CDATA[the last line of content from include_2]]></TEXT></LINE>",
    "<LINE><NBR>13</NBR><TEXT></TEXT><END_OF_INC><![CDATA[%i cclsource:include_2.inc]]></END_OF_INC></LINE>",
    "<LINE><NBR>14</NBR><TEXT><![CDATA[]]></TEXT></LINE>",
    ^<LINE><NBR>15</NBR><TEXT><![CDATA[declare some_vc = vc with protect, noconstant("")]]></TEXT></LINE>^,
    ^<LINE><NBR>16</NBR><TEXT><![CDATA[set some_vc = "hello_world"]]></TEXT></LINE>^,
    "<LINE><NBR>17</NBR><TEXT><![CDATA[call echo(some_vc)]]></TEXT></LINE>", ;intentional
    "<LINE><NBR>18</NBR><TEXT><![CDATA[; a comment with a cdata tag &lt;![CDATA[ not sure why this needs to be he",
        "re but it is]]&gt;]]></TEXT></LINE>",
    "<LINE><NBR>19</NBR><TEXT><![CDATA[]]></TEXT></LINE>",
    "<LINE><NBR>20</NBR><TEXT><![CDATA[end]]></TEXT></LINE>",
    "<LINE><NBR>21</NBR><TEXT><![CDATA[]]></TEXT></LINE>",
    "<LINE><NBR>22</NBR><TEXT><![CDATA[go]]></TEXT></LINE>",
    "</LINES></LISTING>"))
  
end ;;;testTransformSource
