#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "file does not exist"
    exit 1
fi

if [[ ! -d $2 ]]; then
    echo "dir does not exist"
    exit 1
fi

objs=$(find "${2}" -mindepth 1 | wc -l)
if [[ "${objs}" -ne 0 ]]; then
    echo "dir must be empty"
    exit 1
fi

touch "${2}/dict.txt"
counter=1
while read -r line; do
    name=$(echo "${line}" | cut -d '(' -f1 | tr -s ' ' | sed -E 's/^[ ]*//' | sed -E 's/[ ]*$//')
    quote=$(echo "${line}" | cut -d ':' -f2 | tr -s ' ' | sed -E 's/^[ ]*//' | sed -E 's/[ ]*$//')

    if ! grep -q "^${name}" "${2}/dict.txt"; then
        echo "${name};${counter}" >> "${2}/dict.txt"
        touch "${2}/${counter}.txt"
    fi

    echo "${quote}" >> "${2}/${counter}.txt"
    counter=$((counter + 1))
done < <(cat "${1}")
