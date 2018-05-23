drop program cclutTestFilterCoverage go
create program cclutTestFilterCoverage

%i cclsource:cclut_code_coverage.inc

/*  
 *  record dt_request (
 *    1 coverage_xml = vc
 *  ) with protect
 *
 *  record dtPrograms(
 *    1 programs[*]
 *      2 programName = vc
 *      2 coverageXML = vc
 *  )
 */
 
call cclut::filterCoverageXML(dt_request->coverage_xml, dtPrograms)

end go
