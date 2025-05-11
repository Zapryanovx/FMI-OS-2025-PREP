#!/bin/bash


if [[ $# -ne 3 ]]; then
    echo "3 args expected"
    exit 1
fi

if [[ ! $1 =~ [1-9]+[0-9]*\.[0-9]+ ]]; then
    echo "arg 1 must be a num"
    exit 1
fi

if [[ ! $2 =~ [a-zA-Z] ]]; then
    echo "arg 2 must be a letter"
    exit 1
fi

if [[ ! $3 =~ [a-zA-Z] ]]; then
    echo "arg 3 must be a letter"
    exit 1
fi

multi=$(cat prefix.csv | awk -F ',' -v uint="${2}" '$2==uint {print $3}')

name=$(cat base.csv | awk -F ',' -v uname="${3}" '$2==uname {print $1}')
measure=$(cat base.csv | awk -F ',' -v uname="${3}" '$2==uname {print $3}')

res=$(echo "${multi} * ${1}" | bc )
echo "${res} $3 (${measure}, ${name})"
