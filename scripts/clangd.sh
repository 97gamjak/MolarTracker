echo "Clang-Tidy on changed files:"

files=($(git diff --name-only $(git merge-base HEAD $(git log --simplify-by-decoration --decorate=full --pretty=format:"%D" HEAD | grep -oP 'refs/remotes/origin/\K[^ ,>]+' | head -1))))

clangd-tidy "${files[@]}" -p=. --tqdm -j3
