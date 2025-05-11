#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "3 args expected"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "1 arg must be a file"
    exit 1
fi

if [[ ! $2 =~ [a-zA-Z_]+ ]]; then
    echo "invalid key format"
    exit 1
fi

if [[ ! $3 =~ [a-zA-Z_]+ ]]; then
    echo "invalid val format"
    exit 1
fi

res=$(mktemp)

found=0
while read line; do
    if echo "${line}" | sed 's/ //g' | sed 's/#.*//g' | grep -E -q "^${2}"; then
        echo "${line} # edited at $(date) by $(whoami)" >> "${res}"
        echo "${2} = ${3} # added at $(date) by $(whoami)" >> "${res}"
        found=1
    else
        echo "${line}" >> "${res}"
    fi
done < <(cat "${1}")

if [[ "${found}" -eq 0 ]]; then
    echo "${2} = ${3} # added at $(date) by $(whoami)" >> "${res}"
fi

cat "${res}" > "${1}"
rm "${res}"
