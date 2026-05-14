#!/bin/bash
input=$(cat)
base=$(echo "$input" | jq -r '.tool_input.base // "dev"')
if [ "$base" != "dev" ]; then
    echo "$input" | jq --arg b "$base" \
        '{systemMessage:("PR base corrected from \""+$b+"\" to \"dev\"."),hookSpecificOutput:{hookEventName:"PreToolUse",updatedInput:(.tool_input|.base="dev")}}'
fi
