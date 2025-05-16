#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 arg expected"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "1st arg must be a file"
    exit 1
fi

if [[ ! -d "${2}" ]]; then
    echo "2nd file must be a dir"
    exit 1
fi

while read forbidden; do
    censore=$(echo "${forbidden}" | sed 's/./*/g')

    while read file; do
        sed -i -E "s/\b${forbidden}\b/${censore}/Ig" "${file}"
    done < <(find "${2}" -type f -name "*.txt")

done < <(cat "${1}")
