#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "1st arg must be a file"
    exit 1
fi

if [[ ! "${1}" =~ .*\.csv ]]; then
    echo "1st file must be csv"
    exit 1
fi

constellation=$(cat "${1}" | grep -E ",${2},.*,.*$" | cut -d ',' -f4 | sort | uniq -c | sort -nr | sed -E "s/^[ ]*//g" | cut -d ' ' -f2)
star=$(cat "${1}" | grep -E ",${constellation},.*,.*,.*$" | sort -t ',' -k 7nr | head -n 1 | cut -d ',' -f1)
echo "${star}"

