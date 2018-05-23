declare setup(null) = null with public
declare teardown(null) = null with public
declare testGetTestName(null) = null with public

/**
record getTestNameReply (
    1 setupTestName = vc
    1 teardownTestName = vc
    1 testExecutionTestName1 = vc
    1 testExecutionTestName2 = vc
)
*/

subroutine setup(null)
  if (textlen(trim(getTestNameReply->setupTestName)) = 0)
    set getTestNameReply->setupTestName = cclut::getTestName(null)
  else 
    set getTestNameReply->setupTestName = concat(getTestNameReply->setupTestName, ";", cclut::getTestName(null))
  endif
end

subroutine testGetTestName1(null)
    set getTestNameReply->testExecutionTestName1 = cclut::getTestName(null)
end

subroutine testGetTestName2(null)
    set getTestNameReply->testExecutionTestName2 = cclut::getTestName(null)
end

subroutine teardown(null)
  if (textlen(trim(getTestNameReply->teardownTestName)) = 0)
    set getTestNameReply->teardownTestName = cclut::getTestName(null)
  else 
    set getTestNameReply->teardownTestName = concat(getTestNameReply->teardownTestName, ";", cclut::getTestName(null))
  endif
end
