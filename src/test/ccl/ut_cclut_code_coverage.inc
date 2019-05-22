/**
  Unit Tests for cclut_code_coverage.inc functions
*/

%i cclsource:cclut_compile_subs.inc
%i cclsource:cclut_code_coverage.inc


declare setupOnce(null) = null
declare testGetNextCoverageToggleLineNumber(null) = null
declare testGetCoverageExclusions(null) = null
declare testExcludeCoverage(null) = null

declare sourceListingXml = vc with protect, noconstant("")


/**
  Constructs the listing xml for ut_cclut_cc_exclusions.
*/
subroutine setupOnce(null)
  declare CCLSOURCE = vc with protect, constant("CCLSOURCE")
  declare CCLUSERDIR = vc with protect, constant("CCLUSERDIR")
  declare listingDirectory = vc with protect, constant(concat(trim(logical(CCLUSERDIR), 3), "/"))
  declare programName = vc with protect, constant("ut_cclut_cc_exclusions")
  declare programFileName = vc with protect, constant(concat(programName, ".prg"))
  declare listingName = vc with protect, constant(concat(programName, ".lis"))
  declare success = i2 with protect, noconstant(FALSE)
  declare errorMessage = vc with protect, noconstant("")
 
  set success = cclutCompileProgram(CCLSOURCE, programFileName, CCLUSERDIR, listingName, errorMessage)
  call cclutAssertI2Equal(CURREF, "compileProgram", success, TRUE)
  set sourceListingXml = cclut::getListingXml(programName, listingDirectory, listingName)
end ;;;setupOnce


/**
  Validates the getNextCoverageToggleLineNumber function
*/
subroutine testGetNextCoverageToggleLineNumber(null)
  declare nextStart = i4 with protect, noconstant(0)
  declare nextToggleLine = i4 with protect, noconstant(0)

  set nextToggleLine = cclut::getNextCoverageToggleLineNumber(sourceListingXml, 1, "OFF", nextStart)
  call cclutAssertI4Equal(CURREF, "first toggle off line", nextToggleLine, 12)
  set nextToggleLine = cclut::getNextCoverageToggleLineNumber(sourceListingXml, 1, "ON", nextStart)
  call cclutAssertI4Equal(CURREF, "first toggle on line", nextToggleLine, 14)
end ;;;testGetNextCoverageToggleLineNumber(null)


/**
  Validate that getCoverageExclusions works as expected.
*/
subroutine testGetCoverageExclusions(null)
  record ccExclusions (1 block[*] 2 startLine = i4 2 endLine = i4)

  call cclut::getCoverageExclusions(sourceListingXml, ccExclusions)

  call cclutAssertI4Equal(CURREF, "exclusion count", size(ccExclusions->block, 5), 4)
  call cclutAssertI4Equal(CURREF, "exclusion 1 start", ccExclusions->block[1].startLine, 12)
  call cclutAssertI4Equal(CURREF, "exclusion 1 stop", ccExclusions->block[1].endLine, 14)
  call cclutAssertI4Equal(CURREF, "exclusion 2 start", ccExclusions->block[2].startLine, 24)
  call cclutAssertI4Equal(CURREF, "exclusion 2 stop", ccExclusions->block[2].endLine, 27)
  call cclutAssertI4Equal(CURREF, "exclusion 3 start", ccExclusions->block[3].startLine, 35)
  call cclutAssertI4Equal(CURREF, "exclusion 3 stop", ccExclusions->block[3].endLine, 37)
  call cclutAssertI4Equal(CURREF, "exclusion 4 start", ccExclusions->block[4].startLine, 42)
  call cclutAssertI4Equal(CURREF, "exclusion 4 stop", ccExclusions->block[4].endLine, 47)
    
end ;;;testGetCoverageExclusions


/**
  Validates that excludeCoverage works as expected
*/ 
subroutine testExcludeCoverage(null)
  record ccExclusions (1 block[*] 2 startLine = i4 2 endLine = i4)

  declare expectedCoverageXml = vc with protect, noconstant("")
  set expectedCoverageXml = 
~<COVERAGE><COVERAGE_NAME>UT_CCLUT_CC_EXCLUSIONS</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>N</TYPE></LINE><LINE><NBR>2</NBR>\
<TYPE>N</TYPE></LINE><LINE><NBR>3</NBR><TYPE>N</TYPE></LINE><LINE><NBR>4</NBR><TYPE>N</TYPE></LINE><LINE><NBR>5</NBR><TYPE>N</TY\
PE></LINE><LINE><NBR>6</NBR><TYPE>N</TYPE></LINE><LINE><NBR>7</NBR><TYPE>N</TYPE></LINE><LINE><NBR>8</NBR><TYPE>N</TYPE></LINE><\
LINE><NBR>9</NBR><TYPE>C</TYPE></LINE><LINE><NBR>10</NBR><TYPE>N</TYPE></LINE><LINE><NBR>11</NBR><TYPE>C</TYPE></LINE><LINE><NBR\
>12</NBR><TYPE>N</TYPE></LINE><LINE><NBR>13</NBR><TYPE>N</TYPE></LINE><LINE><NBR>14</NBR><TYPE>N</TYPE></LINE><LINE><NBR>15</NBR\
><TYPE>C</TYPE></LINE><LINE><NBR>16</NBR><TYPE>N</TYPE></LINE><LINE><NBR>17</NBR><TYPE>N</TYPE></LINE><LINE><NBR>18</NBR><TYPE>C\
</TYPE></LINE><LINE><NBR>19</NBR><TYPE>C</TYPE></LINE><LINE><NBR>20</NBR><TYPE>N</TYPE></LINE><LINE><NBR>21</NBR><TYPE>C</TYPE><\
/LINE><LINE><NBR>22</NBR><TYPE>C</TYPE></LINE><LINE><NBR>23</NBR><TYPE>C</TYPE></LINE><LINE><NBR>24</NBR><TYPE>N</TYPE></LINE><L\
INE><NBR>25</NBR><TYPE>N</TYPE></LINE><LINE><NBR>26</NBR><TYPE>N</TYPE></LINE><LINE><NBR>27</NBR><TYPE>N</TYPE></LINE><LINE><NBR\
>28</NBR><TYPE>N</TYPE></LINE><LINE><NBR>29</NBR><TYPE>N</TYPE></LINE><LINE><NBR>30</NBR><TYPE>N</TYPE></LINE><LINE><NBR>31</NBR\
><TYPE>C</TYPE></LINE><LINE><NBR>32</NBR><TYPE>C</TYPE></LINE><LINE><NBR>33</NBR><TYPE>C</TYPE></LINE><LINE><NBR>34</NBR><TYPE>C\
</TYPE></LINE><LINE><NBR>35</NBR><TYPE>N</TYPE></LINE><LINE><NBR>36</NBR><TYPE>N</TYPE></LINE><LINE><NBR>37</NBR><TYPE>N</TYPE><\
/LINE><LINE><NBR>38</NBR><TYPE>N</TYPE></LINE><LINE><NBR>39</NBR><TYPE>N</TYPE></LINE><LINE><NBR>40</NBR><TYPE>N</TYPE></LINE><L\
INE><NBR>41</NBR><TYPE>C</TYPE></LINE><LINE><NBR>42</NBR><TYPE>N</TYPE></LINE><LINE><NBR>43</NBR><TYPE>N</TYPE></LINE><LINE><NBR\
>44</NBR><TYPE>N</TYPE></LINE><LINE><NBR>45</NBR><TYPE>N</TYPE></LINE><LINE><NBR>46</NBR><TYPE>N</TYPE></LINE><LINE><NBR>47</NBR\
><TYPE>N</TYPE></LINE><LINE><NBR>48</NBR><TYPE>N</TYPE></LINE><LINE><NBR>49</NBR><TYPE>C</TYPE></LINE><LINE><NBR>50</NBR><TYPE>N\
</TYPE></LINE><LINE><NBR>51</NBR><TYPE>N</TYPE></LINE><LINE><NBR>52</NBR><TYPE>C</TYPE></LINE><LINE><NBR>53</NBR><TYPE>N</TYPE><\
/LINE><LINE><NBR>54</NBR><TYPE>C</TYPE></LINE><LINE><NBR>55</NBR><TYPE>C</TYPE></LINE><LINE><NBR>56</NBR><TYPE>U</TYPE></LINE><L\
INE><NBR>57</NBR><TYPE>N</TYPE></LINE><LINE><NBR>58</NBR><TYPE>N</TYPE></LINE><LINE><NBR>59</NBR><TYPE>N</TYPE></LINE><LINE><NBR\
>60</NBR><TYPE>N</TYPE></LINE><LINE><NBR>61</NBR><TYPE>C</TYPE></LINE><LINE><NBR>62</NBR><TYPE>N</TYPE></LINE><LINE><NBR>63</NBR\
><TYPE>N</TYPE></LINE></LINES></COVERAGE>~


  declare initialCoverageXml = vc with protect, noconstant("")
  set initialCoverageXml = 
~<COVERAGE><COVERAGE_NAME>UT_CCLUT_CC_EXCLUSIONS</COVERAGE_NAME><LINES><LINE><NBR>1</NBR><TYPE>N</TYPE></LINE><LINE><NBR>2</NBR>\
<TYPE>N</TYPE></LINE><LINE><NBR>3</NBR><TYPE>N</TYPE></LINE><LINE><NBR>4</NBR><TYPE>N</TYPE></LINE><LINE><NBR>5</NBR><TYPE>N</TY\
PE></LINE><LINE><NBR>6</NBR><TYPE>N</TYPE></LINE><LINE><NBR>7</NBR><TYPE>N</TYPE></LINE><LINE><NBR>8</NBR><TYPE>N</TYPE></LINE><\
LINE><NBR>9</NBR><TYPE>C</TYPE></LINE><LINE><NBR>10</NBR><TYPE>N</TYPE></LINE><LINE><NBR>11</NBR><TYPE>C</TYPE></LINE><LINE><NBR\
>12</NBR><TYPE>N</TYPE></LINE><LINE><NBR>13</NBR><TYPE>C</TYPE></LINE><LINE><NBR>14</NBR><TYPE>N</TYPE></LINE><LINE><NBR>15</NBR\
><TYPE>C</TYPE></LINE><LINE><NBR>16</NBR><TYPE>N</TYPE></LINE><LINE><NBR>17</NBR><TYPE>N</TYPE></LINE><LINE><NBR>18</NBR><TYPE>C\
</TYPE></LINE><LINE><NBR>19</NBR><TYPE>C</TYPE></LINE><LINE><NBR>20</NBR><TYPE>N</TYPE></LINE><LINE><NBR>21</NBR><TYPE>C</TYPE><\
/LINE><LINE><NBR>22</NBR><TYPE>C</TYPE></LINE><LINE><NBR>23</NBR><TYPE>C</TYPE></LINE><LINE><NBR>24</NBR><TYPE>N</TYPE></LINE><L\
INE><NBR>25</NBR><TYPE>C</TYPE></LINE><LINE><NBR>26</NBR><TYPE>C</TYPE></LINE><LINE><NBR>27</NBR><TYPE>N</TYPE></LINE><LINE><NBR\
>28</NBR><TYPE>N</TYPE></LINE><LINE><NBR>29</NBR><TYPE>N</TYPE></LINE><LINE><NBR>30</NBR><TYPE>N</TYPE></LINE><LINE><NBR>31</NBR\
><TYPE>C</TYPE></LINE><LINE><NBR>32</NBR><TYPE>C</TYPE></LINE><LINE><NBR>33</NBR><TYPE>C</TYPE></LINE><LINE><NBR>34</NBR><TYPE>C\
</TYPE></LINE><LINE><NBR>35</NBR><TYPE>N</TYPE></LINE><LINE><NBR>36</NBR><TYPE>C</TYPE></LINE><LINE><NBR>37</NBR><TYPE>N</TYPE><\
/LINE><LINE><NBR>38</NBR><TYPE>N</TYPE></LINE><LINE><NBR>39</NBR><TYPE>N</TYPE></LINE><LINE><NBR>40</NBR><TYPE>N</TYPE></LINE><L\
INE><NBR>41</NBR><TYPE>C</TYPE></LINE><LINE><NBR>42</NBR><TYPE>N</TYPE></LINE><LINE><NBR>43</NBR><TYPE>C</TYPE></LINE><LINE><NBR\
>44</NBR><TYPE>C</TYPE></LINE><LINE><NBR>45</NBR><TYPE>U</TYPE></LINE><LINE><NBR>46</NBR><TYPE>N</TYPE></LINE><LINE><NBR>47</NBR\
><TYPE>N</TYPE></LINE><LINE><NBR>48</NBR><TYPE>N</TYPE></LINE><LINE><NBR>49</NBR><TYPE>C</TYPE></LINE><LINE><NBR>50</NBR><TYPE>N\
</TYPE></LINE><LINE><NBR>51</NBR><TYPE>N</TYPE></LINE><LINE><NBR>52</NBR><TYPE>C</TYPE></LINE><LINE><NBR>53</NBR><TYPE>N</TYPE><\
/LINE><LINE><NBR>54</NBR><TYPE>C</TYPE></LINE><LINE><NBR>55</NBR><TYPE>C</TYPE></LINE><LINE><NBR>56</NBR><TYPE>U</TYPE></LINE><L\
INE><NBR>57</NBR><TYPE>N</TYPE></LINE><LINE><NBR>58</NBR><TYPE>N</TYPE></LINE><LINE><NBR>59</NBR><TYPE>N</TYPE></LINE><LINE><NBR\
>60</NBR><TYPE>N</TYPE></LINE><LINE><NBR>61</NBR><TYPE>C</TYPE></LINE><LINE><NBR>62</NBR><TYPE>N</TYPE></LINE><LINE><NBR>63</NBR\
><TYPE>N</TYPE></LINE></LINES></COVERAGE>~

  declare actualCoverageXml = vc with protect, noconstant("")
  call cclut::getCoverageExclusions(sourceListingXml, ccExclusions)
  set actualCoverageXml = cclut::excludeCoverage(initialCoverageXml, ccExclusions)
  call cclutAssertVcEqual(CURREF, "check coverage less exclusions", actualCoverageXml, expectedCoverageXml)
end ;;;testExcludeCoverage(null)