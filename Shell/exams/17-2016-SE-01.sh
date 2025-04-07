#!/bin/bash

if [[ "$#" -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! -d "$1" ]]; then
    echo "dir expected"
    exit 1
fi

for link in $(find "$1" -type l); do
    if [[ ! -f "$link" ]]; then
        echo "$link"
    fi
done
