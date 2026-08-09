- Replace the insertion-marker changelog diff check with a fragment-based
  changelog (`changes/user/`, `changes/developer/`), ported from the PQ
  sister project. Every PR now adds a small `<category>.<slug>.md` file
  instead of hand-editing `CHANGELOG.md`/`DEV-CHANGELOG.md`; a new
  `scripts/update_changelog.py <version>` aggregates fragments into the
  changelogs and stamps the release, replacing the shared `devops`
  submodule's `update_changelogs` command for this step
