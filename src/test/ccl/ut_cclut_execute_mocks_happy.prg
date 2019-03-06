;******************************************************************************
; This program is for unit testing happy path of cclutExecuteProgramWithMocks *
;******************************************************************************
drop program ut_cclut_execute_mocks_happy:dba go
create program ut_cclut_execute_mocks_happy:dba

/* Expected reply structure
record reply (
    1 number_parameter = i4
    1 string_parameter = vc
    1 regular_join[*]
        2 person_id = f8
        2 encounter_id = f8
        2 encounter_alias = vc
    1 left_join[*]
        2 person_id = f8
        2 encounter_id = f8
        2 encounter_alias = vc
    1 rdb_join[*]
        2 columns = vc
        2 data = vc
)
*/

record public::persistRecord (
    1 item = i4
) with persistscript

declare public::internalVariable = i4 with protect, noconstant(0)
record public::internalRecord (
    1 item = i4
) with protect

declare public::internalSubroutine(null) = null with protect

subroutine (public::internalSubroutine(null) = null with protect)
    declare stat = i4 with protect, noconstant(0)
    set public_subroutine = 1

    if (reflect(parameter(1, 0)) > " ")
        set reply->number_parameter = $1
    endif
    if (reflect(parameter(2, 0)) > " ")
        set reply->string_parameter = $2
    endif

    declare newSize = i4 with protect, noconstant(0)

    select into "nl:"
    from sample_person sp
        ,sample_encounter se
        ,sample_encounter_alias sea
    plan sp
    join se
        where se.person_id = sp.person_id
    join sea
        where sea.encounter_id = se.encounter_id
    detail
        newSize = size(reply->regular_join, 5) + 1
        stat = alterlist(reply->regular_join, newSize)
        reply->regular_join[newSize].person_id = sp.person_id
        reply->regular_join[newSize].encounter_id = se.encounter_id
        reply->regular_join[newSize].encounter_alias = sea.encounter_alias
    with nocounter

    select into "nl:"
    from sample_person sp
        ,(left join sample_encounter se on se.person_id = sp.person_id)
        ,(left join sample_encounter_alias sea on sea.encounter_id = se.encounter_id)
    plan sp
    join se
    join sea
    order by sp.person_id
    detail
        newSize = size(reply->left_join, 5) + 1
        stat = alterlist(reply->left_join, newSize)
        reply->left_join[newSize].person_id = sp.person_id
        reply->left_join[newSize].encounter_id = se.encounter_id
        reply->left_join[newSize].encounter_alias = sea.encounter_alias
    with nocounter

    rdb set output "ccluserdir:cclut_happy.dat" end
    rdb
        select *
        from sample_person_rdb sp
            ,sample_encounter_rdb se
            ,sample_encounter_alias_rdb sea
        where se.person_id = sp.person_id
        and sea.encounter_id = se.encounter_id
    end
end

call internalSubroutine(null)

set internalVariable = 1
set internalRecord->item = 1

set persistRecord->item = 1

end go
