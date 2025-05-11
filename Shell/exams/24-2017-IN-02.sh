#!/bin/bash


if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

# if [[ "${whoami}" != "root" ]]; then
#    echo "script must be executed by root"
#    exit 1
#fi

processes=$(mktemp)
(ps -e -o user=,pid=,time= | tr -s ' ' | sort) >> "${processes}"


fooCnt=$(cat "${processes}" | awk -v user="${1}" '$1==user{print $1}' | wc -l)

#A)

while read cnt user; do
    if [[ $cnt -gt $fooCnt ]]; then
        echo "${user}"
    fi
done < <(cat "${processes}" | cut -d ' ' -f1 | uniq -c | tr -s ' ' | cut -c 2-)

#B)

while read user; do
    curr=$(cat ${processes} | grep -E "${user}" | cut -d ' ' -f3 | awk -F ':' '{res+=($1*3600)+($2*60)+$3} END{print res}')
    sum=$(( ${sum} + ${curr}))
done < <(cat "${processes}" | cut -d ' ' -f1 | uniq)

total=$(cat ${processes} | wc -l)
avg=$(echo "scale=2;${sum}/${total}" | bc)
echo "${avg}"

#C)

while read user pid time; do
    secs=$(echo "${time}" | awk -F ':' '{print ($1*3600)+($2*60)+$3}')
    if [[ $(echo "${secs} > ${avg}/2" | bc) -eq 1 ]]; then
        echo "kill ${pid}"
    fi
done < <(cat "${processes}" | grep -E "^${1}")

rm "${processes}"

