declare setup(null) = null
declare teardown(null) = null
declare testGetTestName(null) = null

/**
record getTestNameReply (
    1 setupTestName = vc
    1 teardownTestName = vc
    1 testExecutionTestName1 = vc
    1 testExecutionTestName2 = vc
)
*/

subroutine setup(null)
  if (getTestNameReply->setupTestName = "")
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
  if (getTestNameReply->teardownTestName = "")
    set getTestNameReply->teardownTestName = cclut::getTestName(null)
  else 
    set getTestNameReply->teardownTestName = concat(getTestNameReply->teardownTestName, ";", cclut::getTestName(null))
  endif
end
