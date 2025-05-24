#!/bin/bash

psFile=$(mktemp)
table=$(mktemp)
counter=0

while true; do
    (ps -e -o pid=,rss=,comm= | tr -s ' ' | sed -E "s/^[ ]+//g") > "${table}"
    
    found="false"
    counter=$((counter + 1))
    
    while read -r comm; do
        sum=$(cat "${table}" | awk -v cmd="${comm}" '$3==cmd {print $2}' | awk 'BEGIN{sum=0} {sum+=$1} END{print sum}')
        if [[ "${sum}" -gt 65536 ]]; then
            found="true"
            echo "${comm}" >> "${psFile}"
        fi
    done < <(cat "${table}" | cut -d ' ' -f3 | sort | uniq)

    if [[ "${found}" == "false" ]]; then
        break;
    fi

    sleep 1

done

cat "${psFile}" | sort | uniq -c | awk -v cnt="${counter}" '$1>=cnt/2 {print $2}'

rm "${psFile}"
rm "${table}"


