#!/bin/bash

if [[ "${#}" -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "1st arg must be a dir"
    exit 1
fi

if [[ ! -d "${2}" ]]; then
    echo "2nd arg must be a dir"
    exit 1
fi

packageName=$(basename "${2}")
packageVersion=$(cat "${2}/version" | head -n 1)
	
tar -caf temp.tar.xz "${2}/tree"
checkSum=$(sha256sum temp.tar.xz | cut -d ' ' -f1)
mv temp.tar.xz "${1}/packages/${checkSum}.tar.xz"

if ! grep -q -E "^${packageName}-${packageVersion}.*$" "${1}/db"; then
	echo "${packageName}-${packageVersion} ${checkSum}" >> "${1}/db"
else
	old=$(grep -E "^${packageName}-${packageVersion}.*$" "${1}/db" | rev | cut -d ' ' -f1 | rev)
	rm "${1}/packages/${old}.tar.xz"
	sed -i -E "s/^${packageName}-${packageVersion}.*$/${packageName}-${packageVersion} ${checkSum}/" "${1}/db"
fi

