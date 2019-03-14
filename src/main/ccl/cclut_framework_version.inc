/**
  The minimum required version of CCL for the CCL Unit Testing Framework to work. 
  
  If newer CCL functionality is leveraged during an update, this value must be updated to the be the oldest version of CCL 
  which supports that functionality.
*/
declare cclut::FRAMEWORK_VERSION = vc with protect, constant("3.3")
declare cclut::MINIMUM_REQUIRED_CCL_VERSION = vc with protect, constant("8.12.0")
