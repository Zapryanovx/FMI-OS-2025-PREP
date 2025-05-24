#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ -f "${1}" ]]; then
    echo "file already exists"
    exit 1
fi

bake_file="./bakefile"
if [[ ! -f "${bake_file}" ]]; then
    echo "./bakefile should exist"
    exit 1
fi

while IFS=: read file dependancy command; do
    for dep in $dependancy; do
        if grep -q -E "^${dep}:" "${bake_file}"; then
            ./2024-SE-04.sh "${dep}"
        elif [[ ! -f "${dep}" ]]; then
            echo "file doesnt exist"
            exit 1
        fi
    done

    eval "${command}"

done < <(cat "${bake_file}" | grep -E "^${1}:")

