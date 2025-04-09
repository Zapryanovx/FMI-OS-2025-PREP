#!/bin/bash


if [[ $# -ne 0 ]]; then
    echo "no args expected"
    exit 1
fi

nums=$(mktemp)
while read line; do
    if [[ -z ${line} ]]; then
        break;
    fi

    if echo "${line}" | grep -E -q '^-?[1-9][0-9]*$'; then
        echo "${line}" >> "${nums}"
    fi
done

#A)

maxAbs=0
while read num; do
    if [[ "${num}" -gt "${maxAbs}" ]]; then
        maxAbs="${num}"
    elif [[ $((num * -1)) -gt "${maxAbs}" ]]; then
        maxAbs=$((num * -1))
    fi
done < <(cat "${nums}")

uniqNums=$(mktemp)
while read num; do
    if [[ "${num}" -eq "${maxAbs}" ]]; then
        echo "${num}" >> "${uniqNums}"
    elif [[ $((num * -1)) -eq "${maxAbs}" ]]; then
        echo "${num}" >> "${uniqNums}"
    fi
done < <(cat "${nums}")

cat "${uniqNums}" | sort | uniq
rm "${uniqNums}"
#B)

sums=$(mktemp)
while read num; do
    sum=$(echo "${num}" | grep -E -o "[0-9]" | awk '{sum += $1} END {print sum}')
    echo "${num} ${sum}" >> "${sums}"
done < <(cat "${nums}")

cat "${sums}" | sort -t ' ' -k 2nr -k 1n | head -n 1 | cut -d ' ' -f1
rm "${sums}"
