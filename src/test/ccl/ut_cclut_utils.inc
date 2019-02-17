declare setup(dummyVar = i2) = null
declare tearDown(dummyVar = i2) = null

subroutine tearDown(dummyVar)
	rollback
end ;tearDown

;**********************************************************************************************************************************
;** cclutIsEmpty
;**********************************************************************************************************************************
/* test_cclutIsEmpty_true ***********************************************************************************
*  Scenario: Validates that TRUE is returned whenever the string is empty or only has whitespace characters *
************************************************************************************************************/
subroutine test_cclutIsEmpty_true(null)
	call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_true 001", cclut::cclutIsEmpty(""), TRUE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_true 002", cclut::cclutIsEmpty("     "), TRUE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_true 003",
        cclut::cclutIsEmpty(concat(char(10), char(20), char(30))), TRUE)
end ;test_cclutIsEmpty_true

;**********************************************************************************************************************************
;** cclutIsEmpty
;**********************************************************************************************************************************
/* test_cclutIsEmpty_false **********************************************************************************
*  Scenario: Validates that FALSE is returned whenever the string has non-whitespace characters             *
************************************************************************************************************/
subroutine test_cclutIsEmpty_false(null)
	call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_false 001", cclut::cclutIsEmpty("aaaaa"), FALSE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_false 002", cclut::cclutIsEmpty("  aaaaa  "), FALSE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_false 003",
        cclut::cclutIsEmpty(concat(char(10), "aaaaa", char(30))), FALSE)
end ;test_cclutIsEmpty_false
