#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "first arg is not a dir"
    exit 1
fi

if [[ ! -d $2 ]]; then
    echo "second arg is not a dir"
    exit 1
fi

packageName=$(basename "${2}")
version=$(cat "${2}/version" | head -n 1)

if grep -E -q "^${packageName}-${version}$" "${1}/db"; then
    checkSum=$(cat "${1}/db" | grep -E "^${packageName}-${version}" | cut -d ' ' -f2)
    rm "${1}/packages/${checkSum}.tar.xz"
    tar -caf temp.tar.xz "${2}/tree"
    newCheckSum=$(sha256sum temp.tar.xz | cut -d ' ' -f1)
    mv temp.tar.xz "${1}/packages/${newCheckSum}.tar.xz"
    sed -E "s/^${packageName}-${version}.*/${packageName}-${version} ${newCheckSum}" "${1}/db"
else
    tar -caf temp.tar.xz "${2}/tree"
    checkSum=$(sha256sum temp.tar.xz | cut -d ' ' -f1)
    mv temp.tar.xz "${1}/packages/${checkSum}.tar.xz"
    echo "${packageName}-${version} ${checkSum}" >> "${1}/db"
    
    tempFile=$(mktemp)
    cat "${1}/db" > "${tempFile}"
    cat "${tempFile}" | sort > "${1}/db"
    rm "${tempFile}"
fi
