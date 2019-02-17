;**********************************************************************************************************************************
;** This is a special test case that is specifically designed to test that when mock tables are not cleaned up, they are still
;** available at the "tearDown" and "tearDownOnce" stage.  This .inc is used by other tests in ut_cclut_ff and
;** ut_cclut_execute_test_case(_file) to confirm that the framework removes the mock tables afterward.
;**********************************************************************************************************************************

if (validate(cclutMockTableName) = 0)
    declare cclutMockTableName = vc with protect, noconstant("")
endif

declare tearDown(dummyVar = i2) = null
declare tearDownOnce(dummyVar = i2) = null

subroutine tearDown(dummyVar)
    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(cclutMockTableName) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclut_mock_table_misbehaving_cleanup_teardown 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclut_mock_table_misbehaving_cleanup_teardown 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclut_mock_table_misbehaving_cleanup_teardown 003", mockDate,
        cnvtdatetime("27-NOV-2018 08:00"))
end ;tearDown

subroutine tearDownOnce(dummyVar)
    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(cclutMockTableName) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclut_mock_table_misbehaving_cleanup_teardown_once 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclut_mock_table_misbehaving_cleanup_teardown_once 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclut_mock_table_misbehaving_cleanup_teardown_once 003", mockDate,
        cnvtdatetime("27-NOV-2018 08:00"))
end ;tearDownOnce

/* test_cclut_mock_table_misbehaving_cleanup ****************************************************************
*  Scenario: Creates a mock table and does not clean it up within tearDown or tearDownOnce                  *
************************************************************************************************************/
subroutine test_cclut_mock_table_misbehaving_cleanup(null)
    set cclutMockTableName =
        cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "1.0|test text|27-NOV-2018 08:00")
    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    declare mockId = f8 with protect, noconstant(0.0)
    declare mockText = vc with protect, noconstant("")
    declare mockDate = dq8 with protect, noconstant(0)
    select into "nl:"
    from (value(cclutMockTableName) m)
    detail
        mockId = m.SAMPLE_TABLE_ID
        mockText = m.SAMPLE_TABLE_TEXT
        mockDate = m.SAMPLE_TABLE_DATE
    with nocounter

    call cclutAssertf8Equal(CURREF, "test_cclut_mock_table_misbehaving_cleanup 001", mockId, 1.0)
    call cclutAssertvcEqual(CURREF, "test_cclut_mock_table_misbehaving_cleanup 002", mockText, "test text")
    call cclutAssertf8Equal(CURREF, "test_cclut_mock_table_misbehaving_cleanup 003", mockDate,
        cnvtdatetime("27-NOV-2018 08:00"))
end ;test_cclut_mock_table_misbehaving_cleanup