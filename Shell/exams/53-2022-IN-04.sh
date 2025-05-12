
#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "dir expected"
    exit 1
fi

if [[ ! -f "${1}/foo.pwd" ]]; then
    echo "foo.pwd doesnt exist"
    exit 1
fi

if [[ ! -f "${1}/foo.cfg" ]]; then
    touch "${1}/foo.cfg"
else
    > "${1}/foo.cfg"
fi

if [[ ! -d "${1}/cfg" ]]; then
    echo "dir cfg doesnt exist"
    exit 1
fi

while read path; do
    user=$(basename "${path}" | cut -d '.' -f1)
    res=$(mktemp)

    ./${1}/validate.sh "${path}" >> "${res}"

    if [[ $? -eq 0 ]]; then
        cat "${res}" >> "${1}/foo.cfg"

        if ! grep -E "^${user}:.*$" "${1}/foo.pwd"; then
            passwd=$(pwgen 16 1)
            crypted=$(mkpasswd "${passwd}")

            echo "${user}:${crypted}" >> "${1}/foo.cfg"
            echo "${user}:${passwd}"
        fi
    elif [[ $? -eq 1 ]]; then
        prefix="$(basename ${path}): "
        cat "${res}" | awk -v pref="${prefix}" '{print pref $0}' 1>&2

    elif [[ $? -eq 2 ]]; then
        echo "err while executing validate.sh"
    fi
done < <(find "${1}/cfg" -type f -name "*.cfg")

