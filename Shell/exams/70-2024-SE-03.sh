#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! "${1}" =~ ^(0|[1-9][0-9]*)$ ]]; then
    echo "num expected"
    exit 1
fi

tones=$(mktemp)
echo "A Bb" >> "${tones}"
echo "Bb B" >> "${tones}"
echo "B C" >> "${tones}"
echo "C Db" >> "${tones}"
echo "Db D" >> "${tones}"
echo "D Eb" >> "${tones}"
echo "Eb E" >> "${tones}"
echo "E F" >> "${tones}"
echo "F Gb" >> "${tones}"
echo "Gb G" >> "${tones}"
echo "G Ab" >> "${tones}"
echo "Ab A" >> "${tones}"

input=$(mktemp)
while read -r line; do
    if [[ -z "${line}" ]]; then
        break
    fi

    echo "${line}" >> "${input}"
done

N=$((${1} % 12 ))

crypt="sha256sum123"
while (( N > 0 )); do

    while read tone next; do
        sed -i -E "s/\[${tone}([^]]*)\]/[${crypt}${next}\1]/g" "${input}"
    done < <(cat "${tones}")
    
    sed -i -E "s/${crypt}//g" "${input}"
    N=$(( $N - 1))
done

cat "${input}"
rm "${input}"
