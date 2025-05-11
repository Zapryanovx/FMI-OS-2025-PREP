#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "file does not exist"
    exit 1
fi

while read site; do
    http20=$(cat "${1}" | grep -E "${site}" | grep -E "HTTP/2\.0" | wc -l)
    nonhttp20=$(cat "${1}" | grep -E "${site}" | grep -E -v "HTTP/2\.0" | wc -l)

    echo "${site} HTTP/2.0: ${http20} non-HTTP/2.0: ${nonhttp20}"
done < <(cat "${1}" | cut -d ' ' -f2 | sort | uniq -c | tr -s ' ' | cut -c 2- | cut -d ' ' -f2- | head -n 3)

cat "${1}" | cut -d ' ' -f1,9 | awk '$2 > 302{print $1}' | sort | uniq -c | sort -k 1nr
