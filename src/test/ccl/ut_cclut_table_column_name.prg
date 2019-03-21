;******************************************************************************
; This program is used by unit tests to validate behavior when a mocked table *
; shares its name with one of its columns (e.g. code_value).                  *
;******************************************************************************
drop program ut_cclut_table_column_name:dba go
create program ut_cclut_table_column_name:dba

    select into "nl:" from sample_table st detail
        cclut_testRecord->value = st.sample_table
    with nocounter

end go
