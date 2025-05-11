#!/bin/bash

# if [[ "$(whoami)" != "root" ]]; then
#     echo "script must be executed by root"
#     exit 1
# fi

table=$(mktemp)
ps -e -o user=,pid=,rss= | tr -s ' ' | grep -E -v "root" >> "${table}"

function checkRSS {
    rootProcesses=$(ps -e -o user=,pid=,rss= | tr -s ' ' | grep -E "root")
    rootRss=$(echo "${rootProcesses}" | awk '{sum+=$3} END{print sum}')

    targetProcesses=$(cat "${table}" | grep -E "^${1}")
    targetRss=$(echo "${targetProcesses}" | awk '{sum+=$3} END{print sum}')

    if [[ ${targetRss} -gt ${rootRss} ]]; then
        echo "killall -u ${1}"
    fi
}

while read user; do

    homeDir=$(cat /etc/passwd | grep -E "^${user}" | cut -d ':' -f6)
    if [[ ! -d "${homeDir}" ]]; then
        cat "${table}" | grep -E "^${user}"
        checkRSS "${user}"
        continue
    fi

    homeOwner=$(cat /etc/passwd | grep -E "^${user}" | cut -d ':' -f6 | xargs -I{} stat -c %U {})
    if [[ "${homeOwner}" != "${user}" ]]; then
        cat "${table}" | grep -E "^${user}"
        checkRSS "${user}"
        continue
    fi

    permsDir=$(cat /etc/passwd | grep -E "^${user}" | cut -d ':' -f6 | xargs -I{} stat -c %A {})
    if [[ "${permsDir:2:1}" != "w" ]]; then
        cat "${table}" | grep -E "^${user}"
        checkRSS "${user}"
        continue
    fi

done < <(cat "${table}" | cut -d ' ' -f1 | sort | uniq)

rm "${table}"

