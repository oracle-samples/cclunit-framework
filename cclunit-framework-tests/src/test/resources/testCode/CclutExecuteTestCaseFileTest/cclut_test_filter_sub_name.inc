declare testDoNotRun(null) = null
declare testDoRun(null) = null

subroutine testDoRunOne(null)
  call echo("I should run")
end

subroutine testDoNotRun(null)
  call echo("I should not run")
end

subroutine testDoRunTwo(null)
  call echo("I should run")
end