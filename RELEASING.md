# Releasing cclunit-framework

There currently is no packaging for CCL projects. Consequently the framework code is not released to a repository. 
Updates to the framework are released by tagging this repository.
When 'releasing' be sure to 
* update the changelog.
* update the version listed in readme.md.
* update the version number in the pom file.
* update the version number in cclut_framework_version.inc.
* update the minimum required CCL vesion in cclut_framework_version.inc if changed.
* double check for dangling call echo and call echorecord statements.
* push to a new branch.
* execute `mvn clean test -P<profileId>` on the branch.
* tag the branch with the release number and merge to master.
* create features to add the release to Cerner's development pipelines.