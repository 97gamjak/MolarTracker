set -e

cpp_checks --dirs src

cd .build
cmake .. -DMOLARTRACKER_ENABLE_DOCS=ON -DMOLARTRACKER_ENABLE_TESTING=ON
make docs
make -j
ctest --output-on-failure
cd ..

cppcheck \
    --project=compile_commands.json \
    --quiet \
    --file-filter=src/* \
    --suppress=useStlAlgorithm \
    --enable=all \
    --enable=performance \
    --enable=style \
    --enable=information \
    --enable=portability \
    --error-exitcode=1 \
    --suppressions-list=.cppcheck \
    --suppress=missingIncludeSystem \
    --inline-suppr \
    --inconclusive \
    --library=qt \
    -I src

clangd-tidy $(find src/) -p=.
