#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "3 args expected"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "not a valid file"
    exit 1
fi

lhs=$(cat ${1} | grep -E "^${2}=" | cut -d '=' -f2- | tr ' ' '\n')
rhs=$(cat ${1} | grep -E "^${3}=" | cut -d '=' -f2- | tr ' ' '\n')

result=""
while read el; do
    echo "${lhs}" | grep -E -q "${el}"
    if [[ ${?} -ne 0 ]]; then
        echo "here"
        result+=$(echo "${el} ")
    fi
done < <(echo "${rhs}")

sed -i -E "s/^${3}=.*/${3}=${result}/" ${1}

