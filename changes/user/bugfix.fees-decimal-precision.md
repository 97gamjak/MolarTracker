- Fix fees and prices displaying incorrectly in the transaction tables when
  the entered value had fewer decimal places than the currency's internal
  precision — e.g. a fee of `1.05` showed up as `1.50000` instead of `1.05`
