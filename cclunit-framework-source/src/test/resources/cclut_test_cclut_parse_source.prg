;*******************************************************
; this program is for unit testing cclut_parse_source  *
;*******************************************************
drop program cclut_test_cclut_parse_source:dba go
create program cclut_test_cclut_parse_source:dba ;plus some comment
/**
  A program for unit testing cclut_parse_source
*/
  declare idx = i4 with protect, noconstant(0)
  for (idx = 1 to 10) ;this is a loop
    call echo("this is a program for unit testing cclut_parse_source")
  endfor
end go
  