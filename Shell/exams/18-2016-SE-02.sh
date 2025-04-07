!/bin/bash

if [[ "$#" -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! "$1" =~ ^[1-9][0-9]*$ ]]; then
    echo "arg1 must be a number"
    exit 1
fi

if [[ $(whoami) != "root" ]]; then
    echo "script must be run by root"
    exit 1
fi

while read user; do
    sum=$(ps -e -o user,rss | grep -E "^${user}" | tr -s ' ' | awk -F ' ' 'BEGIN{s=0} {s+=$2} END{print s}')
    if [[ $sum -gt $1 ]]; then
        killpid="$(ps -e -o user,rss,pid | grep -E "^${user}" | tr -s ' ' | sort -t ' ' -k3nr | head -n 1 | cut -d ' ' -f3)"
        echo "${killpid} was killed."
        kill "${killpid}"
    fi
done < <(ps -e -o user= | sort | uniq)

