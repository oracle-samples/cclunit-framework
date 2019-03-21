drop program cclut_get_framework_state:dba go
create program cclut_get_framework_state:dba
/*
  This program allows the retrieval of descriptors of the existing framework, such as version.
*/

%i cclsource:cclut_framework_version.inc

if (validate(reply) = FALSE)
  /**
    The state of the testing framework.
   
    @reply
    @field state
      The state of the framework. This response is XML and has the following values:
      <ul>
        <li>VERSION: the version of the testing framework currently within the environment</li>
      </ul>
    @field requiredCcl
      The minimum CCL version required by this version of the framework
  */
  record reply (
      1 state = vc
      1 requiredCcl = vc
  ) with protect
endif

declare generateOutputLine(cclutDisplay = vc) = vc with protect
declare fillStr(cclutLen = i4, cclutFill = vc) = vc with protect

subroutine generateOutputLine(cclutDisplay)
  return(concat("*", fillStr(cclutMargin, " "), cclutDisplay, fillStr(cclutMargin+cclutTextWidth-textlen(cclutDisplay), " "), "*"))
end ;;;generateOutputLine

subroutine fillStr(cclutLen, cclutFill)
  declare cclutRetVal = vc with protect, noconstant(trim(""))
  declare cclutIdx = i4 with protect, noconstant(0)
  for (cclutIdx = 1 to cclutLen)
    set cclutRetVal = notrim(concat(cclutRetVal, cclutFill))
  endfor
  return (cclutRetVal)
end ;;;fillStr

declare cclutVersionDisplay = vc with protect, constant(concat("CCL Unit Framework version ", cclut::FRAMEWORK_VERSION))
declare cclutCclVersionDisplay = vc with protect, constant(concat(
    "Minimum required CCL version ", cclut::MINIMUM_REQUIRED_CCL_VERSION))

declare cclutVersionLen = i4 with protect, constant(textlen(cclutVersionDisplay))
declare cclutCclVersionLen = i4 with protect, constant(textlen(cclutCclVersionDisplay))
declare cclutTextWidth = i4 with protect, constant(maxval(cclutVersionLen, cclutCclVersionLen))
declare cclutMargin = i4 with protect, constant(4)
declare cclutLineWidth = i4 with protect, constant(2*cclutMargin + cclutTextWidth)
declare cclutFillLen = i4 with protect, noconstant(2+cclutLineWidth)

declare cclutLineOfStars = vc with protect, constant(fillStr(cclutFillLen, "*"))
declare cclutLineOfSpaces = vc with protect, constant(concat("*", fillStr(cclutLineWidth, " "), "*"))


set reply->state = concat("<STATE><VERSION><![CDATA[", cclut::FRAMEWORK_VERSION, "]]></VERSION>",
"<REQUIRED_CCL>", cclut::MINIMUM_REQUIRED_CCL_VERSION, "</REQUIRED_CCL></STATE>")

call echo(cclutLineOfStars) ;intentional
call echo(cclutLineOfSpaces) ;intentional
call echo(generateOutputLine(cclutVersionDisplay)) ;intentional
call echo(generateOutputLine(cclutCclVersionDisplay)) ;intentional
call echo(cclutLineOfSpaces) ;intentional
call echo(cclutLineOfStars) ;intentional


if (validate(_memory_reply_string) = TRUE)
  set _memory_reply_string = concat(cclutVersionDisplay, char(10), char(13), cclutCclVersionDisplay)
endif

end go