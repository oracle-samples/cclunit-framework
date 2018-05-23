# cclunit-framework-source 

This project contains the CCL code which forms the CCL Unit Framework for executing CCL Unit tests and generating code coverage data.  
It can be installed into an HNAM environment by executing `mvn clean install -P<some profile id>` on the [cclunit-framework][cclunit-framework] 
project from a device configured to be able to execute [mavenized CCL projects][mavenized CCL projects] in that environment. Alternatively, the code
files can be copied to cclsource and manually included. Be sure to copy all source files and include all prg files in that case. The Discern prompt forms 
cclut.dpb and cclut_ff.dpb should also be imported using Discern Visual Developer if installing manually.

[cclunit-framework]: ../README.md
[mavenized CCL projects]: https://github.com/cerner/ccl-testing/tree/master/cerner-maven-ccl-plugin