#!/bin/bash

# if [[ "${whoami}" != "root" ]]; then
#    echo "script must be executed by root"
#    exit 1
# fi

if [[ $# -ne 3 ]]; then
    echo "3 args expected"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "first arg must be dir"
    exit 1
fi

if [[ ! -d $2 ]]; then
    echo "second arg must be dir"
    exit 1
fi

objsInDir2=$(find "${2}" -mindepth 1 -type f | wc -l)
if [[ "${objsInDir2}" -ne 0 ]]; then
    echo "second dir must be empty"
    exit 1
fi

while read path; do
    echo "${path}"
    fileName=$(echo "${path}" | awk -F '/' '{print $NF}')
    echo "${fileName}" | grep -q -E "${3}"
    if [[ ${?} -eq 0 ]]; then
        mv "${path}" "${2}/${fileName}"
    fi
done < <(find ${1} -mindepth 1 -type f)


