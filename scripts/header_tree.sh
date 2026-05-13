#!/bin/bash
# header_tree.sh <partial-filename>
# Example: ./header_tree.sh main_controller.cpp

FILE_PATTERN=${1:?"Usage: $0 <partial-filename>"}
BUILD_DIR=${2:-.build}
TRACE_DIR=/tmp/ftrace_project

CMD=$(jq -r --arg pat "$FILE_PATTERN" \
    '.[] | select(.file | endswith($pat)) | .command' \
    "$BUILD_DIR/compile_commands.json" | head -1)

if [ -z "$CMD" ]; then
    echo "No file matching '$FILE_PATTERN' found in $BUILD_DIR/compile_commands.json"
    exit 1
fi

mkdir -p "$TRACE_DIR"

eval "$(echo "$CMD" |
    sed 's|clang++-20|g++|' |
    sed 's|g++|g++ -H|' |
    sed 's|-o [^ ]*|-o /dev/null|')" 2>&1 |
    grep -E "^\.+" |
    grep -v "/usr/include" |
    grep -v "$BUILD_DIR" \
        >"$TRACE_DIR/header_tree.txt" || true

echo "Header tree for '$FILE_PATTERN' written to $TRACE_DIR/header_tree.txt"
