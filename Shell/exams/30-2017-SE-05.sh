#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "first arg must be a dir"
    exit 1
fi

find "${1}" -mindepth 1 -maxdepth 1 -type f -printf "%f\n" 2>/dev/null \
| grep -E "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${2}$" \
| sort -V -r \
| head -n 1

