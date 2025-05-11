#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "3 args expected"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "1 arg must be a file"
    exit 1
fi

if [[ ! -f $2 ]]; then
    echo "2 arg must be a file"
    exit 1
fi

if [[ ! ${2} =~ \.cfg$ ]]; then
    echo "2 file must be .cfg format"
    exit 1
fi

if [[ ! -d $3 ]]; then
    echo "3 arg must be a dir"
    exit 1
fi

while read file; do
    
    fileName=$(basename "${file}")
    at=1
    errors=$(mktemp)
    valid=1
    
    while read line; do
        if echo "${line}" | grep -E -q -v "^(#.*)|(\{[^}]*\};)$"; then
            echo "Line ${at}:${line}" >> "${errors}"      
            valid=0
        fi
       
        at=$(( at + 1))
    done < <(cat "${file}")
    
    if [[ "${valid}" -eq 0 ]]; then
        echo "Error in ${file}"
        cat "${errors}"
    else
        user=$(basename "${file}" ".cfg")
        cat "${file}" >> "${2}"

        if ! grep -E -q "^${user}" "${1}"; then
            pwd=$(pwgen 16 1)
            echo "${user}:${pwgen}" >> "${1}"
            echo "${user} ${pwgen}"
        fi
    fi
    
    rm "${errors}"

done < <(find "${3}" -mindepth 1 -type f -name "*.cfg")
