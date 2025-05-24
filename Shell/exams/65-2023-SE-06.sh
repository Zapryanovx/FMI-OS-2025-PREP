#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "1st arg must be a dir"
    exit 1
fi

if [[ -d "${2}" ]]; then
    echo "2nd arg should not exist"
    exit 1
fi

mkdir "${2}"

files=$(mktemp)
(find "${1}" -type f -name "*.jpg" -printf '%TY-%Tm-%Td %.8TT %p\n' | sort -t ' ' -k1) > "${files}"
echo "end" >> "${files}"
cat "${files}"
echo "---------"
filtered=$(mktemp)

start=$(cat "${files}" | head -n 1 | cut -d ' ' -f1)
curr="${start}"

while read data; do
    
    day=$(echo "${data}" | cut -d ' ' -f1)
    hour=$(echo "${data}" | cut -d ' ' -f2)
    path=$(echo "${data}" | cut -d ' ' -f3)

    echo "${day} - ${curr}"
    if [[ "${day}" != "${curr}" ]]; then
        mkdir "${2}/${start}_${curr}"
        
        cat "${filtered}"
        echo "------"
        
        while read file new_file; do
            cp "${file}" "${2}/${start}_${curr}/${new_file}"
        done < <(cat "${filtered}")

        start="${day}"
        curr="${day}"

        > "${filtered}" 
    else
        echo "${path} ${day}_${hour}.jpg" >> "${filtered}"
    fi

    curr=$(date -d "${curr} + 1 day" +'%Y-%m-%d' 2>/dev/null)
done < <(cat "${files}")

rm "${files}"
rm "${filtered}"

