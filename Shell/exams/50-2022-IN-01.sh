#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "arg 1 must be a dir"
    exit 1
fi

if [[ ! -d "${2}" ]]; then
    echo "arg 2 must be a dir"
    exit 1
fi

cnt=$(find "${2}" -mindepth 1 | wc -l)
if [[ ${cnt} -ne 0 ]]; then
    echo "arg 2 must be an empty dir"
    exit 1
fi

dirs=$(find "${1}" -mindepth 1 -type f)

while read path; do
    
    name=$(basename "${path}")
    if echo "${name}" | grep -E -q "^\..*\.swp$"; then
        cleared=$(echo "${name}" | cut -d '.' -f2- | rev | cut -d '.' -f2- | rev)
        if echo "${dirs}" | grep -E -q "/${cleared}$"; then
            continue
        fi
    fi
    
    cpyTo="${2}/$(echo "${path}" | cut -d '/' -f2-)"

    mkdir -p "$(dirname "$cpyTo")"
    cp ${path} ${cpyTo}

done < <(echo "${dirs}")

