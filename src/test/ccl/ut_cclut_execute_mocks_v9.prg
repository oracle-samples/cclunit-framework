;******************************************************************************
; This program is for unit testing test_cclutExecuteProgramWithMocks_v9.  It  *
; overrides cclutGetCurrentVersion to return 90000.                           *
;******************************************************************************
drop program ut_cclut_execute_mocks_v9:dba go
create program ut_cclut_execute_mocks_v9:dba

    declare cclut::cclutGetCurrentVersion(null) = i4 with protect
    subroutine cclut::cclutGetCurrentVersion(null)
        return(90000)
    end

%i cclsource:cclutmock.inc

    set cclut_testRecord->table_name = cclutDefineMockTable("sample_table", "sample_table", "vc")
    call cclutCreateMockTable("sample_table")

    call cclutExecuteProgramWithMocks("ut_cclut_table_column_name")

    call cclutRemoveMockTable("sample_table")

end
go
