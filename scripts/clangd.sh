echo "Clang-Tidy on changed files:"

files=()
while IFS= read -r line; do
    status=$(echo "$line" | cut -f1)
    file=$(echo "$line" | cut -f2)
    [[ "$status" != "D" ]] && files+=("$file")
done < <(git diff --name-status "$(git merge-base HEAD origin/dev)")

clangd-tidy "${files[@]}" -p=. --tqdm -j3
