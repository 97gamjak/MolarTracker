- Add `ui::HelpDialog` (`src/ui/help/`) — empty help page framework with title
  label, `QTextBrowser` content area, and "Export to PDF…" button backed by
  `Qt6::PrintSupport` / `QPrinter`; wired through `HelpMenu::requestHelpPage`
  signal and `HelpMenuController`
- Add watchlists: the Securities sidebar now expands into "All Securities"
  (shown by default) plus your saved watchlists. Right-click Securities to
  create a watchlist, right-click a security row to add/remove it to/from a
  watchlist, and right-click a watchlist to rename or delete it
