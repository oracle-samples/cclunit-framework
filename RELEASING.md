# Releasing cclunit-framework

There currently is no packaging for CCL projects. Consequently the framework code is not released to a repository. 
Updates to the framework are released by tagging this repository.
The framework is installed into a Cerner Millennium domain by downloading the project and
executing `mvn clean install -P<some profile id>`. The changelog and version documentation do need to be updated when pull requests are merged, however.
