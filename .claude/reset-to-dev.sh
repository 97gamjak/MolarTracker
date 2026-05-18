#!/bin/bash
# Ensures claude/ branches are created from origin/dev, not origin/main.
# The session harness creates claude/ branches from HEAD (main), so this
# recreates the branch from origin/dev before any work begins.
set -euo pipefail

CURRENT_BRANCH=$(git branch --show-current)

if [[ "$CURRENT_BRANCH" != claude/* ]]; then
    exit 0
fi

# Only act if the branch has no commits beyond origin/main,
# i.e. the harness just created it and no work has been done yet.
AHEAD_OF_MAIN=$(git rev-list origin/main..HEAD --count 2>/dev/null || echo "0")
if [[ "$AHEAD_OF_MAIN" -gt 0 ]]; then
    exit 0
fi

git checkout -B "$CURRENT_BRANCH" origin/dev
