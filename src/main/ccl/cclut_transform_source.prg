drop program cclut_transform_source:dba go
create program cclut_transform_source:dba
/**
  Transform a list of source code text into an XML format. The XML will be formatted as follows:
  <pre>
  &lt;LISTING&gt;
    &lt;LISTING_NAME&gt;program name&lt;/LISTING_NAME&gt;
    &lt;LINE&gt;
      &lt;NBR&gt;##&lt;/NBR&gt;
      &lt;TEXT&gt;&lt;![CDATA[source text]]&gt;&lt;/TEXT&gt;
    &lt;/LINE&gt;
    [... additional &lt;LINE /&gt; tag ...]
  &lt;/LISTING&gt;
  </pre>
  The consumer is expected to provide the cclutRequest and cclutReply structures.
*/

%i cclsource:cclut_xml_functions.inc

/**
  @request
  @field programName
    The name of the program whose source code is to be transformed.
  @field compileDate
    The compile date/time to report for the script.
  @field source
    A list of lines representing the source code of the script.
    @field line
      An individual line from the program source code.

  record cclutRequest(
    1 programName = vc
    1 compileDate = vc
    1 source[*]
      2 line = vc
  ) with protect
*/

/**
  @reply
  @field xml
    The XML version of the transformed source code.

  record cclutReply(
    1 xml = vc
  ) with protect
*/

declare public::main(null) = null with protect
declare public::escapeCData(source = vc) = vc with protect

/**
  Replaces the < from all <![CDATA[ with &lt; and the > from all ]]> with &gt;
*/
subroutine public::escapeCData(source)
  declare cclutReturnVal = vc with protect, noconstant("")

  set cclutReturnVal = replace(source, "<![CDATA[", "&lt;![CDATA[")
  set cclutReturnVal = replace(cclutReturnVal, "]]>", "]]&gt;")
  return (cclutReturnVal)
end ;;;escapeCData


/**
  The main function of the program
*/
subroutine public::main(null)
  declare cclutLineNumber  = i4 with protect, noconstant(0)
  declare cclutNameXml     = vc with protect, noconstant("")
  declare cclutCompDateXml = vc with protect, noconstant("")
  declare cclutSourceXml   = vc with protect, noconstant("")
   
  declare cclutLineXml     = vc with protect, noconstant("")
  declare cclutNbrXml      = vc with protect, noconstant("")
  declare cclutTextXml     = vc with protect, noconstant("")
  declare cclutExtra       = vc with protect, noconstant("")
  declare cclutLine        = vc with protect, noconstant("")
  declare cclutEscapedLine = vc with protect, noconstant("")

  for (cclutLineNumber = 1 to size(cclutRequest->source, 5))
    set cclutExtra = ""
    set cclutLine = cclutRequest->source[cclutLineNumber].line
    if (cclutLine = ";;;;CCLUT_START_INC_FILE *")
      set cclutTextXml = cclut::createXmlElement("TEXT", "")
      set cclutExtra = cclut::createXmlElement("START_OF_INC", 
          trim(concat("<![CDATA[", trim(substring(size(";;;;CCLUT_START_INC_FILE ")+1, 1000, cclutLine), 3), "]]>"), 3))
    elseif (cclutLine = ";;;;CCLUT_END_INC_FILE *")
      set cclutTextXml = cclut::createXmlElement("TEXT", "")
      set cclutExtra = cclut::createXmlElement("END_OF_INC", 
          trim(concat("<![CDATA[", trim(substring(size(";;;;CCLUT_END_INC_FILE ")+1, 1000, cclutLine), 3), "]]>"), 3))
    else
      set cclutEscapedLine = escapeCData(cclutLine);
      set cclutTextXml = cclut::createXmlElement("TEXT", concat("<![CDATA[", cclutEscapedLine, "]]>"))
    endif
  
    set cclutNbrXml = cclut::createXmlElement("NBR", trim(cnvtstring(cclutLineNumber, 25), 3))
    set cclutLineXml = cclut::createXmlElement("LINE", trim(concat(trim(cclutNbrXml), trim(cclutTextXml), trim(cclutExtra))))
    set cclutSourceXml = concat(cclutSourceXml, cclutLineXml)
  endfor
  
  set cclutSourceXml = cclut::createXmlElement("LINES", trim(cclutSourceXml, 3))
  set cclutNameXml = cclut::createXmlElement("LISTING_NAME", trim(cnvtupper(cclutRequest->programName), 3))
  set cclutCompDateXml = cclut::createXmlElement("COMPILE_DATE", trim(cnvtupper(cclutRequest->compileDate), 3))
  
  set cclutReply->xml = cclut::createXmlElement("LISTING", trim(concat(cclutNameXml, cclutCompDateXml, trim(cclutSourceXml, 3)), 3))
end ;;;main

call main(null)

end go 