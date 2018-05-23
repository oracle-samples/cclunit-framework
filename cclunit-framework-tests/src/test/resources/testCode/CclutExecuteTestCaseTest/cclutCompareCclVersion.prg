drop program cclutCompareCclVersion go
create program cclutCompareCclVersion

/*
 The CCL version against which to compare the current CCL version
 record request (
    1 to_compare = vc
 )
 */

/*
 record reply (
    1 less_than_ind = i2 ; 1 = current version is less than given; 0 if not
    1 current_version = vc ; The current version of CCL
 )
 */

%i cclsource:cclut_env_utils.inc

set reply->current_version = cclutGetCCLVersion(null)
set reply->less_than_ind = cclutIsCCLVersionLessThan(reply->current_version, request->to_compare)
 
 
end
go
