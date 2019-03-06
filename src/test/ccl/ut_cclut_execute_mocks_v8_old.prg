;******************************************************************************
; This program is for unit testing test_cclutExecuteProgramWithMocks_v8_old.  *
; It overrides cclutGetCurrentVersion to return 81400 and                     *
; cclutGetCurrentRevision to return 8.                                        *
;******************************************************************************
drop program ut_cclut_execute_mocks_v8_old:dba go
create program ut_cclut_execute_mocks_v8_old:dba

    declare cclut::cclutGetCurrentVersion(null) = i4 with protect
    subroutine cclut::cclutGetCurrentVersion(null)
        return(81400)
    end

    declare cclut::cclutGetCurrentRevision(null) = i4 with protect
    subroutine cclut::cclutGetCurrentRevision(null)
        return(8)
    end

%i cclsource:cclutmock.inc

    set cclut_testRecord->table_name = cclutDefineMockTable("sample_table", "sample_table", "vc")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "sample_table_v8")

    call cclutExecuteProgramWithMocks("ut_cclut_table_column_name")

    call cclutRemoveMockTable("sample_table")

end
go
