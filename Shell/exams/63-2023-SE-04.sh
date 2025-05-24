#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "dir expected"
    exit 1
fi

hash_sums=$(mktemp)
find "${1}" -type f 2>/dev/null | xargs -I{} sha256sum {} > "${hash_sums}"

groups=0
mem=0

while read hash; do
    files=$(mktemp)

    (cat "${hash_sums}" | grep -E "^${hash}" | cut -d ' ' -f3) > "${files}"

    cnt=$(cat "${files}" | wc -l) 
    if [[ "${cnt}" -eq 1 ]]; then
        rm "${files}"
        continue;
    fi
    
    my_cpy=$(cat "${files}" | head -n 1)
    groups=$(( ${groups} + 1 ))

    bytes=$(stat "${my_cpy}" --printf "%s\n")
    cnt=$(( ${cnt} - 1 ))
    mem=$(( ${mem} + ${bytes} * ${cnt} ))

    while read path; do
        rm "${path}"
        ln -s "${my_cpy}" "${path}"
    done < <(cat "${files}" | tail -n +2)
    
    rm "${files}"

done < <(cat "${hash_sums}" | cut -d ' ' -f1 | sort | uniq)

echo "Groups dedublicated: ${groups}"
echo "Freed memory: ${mem}"
rm "${hash_sums}"
