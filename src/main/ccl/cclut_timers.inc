%#ifndef CCLUT_TIMERS_INC
%#def CCLUT_TIMERS_INC
;**********************************************************************************************************************************
;
; Contains subroutines for implementing a CCLUT timer.
;
;**********************************************************************************************************************************
 
declare cclutStartTimer(cclutTimerName = vc(value), cclutLineNumber = i4(value, CURREF)) = i2
declare cclutEndTimer(cclutTimerName = vc, cclutLimit = f8) = i2
declare cclutGetTimerElapse(cclutTimerName = vc) = f8
 
record cclut_timerList
(
  1 timer[*]
    2 name = vc
    2 limit = f8
    2 lineNumber = i4
    2 startTime = f8
    2 endTime = f8
) with protect 
  
/**
  A timer is created with a specified name and its startTime is set to cnvtdatetime(curdate, curtime3). 
  Timer names must be unique. If a timer already exists with that name, then a CCL exception is thrown.  
  @param cclutTimerName 
    The name of the timer to start.
  @param cclutLineNumber 
    The line number at which the timer is started.
  @returns
    A boolean flag indicating whether the timer was created and started.
    @value TRUE 
      The timer was created and started. 
    @value FALSE
      The timer was not created. 
*/
subroutine cclutStartTimer(cclutTimerName, cclutLineNumber)
  declare cclutTimerCount = i4 with protect, noconstant(size(cclut_timerList->timer, 5))
  declare cclutTimerIndex = i4 with protect, noconstant(0)
  declare cclutTimerPosition = i4 with protect, noconstant(0)
  declare cclutStat = i4 with protect, noconstant(0)
  
  ;Make sure the Timer Name is unique
  set cclutTimerPosition = locateval(cclutTimerIndex, 1, cclutTimerCount, trim(cnvtupper(cclutTimerName), 3), 
                                 cnvtupper(cclut_timerList->timer[cclutTimerIndex].name))
 
  ;If the Timer Name does not already exists, then store the new timer information.
  if (cclutTimerPosition = 0)
    set cclutTimerCount = cclutTimerCount + 1
    set cclutStat = alterlist(cclut_timerList->timer, cclutTimerCount)
    set cclut_timerList->timer[cclutTimerCount].name = trim(cclutTimerName, 3)
    set cclut_timerList->timer[cclutTimerCount].lineNumber = cclutLineNumber
    set cclut_timerList->timer[cclutTimerCount].startTime = cnvtdatetime(curdate, curtime3)
    return (TRUE)
  else
    call cclexception(100, "E", concat("startTimer() - A timer already exists with the specified name - ", 
                      trim(cclutTimerName, 3)))
    return (FALSE)
  endif
end ;;;startTimer
 
/**
  Sets the endTime of a specified timer to the current time and the limit to the number of seconds provided. 
  If the specified timer is not found then a CCL exception is thrown.
  @param cclutTimerName 
    The name of the timer to end.
  @param cclutLimit 
    The maximum number of seconds allowed for the timer.
  @return
    A boolean flag indicating if the endTime was set for the timer.
    @value TRUE 
      The endTime was set for the timer.
    @value FALSE 
      The endTime was not set for the timer.
*/ 
subroutine cclutEndTimer(cclutTimerName, cclutLimit)
  declare cclutTimerCount = i4 with protect, noconstant(size(cclut_timerList->timer, 5))
  declare cclutTimerIndex = i4 with protect, noconstant(0)
  declare cclutTimerPosition = i4 with protect, noconstant(0)
 
  ;Find the Timer Name in the list of timers
  set cclutTimerPosition = locateval(cclutTimerIndex, 1, cclutTimerCount, trim(cclutTimerName, 3), 
                                 trim(cclut_timerList->timer[cclutTimerIndex].name, 3))
 
  ;If the Timer Name exists, then store the end time and max seconds
  if (cclutTimerPosition > 0)
    set cclut_timerList->timer[cclutTimerPosition].endTime = cnvtdatetime(curdate, curtime3)
    set cclut_timerList->timer[cclutTimerPosition].limit = cclutLimit
    return (TRUE)
  else
    call cclexception(100, "E", concat("endTimer() - Timer end time not set because specified timer was never started: ", 
                      trim(cclutTimerName, 3)))
    return (FALSE)
  endif
end ;;;endTimer
 
/**
  Returns a timer's elapse in seconds (endTime - startTime). If the specified timer does not exist or, if its
  start time or end time is not set, then a CCL exception is thrown.
  @param cclutTimerName 
    The name of the timer.
  @returns
    The number of seconds elapsed between the timers startTime and endTime or -1 if the timer is not found.
*/
subroutine cclutGetTimerElapse(cclutTimerName)
  declare CCLUT_DD_UNIT_HSEC = i4 with protect, constant(6)
  declare cclutTimerCount = i4 with protect, noconstant(size(cclut_timerList->timer, 5))
  declare cclutTimerIndex = i4 with protect, noconstant(0)
  declare cclutTimerPosition = i4 with protect, noconstant(0)
  declare cclutElapsedTime = f8 with protect, noconstant(0.0000)
  declare cclutValidTimer = i2 with protect, noconstant(TRUE)
 
  set cclutTimerPosition = locateval(cclutTimerIndex, 1, cclutTimerCount, trim(cclutTimerName, 3), 
                                 trim(cclut_timerList->timer[cclutTimerIndex].name, 3))
  if (cclutTimerPosition > 0) 
    if (cclut_timerList->timer[cclutTimerPosition].startTime = 0.0)
      call cclexception(100, "E", concat("getTimerElapse() - No start time was set for timer: ", cclutTimerName))
      set cclutValidTimer = FALSE
    endif
 
    if (cclut_timerList->timer[cclutTimerPosition].endTime = 0.0)
      call cclexception(100, "E", concat("getTimerElapse() - No end time was set for timer: ", cclutTimerName))
      set cclutValidTimer = FALSE
    endif
 
    if (cclutValidTimer)
      set cclutElapsedTime = round((datetimediff(cclut_timerList->timer[cclutTimerPosition].endTime, 
          cclut_timerList->timer[cclutTimerPosition].startTime, CCLUT_DD_UNIT_HSEC)/100.0), 2)
    else
      set cclutElapsedTime = -1.0
    endif
  else
    call cclexception(100, "E", concat("getTimerElapse() - No timer was found with the specified name - ", 
                      trim(cclutTimerName, 3)))
    set cclutElapsedTime = -1.0
  endif
  return (cclutElapsedTime)
end ;;;getTimerElapse
 
%#endif CCLUT_TIMERS_INC