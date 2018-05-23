declare testOne(null) = null
declare testTwo(null) = null
declare doTimer(name = vc, elapse = i4, maxElapse = f8) = null

subroutine testOne(null)
  call doTimer("timerA", 1, 1.5)
  call doTimer("tImera", 2, 2.5)
end

/**
  Start a timer with the given name. Stop it after delay seconds with a maximum allowed elapse of maxDelay.
*/
subroutine doTimer(name, delay, maxDelay)
  call cclutStartTimer(name, CURREF)
  declare endTime = dq8 with noconstant(cnvtlookahead(concat('"', trim(build(delay)), ',S"'), cnvtdatetime(curdate, curtime3)))
  while (cnvtdatetime(curdate, curtime3) < endTime)
    call pause(1)
  endwhile
  call cclutEndTimer(name, maxDelay)
end 