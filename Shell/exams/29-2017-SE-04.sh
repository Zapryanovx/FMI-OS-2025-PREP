#!/bin/bash

if [[ $# -lt 1 ]] || [[ $# -gt 2 ]]; then
    echo "one or two args expected"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "first arg must be a dir"
    exit 1
fi

brokenLinks=0


for link in $(find "${1}" -type l 2>/dev/null); do
    if [[ ! -e "${link}" ]]; then
        brokenLinks=$(( brokenLinks + 1))
    else
        if [[ $# -eq 1 ]]; then
            echo "${link} -> $(readlink -f ${link})"
        else
            echo "${link} -> $(readlink -f ${link})" >> "${2}"
        fi
    fi
done

if [[ $# -eq 1 ]]; then
    echo "Broken links: ${brokenLinks}"
else
    echo "Broken links: ${brokenLinks}" >> "${2}"
fi

