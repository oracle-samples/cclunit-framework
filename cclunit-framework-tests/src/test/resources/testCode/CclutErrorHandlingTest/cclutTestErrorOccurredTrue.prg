drop program cclutTestErrorOccurredTrue go
create program cclutTestErrorOccurredTrue
%i cclsource:cclut_error_handling.inc

/*
 * reply (
 *      1 error_ind = i2
 *      1 error_msg = vc
 * )
 */

select into "nl:"
from nonexistent_table_should_never_exist
where no_such_column = 0
with nocounter

declare cclutTestErrorMsg = vc with protect, noconstant("")
set reply->error_ind = cclut::errorOccurred(cclutTestErrorMsg)
set reply->error_msg = cclutTestErrorMsg

end go