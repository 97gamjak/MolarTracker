echo "Clang-Tidy on changed files:"

files=($(git diff --name-status $(git merge-base HEAD origin/dev)))

clangd-tidy "${files[@]}" -p=. --tqdm -j3
