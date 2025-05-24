#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "1st arg must be a dir"
    exit 1
fi

if [[ ! "${2}" =~ ^.*\.svg$ ]]; then
    echo "2nd arg is not .svg file"
    exit 1
fi

classes=$(mktemp)
while read file; do
    cat "${file}" | grep -E "^[ ]*class" >> "${classes}"
done < <(find "${1}" -type f 2>/dev/null -name "*.h")

while read data; do

    edges=$(echo "${data}" \
    | sed -E "s/(class|public|protected|private)//g" \
    | sed -E "s/^[ ]*//g" \
    | sed -E "s/[ ]*$//g" \
    | tr -d ',' \
    | tr -s ' ' \
    | sed -E "s/[ ]*:[ ]*/ /g" \
    )
        
    if (( $(echo "${edges}" | tr ' ' '\n' | wc -l) == 1 )); then
        echo "${edges}" >> "${2}"
        continue;
    fi

    child=$(echo "${edges}" | cut -d ' ' -f1)
    parents=$(mktemp)
    (echo "${edges}" | cut -d ' ' -f2- | tr ' ' '\n') > "${parents}"

    echo "${child}" >> "${2}"
    while read parent; do
        echo "${parent} -> ${child}" >> "${2}"
    done < <(cat "${parents}")

done < <(cat "${classes}")
