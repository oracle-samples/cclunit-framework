/**
  The minimum required version of CCL for the CCL Unit Testing Framework to work.

  If newer CCL functionality is leveraged during an update, this value must be updated to the be the oldest version of CCL
  which supports that functionality.

  If the minimum ever needs to be bumped, the architecture for this concept will need to be revised because
  there are now different minimums depending on the CCL revision. Currently this is okay because 9.0.0 equates to 8.12.2.
*/
declare cclut::FRAMEWORK_VERSION = vc with protect, constant("3.3")
declare cclut::MINIMUM_REQUIRED_CCL_VERSION = vc with protect, constant("8.12.0")
