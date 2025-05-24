#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

val=$(${1})
if [[ $? -ne 0 ]]; then
    exit 3
fi

date=$(date +"%A %Y-%m-%d %H:%M:%S" | tr -s ' ')

day=$(echo "${date}" | cut -d ' ' -f1)
hour=$(echo "${date}" | cut -d ' ' -f3 | cut -d ':' -f1)

sum=0
cnt=0

while read line; do
    res=$(echo "$line" | cut -d',' -f2)
    sum=$(echo "$sum + $res" | bc)
    cnt=$(($cnt + 1))
done < <(grep "^${day} " "${2}" | grep " ${hour}:")

avg=0
if [[ "${cnt}" -ne 0 ]]; then
    avg=$(echo "${sum}/${cnt}" | bc)
fi

echo "${date},${val}" >> "${2}"

if [ "$(echo "${val} > 2 * ${avg}" | bc)" -eq 1 ] || \
   [ "$(echo "${val} < ${avg} / 2" | bc)" -eq 1 ]; then

    output=$(echo "${date}" | cut -d ' ' -f2- | cut -d ':' -f1)
    echo "${output}:${val} abnormal"
    exit 2
fi

