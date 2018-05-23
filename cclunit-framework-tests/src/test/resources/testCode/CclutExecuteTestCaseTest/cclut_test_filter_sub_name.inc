declare testDoNotRun(null) = null
declare testDoRun(null) = null

subroutine testDoNotRun(null)
	call echo("I should not run")
end

subroutine testDoRun(null)
	call echo("I should run")
end