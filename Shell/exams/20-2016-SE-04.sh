#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 arguments expected."
    exit 1
fi

if [[ ! $1 =~ ^[1-9][0-9]*$ || ! $2 =~ ^[1-9][0-9]*$ ]]; then
    echo "2 numbers expected."
    exit 1
fi

mkdir a b c

while read fileName; do
    lines=$(cat "${fileName}" | wc -l)
    printf "${lines}" - "${fileName}"
    if [[ "${lines}" -lt $1 ]]; then
        mv "${fileName}" a
    elif [[ "${lines}" -lt $2 ]]; then
        mv "${fileName}" b
    else
        mv "${fileName}" c
    fi
done < <(find . -type f)
