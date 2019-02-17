/**
* Executes a test knowing that every call to getPersonName(id) will return "Bob Marley".
*/
subroutine (testGetNameReturnsBobMarley(null) = null)
  declare otherScriptCallCount = i4 with protect, noconstant(0)

  call cclutExecuteProgramWithMocks("the_script", "", "testGetNameReturnsBobMarley")

  ; assert stuff here
end
subroutine (testGetNameReturnsBobMarley::getPersonName(id = f8) = vc)
    return ("Bob Marley")
end ;;;testGetNameReturnsBobMarley
