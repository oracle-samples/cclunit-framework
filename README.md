# cclunit-framework

[The CCL Unit Framework][cclunit-framework-doc] is a collection of CCL source code for executing CCL Unit Tests and generating code coverage data.

***cclunit-framework*** is a maven reactor project which uses the [ccl-maven-plugin][ccl-maven-plugin]
 to install the CCL Unit Framework code into an HNAM environment and then test it. 
 For instructions on installing the framework code [look here][cclunit-framework-installation]. 
 
 The following modules are included in the reactor build
* [cclunit-framework-source][cclunit-framework-source] - The CCL Unit Framework source code.
* [cclunit-framework-tests][cclunit-framework-tests] - A maven project which tests the CCL Unit Framework installation.


## Current Version
3.0

## Update Schedule

The update schedule is driven by requests for and contributions of enhancements and corrections.

## Contribute

You are welcomed to contribute documentation improvements as well as code corrections and enhancements.  
Please read our [Contribution Guidelines][contibution_guidelines].

## Release

Committers:  
Please read and follow the [Release Guidelines][release_guidelines] when mergining a pull request.


## License

```
Copyright 2017 Cerner Innovation, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

[ccl-maven-plugin]:https://github.com/cerner/ccl-testing/tree/master/ccl-maven-plugin
[cclunit-framework-installation]:cclunit-framework-source/doc/FRAMEWORKINSTALL.md
[cclunit-framework-doc]:cclunit-framework-source/doc/FRAMEWORK.md
[cclunit-framework-source]:cclunit-framework-source/README.md
[cclunit-framework-tests]:cclunit-framework-tests
[cclunit-framework-schema-xml]:cclunit-framework-schema-xml
[contibution_guidelines]: CONTRIBUTING.md#contributing
[release_guidelines]: RELEASING.md
