#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "no args found"
    exit 1
fi

n=10
if [[ "${1}" == "-n" ]]; then
    n="${2}"
    shift 2
fi

results=$(mktemp)
for file in "${@}"; do
    idf=$(basename -s "${file}")
    while read line; do
        time=$(echo "${line}" | cut -d ' ' -f1,2)
        data=$(echo "${line}" | cut -d ' ' -f3-)
        echo "${time} ${idf} ${data}" >> "${results}"
    done < <(cat "${file}" | head -n "${n}")
done

cat "${results}" | sort -t ' ' -k 1n -k 2n
rm "${results}"
