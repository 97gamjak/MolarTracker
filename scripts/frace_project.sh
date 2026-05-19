#!/bin/bash
# ftrace_project.sh
set -e

BUILD_DIR=${1:-build}
TRACE_DIR=/tmp/ftrace_project
mkdir -p "$TRACE_DIR"
rm -rf "$TRACE_DIR"/*

echo "Extracting all TUs from compile_commands.json..."
FILES=$(jq -r '.[].file' "$BUILD_DIR/compile_commands.json")
TOTAL=$(echo "$FILES" | wc -l)
COUNT=0

for FILE in $FILES; do
    COUNT=$((COUNT + 1))
    echo "[$COUNT/$TOTAL] $FILE"

    BASENAME=$(basename "$FILE" .cpp)
    OUT="$TRACE_DIR/${BASENAME}_$COUNT"

    CMD=$(jq -r --arg f "$FILE" \
        '.[] | select(.file == $f) | .command' \
        "$BUILD_DIR/compile_commands.json")

    CLANG_CMD=$(echo "$CMD" |
        sed 's|clang++-20|g++|' |
        sed 's|g++|clang++-20|' |
        sed "s|-o [^ ]*|-o ${OUT}.o|" |
        sed 's|-Wunused-private-field||g')

    eval "$CLANG_CMD -ftime-trace -Wno-unused-private-field" #2>/dev/null || echo "  WARNING: failed $FILE"
done

echo ""
echo "Aggregating with ClangBuildAnalyzer..."
ClangBuildAnalyzer --all "$TRACE_DIR" "$TRACE_DIR/capture.bin"
ClangBuildAnalyzer --analyze "$TRACE_DIR/capture.bin" | tee "$TRACE_DIR/report.txt"

echo ""
echo "Full report: $TRACE_DIR/report.txt"
