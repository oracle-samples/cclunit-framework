# Releasing cclunit-framework

There currently is no packaging for CCL projects. Consequently the framework code is not released to a repository. 
It is installed into a Cerner Millennium domain by downloading the project and 
executing `mvn clean install -P<some profile id>`. The changelog and version documentation do need to be updated when pull requests are commited, however.
