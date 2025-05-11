#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "at least 1 arg expected"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "dir does not exist"
    exit 1
fi

if [[ $# -eq 1 ]]; then
    for link in $(find "${1}" -type l); do
        if [[ ! -e "${link}" ]]; then
            echo "${link} is broken"
        fi
    done
elif [[ $# -eq 2 ]]; then
    if [[ ! $2 =~ ^[1-9][0-9]*$ ]]; then
        echo "expected num at pos 2"
        exit 1
    fi

    find "${1}" -type f -printf "%p,%n\n" 2>/dev/null | awk -F ',' -v target="${2}" '$2>=target{print $1}'
else
    echo "1 or 2 args expected"
fi

