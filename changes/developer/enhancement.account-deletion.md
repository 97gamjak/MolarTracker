- Add `deleteAccount()` through the repo/service/store layers for accounts,
  following the existing create/update pattern; deletion is staged in
  `AccountStore` and only reaches the database on commit, mirroring how
  account creation and renaming already work
