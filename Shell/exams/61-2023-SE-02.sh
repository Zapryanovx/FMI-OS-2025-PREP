#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "expected time limit and cmd"
    exit 1
fi

if [[ ! "${1}" =~ ^[0-9]+$ ]]; then
    echo "1st arg must be num"
    exit 1
fi

time_limit="${1}"
starting_point=$(date +"%s.%N")

shift 1
cmd="$*"

count=0
diff=0
while true; do
    bash -c "${cmd}" > /dev/null
    curr=$(date +"%s.%N")

    diff=$(echo "$curr - $starting_point" | bc -l)
    count=$(( $count + 1 ))

    if (( $(echo "${diff} >= ${time_limit}" | bc) )); then
        break;
    fi
done 

echo "Ran the command '${cmd}' ${count} for $(echo "${diff}" | bc -l) seconds."
echo "Average runtime: $(echo "${diff} / ${count}" | bc -l) seconds."

