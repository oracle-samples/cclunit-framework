/**
  Unit Tests for cclut_get_framework_state.
*/

%i cclsource:cclut_framework_version.inc

declare captureEcho(msg = vc) = null with protect
declare testing::checkMessages(null) = null with protect
declare setup(null) = null with protect
declare testIt(null) = null with protect
declare testItWithoutReplyWithoutMRS(null) = null with protect
declare testItWithoutReplyWithMRS(null) = null with protect
declare testItWithReplyWithMRS(null) = null with protect


record testing::echoMessages (
  1 message[*]
    2 value = vc
) with protect

record testing::expectedMessages (
  1 message[*]
    2 value = vc
    2 type = i4
) with protect

declare testing::ASSERT_TYPE_EQUAL = i4 with protect, constant(0)
declare testing::ASSERT_TYPE_DATE = i4 with protect, constant(1)
declare testing::ASSERT_TYPE_MATCH = i4 with protect, constant(2)

declare expectedVersionString = vc with protect, constant(cclut::FRAMEWORK_VERSION)
declare expectedVersionDisplay = vc with protect, constant(concat("CCL Unit Framework version ", expectedVersionString))
declare expectedCclVersion = vc with protect, constant(cclut::MINIMUM_REQUIRED_CCL_VERSION)
declare expectedCclVersionDisplay = vc with protect, constant(concat("Minimum required CCL version ", expectedCclVersion))
declare versionDisplayExcessWidth = i4 with protect, constant(textlen(expectedVersionDisplay) - textlen(expectedCclVersionDisplay))

subroutine captureEcho(msg)
  declare messageCount = i4 with protect, noconstant(0)
  declare stat = i4 with protect, noconstant(0)

  set messageCount = 1 + size(testing::echoMessages->message, 5)
  set stat = alterlist(testing::echoMessages->message, messageCount)
  set testing::echoMessages->message[messageCount].value = msg
  call echo(msg) ;intentional
  return (null)
end ;;;captureEcho

subroutine testing::checkMessages(null)
  declare count = i4 with protect, noconstant(0)
  declare idx = i4 with protect, noconstant(0)
  declare matches = i2 with protect, noconstant(FALSE)

  set count = size(testing::expectedMessages->message, 5)
  call cclutAssertI4Equal(CURREF, "message count", size(testing::echoMessages->message, 5), count)

  for (idx = 1 to count)
    case(testing::expectedMessages->message[idx].type)
    of testing::ASSERT_TYPE_EQUAL:
      call cclutAssertVCEqual(CURREF, build2("message-",trim(build(idx),3)), testing::echoMessages->message[idx].value,
        testing::expectedMessages->message[idx].value)
    of testing::ASSERT_TYPE_MATCH:
      set matches = operator(testing::echoMessages->message[idx].value, "regexplike",
          testing::expectedMessages->message[idx].value)
      call cclutAssertI2Equal(CURREF, build2("message-",trim(build(idx),3)), matches, TRUE)
    endcase
  endfor
end ;;;


/**
  per-test initialization
*/
subroutine setup(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = initRec(testing::echoMessages)
  set stat = initRec(testing::expectedMessages)

/*
  set testing::expectedMessages->message[1].value = "*********************************************"
  set testing::expectedMessages->message[2].value = "*                                           *"
  set testing::expectedMessages->message[3].value = concat("*    ", expectedVersionDisplay, "         *")
  set testing::expectedMessages->message[4].value = concat("*    ", expectedCclVersionDisplay, "    *")
  set testing::expectedMessages->message[5].value = "*                                           *"
  set testing::expectedMessages->message[6].value = "*********************************************"
*/

  set stat = alterlist(testing::expectedMessages->message, 6)
  set testing::expectedMessages->message[1].value = cclut::FRAMEWORK_STATE_LINE_OF_STARS
  set testing::expectedMessages->message[2].value = cclut::FRAMEWORK_STATE_LINE_OF_SPACES
  if (versionDisplayExcessWidth < 0)
    set testing::expectedMessages->message[3].value =
        concat("*    ", expectedVersionDisplay, cclutFillStr(-versionDisplayExcessWidth, " "), "    *")
  else
    set testing::expectedMessages->message[3].value = concat("*    ", expectedVersionDisplay, "    *")
  endif
  if (versionDisplayExcessWidth > 0)
    set testing::expectedMessages->message[4].value =
      concat("*    ", expectedCclVersionDisplay, cclutFillStr(versionDisplayExcessWidth, " "), "    *")
  else
    set testing::expectedMessages->message[4].value = concat("*    ", expectedCclVersionDisplay, "    *")
  endif
  set testing::expectedMessages->message[5].value = cclut::FRAMEWORK_STATE_LINE_OF_SPACES
  set testing::expectedMessages->message[6].value = cclut::FRAMEWORK_STATE_LINE_OF_STARS
end ;;;setup


/**
  Test cclut_get_framework_state as it will be called by the framework.
*/
subroutine testIt(null)
  record reply (
      1 state = vc
  ) with protect

  execute cclut_get_framework_state with replace ("ECHO", captureEcho)

  call cclutAssertVCEqual(CURREF, "framework state", reply->state,
      concat("<STATE><VERSION><![CDATA[", expectedVersionString, "]]></VERSION><REQUIRED_CCL>", expectedCclVersion,
          "</REQUIRED_CCL></STATE>"))

  call testing::checkMessages(null)
end ;;;testIt

/**
  Test cclut_get_framework_state as it will be called in a CCL session to check the current framework version.
*/
subroutine testItWithoutReplyWithoutMRS(null)
  execute cclut_get_framework_state with replace ("ECHO", captureEcho)

  call testing::checkMessages(null)
end ;;;testItWithoutReplyWithoutMRS

/**
  Test cclut_get_framework_state as it will be called from DvDev to check the current framework version.
*/
subroutine testItWithoutReplyWithMRS(null)
  declare _memory_reply_string = vc with protect, noconstant("")
  execute cclut_get_framework_state with replace ("ECHO", captureEcho)

  call cclutAssertVCEqual(CURREF, "validate mrs", _memory_reply_string,
      concat(expectedVersionDisplay, char(10), char(13), expectedCclVersionDisplay))
  call testing::checkMessages(null)
end ;;;testItWithoutReply


/**
  Test cclut_get_framework_state as it might be called by a custom program to check the current framework version.
*/
subroutine testItWithReplyWithMRS(null)
  declare _memory_reply_string = vc with protect, noconstant("")

  record reply (
      1 state = vc
  ) with protect

  execute cclut_get_framework_state with replace ("ECHO", captureEcho)

  call cclutAssertVCEqual(CURREF, "framework state", reply->state,
      concat("<STATE><VERSION><![CDATA[", expectedVersionString, "]]></VERSION><REQUIRED_CCL>", expectedCclVersion,
          "</REQUIRED_CCL></STATE>"))
  call cclutAssertVCEqual(CURREF, "validate mrs", _memory_reply_string,
      concat(expectedVersionDisplay, char(10), char(13), expectedCclVersionDisplay))
  call testing::checkMessages(null)
end ;;;testItWithoutReply

