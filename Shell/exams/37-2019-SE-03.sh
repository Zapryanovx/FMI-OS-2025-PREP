#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "one arg expected"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "dir expected"
    exit 1
fi

mkdir "extracted"

lastRun=$(stat -c "%X" "${0}")

while read archive; do
    name=$(basename "${archive}" | cut -d '_' -f1)
    timestamp=$(basename "${archive}" | cut -d '_' -f2 | cut -d '-' -f2 | cut -d '.' -f1)

    while read file; do
        if [[ $(basename "${file}") == "meow.txt" ]]; then
            tar -xaf "${archive}" "${file}" 
            mv "${file}" "extracted/${name}_${timestamp}.txt"

            #tar --delete -f "${archive}" "${file}"
        fi
    done < <(tar -tf "${archive}")

done < <(find "${1}" -type f -printf "%p %T@\n" | grep -E "[^_]+_report-[1-9][0-9]*\.tgz" | awk -v lr="${lastRun}" '$2 > lr {print $1}')

