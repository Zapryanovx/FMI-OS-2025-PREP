#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected."
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "file does not exist."
    exit 1
fi

cat ${1} | cut -d '-' -f2 | cut -c 2- \
| awk '{printf "%d. %s\n", NR, $0}' \
| sort -t ' ' -k2

