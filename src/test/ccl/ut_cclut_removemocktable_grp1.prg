;**********************************************************************************
; This program is for unit testing test_cclutRemoveMockTable_created_table_group1.*
; It overrides cclutGetCurrentGroup to return group 1.                            *
;**********************************************************************************
drop program ut_cclut_removemocktable_grp1:dba go
create program ut_cclut_removemocktable_grp1:dba

    declare cclut::cclutGetCurrentGroup(null) = i4 with protect
    subroutine cclut::cclutGetCurrentGroup(null)
        return(1)
    end

%i cclsource:cclutmock_table.inc

    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutRemoveMockTable("sample_table")

end
go
