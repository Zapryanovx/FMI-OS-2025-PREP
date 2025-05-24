#!/bin/bash

if [[ $# -ne 0 ]]; then
    echo "no args expected"
    exit 1
fi

if [[ -z "${PASSWD}" ]]; then
    PASSWD="/etc/passwd"
fi

local_users=$(mktemp)
(cat "${PASSWD}" | awk -F ':' '$3>=1000 {print $1}' | sort) > "${local_users}"

cloud_users=$(mktemp)
(./occ user:list | cut -d ':' -f1 | sed -E "s/^[ ]*- //g" | sort) > "${cloud_users}" 

while read user; do
    ./occ user:add "${user}"
done < <(comm -23 "${local_users}" "${cloud_users}")

while read user; do
    if ./occ user:info "${user}" | grep -Eq "- enabled: false"; then
        ./occ user:enable "${user}"
    fi
done < <(comm -12 "${local_users}" "${cloud_users}")

while read user; do
    if ./occ user:info "${user}" | grep -Eq "- enabled: true"; then
        ./occ user:disable "${user}"
    fi
done < <(comm -13 "${local_users}" "${cloud_users}")

rm "${local_users}"
rm "${cloud_users}"
