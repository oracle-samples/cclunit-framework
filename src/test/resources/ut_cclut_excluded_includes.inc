This is a test file used by ut_cclut_generate_test_case to validate that include strings are correctly excluded from the
source file.

%i cclsource:test_include.inc
%include cclsource:test_include2.inc
%i          cclsource:test_include3.inc

%i ccluserdir:test_include4.inc some junk characters
%j this is not an include line
%I ccluserdir:test_include5.inc
%INCLUDE CCLUSERDIR:TEST_INCLUDE6.INC
              %i this_should_not_work_since_the_percentage_is_not_flush_left.inc

call echo("non-include line") ;intentional
;%i this_should_also_not_work.inc