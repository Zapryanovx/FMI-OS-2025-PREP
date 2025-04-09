#!/bin/bash

# if [[ "$(whoami)" != "root" ]]; then
#    echo "script must be executed by root"
#    exit 1
# fi

if [[ $# -ne 0 ]]; then
    echo "no args expected"
    exit 1
fi

table=$(mktemp)
ps -e -o user=,pid=,rss= | tr -s ' ' >> "${table}"

#A)
while read user; do
    processes=$(cat "${table}" | grep -E "${user}")
    echo "${processes}" | awk -v user="${user}" '{memo+=$3} END{printf "%s %d %d\n", user, NR, memo}'
done < <(cat "${table}" | cut -d ' ' -f1 | sort | uniq)


#B)

while read user; do
    processes=$(cat "${table}" | grep -E "^${user}" | sort -t ' ' -k 3nr)
    biggest=$(echo "${processes}" | head -n 1)

    sum=$(echo "${processes}" | awk '{memo+=$3} END{print memo}')
    cnt=$(echo "${processes}" | wc -l)

    avg=$(echo "scale=2; $sum / $cnt" | bc)
    rss=$(echo "$biggest" | awk '{print $3}')

    if (( $(echo "$rss > 2 * $avg" | bc -l) )); then
        echo "kill $(echo "$biggest" | awk '{print $2}')"
    fi

done < <(cat "${table}" | cut -d ' ' -f1 | sort | uniq)


rm "${table}"
