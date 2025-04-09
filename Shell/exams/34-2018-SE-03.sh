#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -f ${1} ]]; then
    echo "1 arg is not a file"
    exit 1
fi

if [[ ! -f ${2} ]]; then
    echo "2 arg is not a file"
    exit 1
fi

while read line; do
    cat "${1}" | grep -E "^[0-9]+,${line}$" | sort -t ',' -k 1n | head -n 1 >> "${2}"
done < <(cat "${1}" | cut -d ',' -f2- | sort | uniq)
