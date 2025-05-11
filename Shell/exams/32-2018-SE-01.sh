#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "one arg expected"
    exit 1
fi

if [[ ! -d ${1} ]]; then
    echo "invalid dir"
    exit 1
fi

logs=$(find ${1} -mindepth 4 -maxdepth 4 -type f -printf "%h %f\\n")
friends=$(find ${1} -mindepth 3 -maxdepth 3 -type d | awk -F '/' '{print $NF}' | sort | uniq)

results=$(mktemp)
while read -r friend log; do
    lines=$(cat "${friend}/${log}" | wc -l)    
    echo "$(basename "${friend}") ${lines}" >> "${results}"
done < <(echo "${logs}")

totalChats=$(mktemp)
while read -r friend; do
    total=$(cat "${results}" | grep -E "^${friend}" | awk '{sum+=$2} END{print sum}')
    echo "${friend} ${total}" >> "${totalChats}"
done < <(echo "${friends}")

cat "${totalChats}" | sort -k 2nr | head 

rm "${results}"
rm "${totalChats}"


