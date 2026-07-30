iwyu_tool.py -p .build $@ 2>&1 | grep -v "no private include name for @headername" | grep -v "has correct "
