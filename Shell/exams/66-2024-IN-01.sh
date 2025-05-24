#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -gt 3 ]]; then
    echo "1 to 3 args expected"
    exit 1
fi

#if [[ -z "${ARKCONF}" ]]; then
#    echo "Env var doesn't exist!"
#    exit 1
#fi

kvp="${ARKCONF}"

is_push="f"
is_pull="f"
is_d="f"

server=""
is_valid_server="f"

for arg in "${@}"; do
    if [[ "${arg}" == "push" ]]; then
        is_push="t"
    elif [[ "${arg}" == "pull" ]]; then
        is_pull="t"
    elif [[ "${arg}" == "-d" ]]; then
        is_d="t"
    else
        server="${arg}"
    fi
done

if [[ "${is_push}" == "f" && "${is_pull}" == "f" ]]; then
    echo "push or pull expected"
    exit 1
fi

if [[ "${is_push}" == "t" && "${is_pull}" == "t" ]]; then
    echo "one of push or pull options should be selected"
    exit 1
fi

if [[ "${server}" =~ ^a[a-z]+$ ]]; then
    is_valid_server="t"
fi

where=$(mktemp)
(cat "${kvp}" | grep -E "^WHERE=" | cut -d '=' -f2 | tr -d '"' | tr ' ' '\n') > "${where}"

what=$(cat "${kvp}" | grep -E "^WHAT=" | cut -d '=' -f2 | tr -d '"')
who=$(cat "${kvp}" | grep -E "^WHO=" | cut -d '=' -f2 | tr -d '"')

if [[ "${is_valid_server}" == "t" ]]; then
    echo "${server}" > "${where}"
fi

while read -r server; do
    if [[ "${is_pull}" == "t" ]]; then
        rsync -n -v -a "${who}@${server}:${what}" "${what}"
    else
        rsync -n -v -a "${what}" "${who}@${server}:${what}"
    fi
done < <(cat "${where}")

read -p "Wanna sync?" ans

if [[ "${ans}" == "n" ]]; then
    rm "${where}"
    exit 0
fi

while read -r server; do
    if [[ "${is_pull}" == "t" ]]; then
        if [[ "${is_d}" == "t" ]]; then
            rsync -a -v --delete "${who}@${server}:${what}" "${what}"
        else
            rsync -a -v "${who}@${server}:${what}" "${what}"
        fi
    else    # push
        if [[ "${is_d}" == "t" ]]; then
            rsync -a -v --delete "${what}" "${who}@${server}:${what}"
        else
            rsync -a -v "${what}" "${who}@${server}:${what}"
        fi
    fi
done < <(cat "${where}")

