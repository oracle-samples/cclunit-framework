drop program cclutGetLogicalUtility go
create program cclutGetLogicalUtility

/*
 * request (
 *  1 logical_name = vc
 * )
 */

/*
 * reply (
 *  1 logical_value = vc
 * )
 */

set reply->logical_value = logical(request->logical_name)

end
go