#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 file names expected"
    exit 1
fi

if [[ ! -f $1 ]] || [[ ! -f $2 ]]; then
    echo "invalid file found"
    exit 1
fi

lhsLines=$(cat ${1} | grep -E "${1}" | wc -l)
rhsLines=$(cat ${2} | grep -E "${2}" | wc -l)

function save(){
    cat ${1} | cut -d '-' -f2 | cut -c 2- | sort >> "${1}.songs"
}

if [[ ${lhsLines} -gt ${rhsLines} ]]; then
    save $1
else
    save $2
fi
