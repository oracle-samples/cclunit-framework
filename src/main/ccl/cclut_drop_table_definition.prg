drop program cclut_drop_table_definition:dba go
create program cclut_drop_table_definition:dba

/**
  A prompt program for dropping a single Discern table definition.  This should only be used through a tdbexecute for
  group1 users.
  @arg (vc) The output destination for the results: stdout (MINE), a file, or a print queue.
    @default MINE
  @arg (vc) A table name for which the Discern table definition will be dropped.  Only custom tables created by CCL Unit
    should be used with this program.  For that reason, the table name must start with "CUST_CCLUT" or it will not be
    dropped.
*/
prompt
	"Output to File/Printer/MINE" = "MINE"
	, "Table Name:" = ""

with outputDestination, tableName

declare public::dropTable(cclutTableName = vc) = null with protect
declare public::main(null) = null with protect

subroutine public::dropTable(cclutTableName)
    if(cclutTableName = "CUST_CCLUT*")
        call echo(concat("Table prefixed with CUST_CCLUT. Dropping ", cclutTableName))
    	call parser(concat(" drop table ", cclutTableName, " go"))
    else
    	call echo(concat("Table was not prefixed with CUST_CCLUT.  Not dropping ", cclutTableName))
    endif
end ;cclutDropTableDefinition::dropTable

subroutine public::main(null)
    call dropTable($tableName)
end ;cclutDropTableDefinition::main

call main(null)

end
go