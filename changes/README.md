# Changelog fragments

Every regular pull request adds a changelog fragment file, or appends a
bullet to an existing unreleased one:

```text
changes/user/<category>.<title>.md
changes/developer/<category>.<title>.md
```

`<category>` is one of the fixed categories below (it selects the changelog
section). `<title>` is a short, free-text, lowercase-with-hyphens slug that
identifies the change (it does not appear in the rendered changelog).

Use `user` when someone running MolarTracker would notice the change in
behavior, results, the UI, or compatibility. Use `developer` for build
tooling, CI, tests, refactors, and internal maintenance.

Example: `changes/user/bugfix.duplicate-account.md` containing:

```markdown
- Fix silent failure when creating a duplicate account — the operation now
  correctly raises an error instead of throwing an unexpected exception type
```

## Multiple points in one fragment

A fragment can contain more than one Markdown bullet. Each bullet's first
line must start with `- `; if a bullet needs more than one line, continue it
on indented lines below, the same way entries already wrap in
`CHANGELOG.md`/`DEV-CHANGELOG.md` today. Do not leave blank lines between
bullets (or between a bullet and its continuation lines):

```markdown
- First point about this change.
- Second point that is long enough to wrap onto a second,
  indented line.
```

## Adding to an existing fragment

If your pull request extends a change that already has an unreleased
fragment (yours or someone else's), add another bullet to that file instead
of creating a new one. Pull requests may add new fragment files, append
bullets to existing ones, or both, and may touch both audiences.

Regular pull requests do not delete unreleased fragments, and do not edit
`CHANGELOG.md` or `DEV-CHANGELOG.md` directly.

## Categories

User categories:

- `enhancement`
- `change`
- `bugfix`
- `performance`
- `compatibility`
- `documentation`

Developer categories:

- `enhancement`
- `bugfix`
- `performance`
- `build`
- `ci`
- `test`
- `internal`
- `documentation`

## Release processing

The release workflow (`scripts/update_changelog.py <version>`, run from
`.github/workflows/create-tag.yml`) routes fragments into the matching
changelog, preserves the released history, and removes consumed fragments.
A release may contain entries for either audience or both.
