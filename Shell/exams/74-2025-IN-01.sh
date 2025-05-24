#!/bin/bash

#if [[ $(whoami) != "root" ]]; then
#    echo "script must be executed by root!"
#    exit 1
#fi

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "file expected"
    exit 1
fi

function changePerms() {
    if [[ -d "${1}" ]]; then
        chmod 755 "${1}"
    elif [[ -f "${1}" ]]; then
        chmod 664 "${1}"
    fi
}

while read dir type perm; do
    while read path bits; do
        if [[ "${type}" == "R" ]] && \ 
           [[ "${perm}" -eq "${bits}" ]]; then    
           changePerms "${path}"
        elif [[ "${type}" == "A" ]] && \
            [[ $(echo $(( ${perm} & ${bits} )) ) -ne 0 ]]; then
            changePerms "${path}"
        elif [[ "${type}" == "T" ]] && \
            [[ $(echo $(( ${perm} & ${bits} )) ) -eq "${perm}" ]]; then
            changePerms "${path}"
        fi
    done < <(find "${dir}" -mindepth 1 \( -type f -o -type d \) 2>/dev/null -printf "%p %m\n")
done < <(cat "${1}")

