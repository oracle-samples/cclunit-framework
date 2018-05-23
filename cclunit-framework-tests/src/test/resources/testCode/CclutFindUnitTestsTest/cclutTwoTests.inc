declare testA(dummyVar = i2) = null
declare testB(dummyVar = i2) = null

subroutine testA(dummyVar)
call echo("hello")
end

subroutine testB(dummyVar)
call echo("hello2")
end