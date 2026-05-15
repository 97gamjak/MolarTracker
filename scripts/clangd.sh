echo "Clang-Tidy on changed files:"

files=()
while IFS=$'\t' read -r status old new; do
    case "$status" in
    D) ;;                                     # deleted — skip
    R*) [[ -f "$new" ]] && files+=("$new") ;; # renamed — use new path
    *) [[ -f "$old" ]] && files+=("$old") ;;  # added/modified
    esac
done < <(git diff --name-status "$(git merge-base HEAD origin/dev)")

clangd-tidy "${files[@]}" -p=. --tqdm -j3
