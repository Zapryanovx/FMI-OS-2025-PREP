#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "file doesnt exist"
    exit 1
fi

wakeup="./2022-SE-01/example-wakeup"

if [[ ! -f "${wakeup}" ]]; then
    echo  "pisna mi"
fi

while read line; do
    device=$(echo "${line}" | awk '{print $1}')
    turn_to=$(echo "${line}" | awk '{print $2}')

    if [[ ! "${device}" =~ ^[A-Z0-9]{0,4}$ ]]; then
        echo "invalid device found - ${device}"
        continue
    fi

    if ! grep -q "${device}" "${wakeup}"; then
        echo "device not found - ${device}"
        continue
    fi

    if [[ "${turn_to}" == "disabled" ]]; then
        new_line=$(grep -E "${device}.*" "${wakeup}" | sed -E "s/\*enabled/\*${turn_to}/g")
    else
        new_line=$(grep -E "${device}.*" "${wakeup}" | sed -E "s/\*disabled/\*${turn_to}/g")
    fi

    sed -i -E "s/^[ ]*${device}.*/${new_line}/g" "${wakeup}"

done < <(cat "${1}" | grep -v "^#.*" | sed -E "s/#.*//g")

