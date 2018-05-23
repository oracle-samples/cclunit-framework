drop program cclutTestErrorOccurredFalse go
create program cclutTestErrorOccurredFalse
%i cclsource:cclut_error_handling.inc

/*
 * reply (
 *      1 error_ind = i2
 *      1 error_msg = vc
 * )
 */

declare cclutTestErrorMsg = vc with protect, noconstant("")
set reply->error_ind = cclErrorOccurred(cclutTestErrorMsg)
set reply->error_msg = cclutTestErrorMsg

end
go