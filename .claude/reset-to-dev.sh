#!/bin/bash
# Ensures claude/ branches start from origin/dev, not origin/main.
# The session harness creates claude/ branches from HEAD (main), so this
# resets to origin/dev at session start before any work begins.
set -euo pipefail

CURRENT_BRANCH=$(git branch --show-current)

if [[ "$CURRENT_BRANCH" != claude/* ]]; then
    exit 0
fi

# Only reset if the branch has no commits beyond what origin/main has,
# i.e. the harness just created it and no work has been done yet.
AHEAD_OF_MAIN=$(git rev-list origin/main..HEAD --count 2>/dev/null || echo "0")
if [[ "$AHEAD_OF_MAIN" -gt 0 ]]; then
    exit 0
fi

# Check if we're already up to date with origin/dev.
BEHIND_DEV=$(git rev-list HEAD..origin/dev --count 2>/dev/null || echo "0")
if [[ "$BEHIND_DEV" -eq 0 ]]; then
    exit 0
fi

git reset --hard origin/dev
