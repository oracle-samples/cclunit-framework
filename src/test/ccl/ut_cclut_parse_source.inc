/**
  Unit Tests for cclut_parse_source
*/

declare testItA(null) = null with protect
declare testItB(null) = null with protect


subroutine testItA(null)
  declare stat = i4 with protect, noconstant(0)
  record request(
    1 programName = vc
    1 path = vc
    1 filename = vc
  ) with protect

  record reply(
    1 source[*]
      2 line = vc
  )  with protect

  call compile("cclsource:cclut_test_cclut_parse_source.prg", "ccluserdir:cclut_test_cclut_parse_source.lis")
  call parser("drop program cclut_test_cclut_parse_source go")
  set request->programName = "cclut_test_cclut_parse_source"
  set request->path = concat(trim(logical("ccluserdir")), "/")
  set request->fileName = "cclut_test_cclut_parse_source.lis"
  execute cclut_parse_source with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)
  set stat = remove("cclut_test_cclut_parse_source.lis")
  set stat = cclutAssertI4Equal(CURREF, "source size", size(reply->source, 5), 9)
  set stat = cclutAssertVCEqual(CURREF, "line 1",  reply->source[1].line, 
      "create program cclut_test_cclut_parse_source:dba ;plus some comment")
  set stat = cclutAssertVCEqual(CURREF, "line 2",  reply->source[2].line, "/**")
  set stat = cclutAssertVCEqual(CURREF, "line 3",  reply->source[3].line, "  A program for unit testing cclut_parse_source")
  set stat = cclutAssertVCEqual(CURREF, "line 4",  reply->source[4].line, "*/")
  set stat = cclutAssertVCEqual(CURREF, "line 5",  reply->source[5].line, "  declare idx = i4 with protect, noconstant(0)")
  set stat = cclutAssertVCEqual(CURREF, "line 6",  reply->source[6].line, "  for (idx = 1 to 10) ;this is a loop")
  set stat = cclutAssertVCEqual(CURREF, "line 7",  reply->source[7].line, 
      ^    call echo("this is a program for unit testing cclut_parse_source") ;intentional^)
  set stat = cclutAssertVCEqual(CURREF, "line 8",  reply->source[8].line, "  endfor")
  set stat = cclutAssertVCEqual(CURREF, "line 9",  reply->source[9].line, "end go")
end ;;;testItA

subroutine testItB(null)
  declare stat = i4 with protect, noconstant(0)
  declare command = vc with protect, noconstant("")
  declare commandLen = i4 with protect, noconstant(0)
  
  record request(
    1 programName = vc
    1 path = vc
    1 filename = vc
  ) with protect

  record reply(
    1 source[*]
      2 line = vc
  )  with protect

  call compile("cclsource:cclut_test_cclut_parse_source.prg", "ccluserdir:cclut_test_cclut_parse_source.lis")

  set command = ~head -n -1 cclut_test_cclut_parse_source.lis > cclut_test_cclut_parse_source_a.lis~
  set command = ~sed s/1\)$// cclut_test_cclut_parse_source.lis > cclut_test_cclut_parse_source_a.lis~
  set commandLen = textlen(command)
  call dcl(command, commandLen, stat)
  call cclutAssertI4Equal(CURREF, "verify dcl status 1", stat, 1)

  set command = ~mv cclut_test_cclut_parse_source_a.lis cclut_test_cclut_parse_source.lis~
  set commandLen = textlen(command)
  call dcl(command, commandLen, stat)
  call cclutAssertI4Equal(CURREF, "verify dcl status 2", stat, 1)

  set command = ~echo "Command executed!" >> cclut_test_cclut_parse_source.lis~
  set commandLen = textlen(command)
  call dcl(command, commandLen, stat)
  call cclutAssertI4Equal(CURREF, "verify dcl status 3", stat, 1)

  call parser("drop program cclut_test_cclut_parse_source go")
  set request->programName = "cclut_test_cclut_parse_source"
  set request->path = concat(trim(logical("ccluserdir")), "/")
  set request->fileName = "cclut_test_cclut_parse_source.lis"
  execute cclut_parse_source with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)

  set stat = remove("cclut_test_cclut_parse_source.lis")
  set stat = cclutAssertI4Equal(CURREF, "source size", size(reply->source, 5), 9)
  set stat = cclutAssertVCEqual(CURREF, "line 1",  reply->source[1].line, 
      "create program cclut_test_cclut_parse_source:dba ;plus some comment")
  set stat = cclutAssertVCEqual(CURREF, "line 2",  reply->source[2].line, "/**")
  set stat = cclutAssertVCEqual(CURREF, "line 3",  reply->source[3].line, "  A program for unit testing cclut_parse_source")
  set stat = cclutAssertVCEqual(CURREF, "line 4",  reply->source[4].line, "*/")
  set stat = cclutAssertVCEqual(CURREF, "line 5",  reply->source[5].line, "  declare idx = i4 with protect, noconstant(0)")
  set stat = cclutAssertVCEqual(CURREF, "line 6",  reply->source[6].line, "  for (idx = 1 to 10) ;this is a loop")
  set stat = cclutAssertVCEqual(CURREF, "line 7",  reply->source[7].line, 
      ^    call echo("this is a program for unit testing cclut_parse_source") ;intentional^)
  set stat = cclutAssertVCEqual(CURREF, "line 8",  reply->source[8].line, "  endfor")
  set stat = cclutAssertVCEqual(CURREF, "line 9",  reply->source[9].line, "end go")
end ;;;testItB
