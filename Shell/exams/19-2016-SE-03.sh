#!/bin/bash

if [[ $# -ne 0 ]]; then
    echo "no args expected"
    exit 1
fi

#if [[ $(whoami) != "root" ]]; then
#    echo "script must be executed by root"
#    exit 1
#fi

users=$(mktemp)
(cat /etc/passwd | grep -E -v "home" | cut -d ':' -f1) >> "${users}"

while IFS=: read user dir; do
    perm=$((stat -c "%a" "${dir}"): -3)
    canWrite=$((( $perm / 100) & 2))
    if [[ $canWrite -eq 0 ]]; then
        printf "${user} - ${perm}"
        echo "${user}" >> "${users}"
    fi
done < <(cat /etc/passwd | grep -E "home" | cut -d ':' -f1,6)

cat "${users}" | sort
rm "${users}"
