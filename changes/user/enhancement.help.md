- Add `ui::HelpDialog` (`src/ui/help/`) — empty help page framework with title
  label, `QTextBrowser` content area, and "Export to PDF…" button backed by
  `Qt6::PrintSupport` / `QPrinter`; wired through `HelpMenu::requestHelpPage`
  signal and `HelpMenuController`
