drop program testprg go
create program testprg

call echo("TEST")

declare temp = i4

set temp = 5

set temp = temp + 6

call echo(cnvtstring(temp))

select into "nl:"
from person p
with maxrec = 5


end
go
