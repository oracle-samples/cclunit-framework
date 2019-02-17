# CCL Unit Framework Check/Install

***Identify the latest released framework version***

Visit [cclunit-framework][cclunit-framework] to determine or retrieve the latest version.

***Identify the installed framework version***

Execute the following CCL command to identify the currently installed version. 

```
execute cclut_get_framework_state "MINE" go
```
If an error occurs indicating the cclut_get_framework_state program does not exist, then the framework is not installed. 
Otherwise the currently installed version will be displayed. 

If the installed version is older than 2.0, the the following instructions 
must be executed to see the verion.
```
record cclut_version (1 state = vc) go
execute cclut_get_framework_state with replace("REPLY", cclut_version) go
call echo(build2("framework version: ", piece(piece(cclut_version->state,"[",3,""),"]",1,""))) go
```

***Install the framework***

Write access in CCLSOURCE and DBA-level CCL access are required to install or upgrade the framework. It is recommended to use maven to install the framework, 
but it can be installed manually if necessary. If a manual installation is performed, be sure to copy all source files and compile all program files to
avoid problems with missing or mismatched dependencies. 

Here are instructions for installing the framework **using maven**.
- [Configure maven][configure-maven] for CCL Unit testing in the environment
- Download and unzip the [cclunit-framework][cclunit-framework] project.
- Open a command prompt and navigate the current directory to the top level folder of the downloaded cclunit-framework project.
- Execute `mvn clean install -P<profile id>` using the profile id created in the configure maven step


Here are instructions for installing the framework **manually**.
- Download and unzip the [cclunit-framework][cclunit-framework] project.
- Copy all of the source code (.prg and .inc files) from cclunit-framework/source/main/ccl and cclunit-framework/source/main/resources to $CCLSOURCE.
- Compile each program file (.prg).
- Import the Discern Prompt Forms cclut_ff.dpb and cclut.dpb using Discern Visual Developer.

[cclunit-framework]:../..
[configure-maven]:https://github.com/cerner/ccl-testing/blob/master/doc/CONFIGUREMAVEN.md