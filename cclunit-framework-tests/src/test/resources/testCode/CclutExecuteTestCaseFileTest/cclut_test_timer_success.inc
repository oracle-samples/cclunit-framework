declare testOne(null) = null
declare testTwo(null) = null
declare doTimer(name = vc, limit = f8, elapse = i4) = null

subroutine testOne(null)
  call doTimer("timerA", 1.5, 1)
  call doTimer("timerB", 2.5, 2)
end

subroutine testTwo(null)
  call doTimer("timerA", 1.25, 1)
  call doTimer("timerB", 2.25, 2)
end

/**
  Start a timer with the given name. Stop it after elapse seconds with a maximum allowed elapse of limit.
*/
subroutine doTimer(name, limit, elapse)
  call cclutStartTimer(name)
  declare endTime = dq8 with noconstant(cnvtlookahead(concat('"', trim(build(elapse)), ',S"'), cnvtdatetime(curdate, curtime3)))
  while (cnvtdatetime(curdate, curtime3) < endTime)
    call pause(1)
  endwhile
  call cclutEndTimer(name, limit)
end 