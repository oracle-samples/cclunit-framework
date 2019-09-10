/**
  Mock for cclut_compile_test_case_file for unit testing. Saves the input structure (cclutRequest) as json in the string
  _mock_request if provided renaming the structure to mockRequest.
*/

drop program mock_cclut_compile_tcf:dba go
create program mock_cclut_compile_tcf:dba 

  declare serializeRequest(mockRequest = vc(ref), buf = vc(ref)) = null

  /**
    Serializes a structure to json using mockRequest for the structure name.
    @param mockRequest
      The structure to serialize
    @param buf
      A return buffer for the json.
  */
  subroutine serializeRequest(mockRequest, buf)
    set buf = cnvtrectojson(mockRequest)
  end
  
  if (validate(_mock_request) = TRUE)
    call serializeRequest(cclutRequest, _mock_request)
  endif
  set cclutReply->status_data.status = "S"
  set cclutReply->testCaseObjectName = "mock"
end go