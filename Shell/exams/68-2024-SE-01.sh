#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "at least 2 args needed"
    exit 1
fi

files=$(mktemp)
replace=$(mktemp)

for arg in "${@}"; do
    if [[ "${arg}" =~ ^-R.*=.*$ ]]; then
        echo "${arg}" | cut -c 3- >> "${replace}"
    else
        echo "${arg}" >> "${files}"
    fi
done

crypt="sha256sum123"
while read file; do 
    while read pair; do
        from=$(echo "${pair}" | cut -d '=' -f1)
        sed -i -E "/^[ ]*#/! s/\\<${from}\\>/${from}${crypt}/g" "${file}"
    done < <(cat "${replace}")

    while read pair; do
        from=$(echo "${pair}" | cut -d '=' -f1)
        to=$(echo "${pair}" | cut -d '=' -f2)
        sed -i -E "/^[ ]*#/! s/\\<${from}${crypt}\\>/${to}/g" "${file}"
    done < <(cat "${replace}")
done < <(cat "${files}")

rm "${files}"
rm "${replace}"
