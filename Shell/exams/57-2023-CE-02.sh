#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "3 arg expected"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "1st file not found"
    exit 1
fi

if [[ ! -f "${2}" ]]; then
    echo "2nd file not found"
    exit 1
fi

lhs_dist=$(cat "${1}" | grep -E "^${3}: [1-9]+[0-9]* megaparsecs$" | cut -d ':' -f2 | sed -E 's/^[ ]*//g' | cut -d ' ' -f1)
rhs_dist=$(cat "${2}" | grep -E "^${3}: [1-9]+[0-9]* megaparsecs$" | cut -d ':' -f2 | sed -E 's/^[ ]*//g' | cut -d ' ' -f1)

if [[ "${lhs_dist}" -lt "${rhs_dist}" ]]
then
    echo "${1}"
elif [[ ${lhs_dist} -gt "${rhs_dist}" ]]
then
    echo "${2}"
else
    echo "equal"
fi
