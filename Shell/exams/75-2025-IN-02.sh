#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! "${1}" =~ ^[a-zA-Z0-9]+\.[a-zA-Z0-9]+$ ]]; then
    echo "invalid domain"
    exit 1
fi

if [[ ! -f "${2}" ]]; then
    echo "file does not exist"
    exit 1
fi

while read artist; do

    echo "; team ${artist}"
    while read composer; do
        while read hostname; do
            address="${hostname}.${1}"
            echo "${composer} IN NS ${address}."
        done < <(cat "${2}" | tr '\t' ' ' | tr -s ' ' | awk -v a="${artist}" -v c="${composer}" '$3==a && $2==c {print $1}')
    done < <(cat "${2}" | tr '\t' ' ' | tr -s ' ' | awk -v a="${artist}" '$3==a {print $2}')
done < <(cat "${2}" | tr '\t' ' ' | tr -s ' ' | cut -d ' ' -f3 | sort | uniq)
