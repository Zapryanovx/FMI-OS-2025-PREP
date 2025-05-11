#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "one arg expected"
    exit 1
fi

NAME="^[A-Z0-9]{1,4}$"
COL3TEMP="^\*(enabled|disabled)$"

if [[ ! $1 =~ $NAME ]]; then
    echo "invalid device"
    exit 1
fi

row=$(awk -v device=$1 'device==$1 {print $0}' example-wakeup)
if [[ -z "${row}" ]]; then
    echo "device not found"
    exit 1
fi

state=$(echo "${row}" | awk '{print $3}')
if [[ ! "${state}" =~ $COL3TEMP ]]; then
    echo "invalid state"
    exit 1
fi

if [[ "${state}" == "*enabled" ]]; then
    echo "$1" > example-wakeup
    echo "disabling"
fi

