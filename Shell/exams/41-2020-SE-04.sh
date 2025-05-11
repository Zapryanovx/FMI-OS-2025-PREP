#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "src doesn't exist"
    exit 1
fi

if [[ -d $2 ]]; then
    echo "dst already exists"
i    exit 1
fi

mkdir -p "${2}"

while read file; do
    basename=$(basename "${file}" ".jpg")

    name=$(echo "${basename}" | sed -E "s/\([^)]*\)//g" | tr -s ' ' | sed "s/^ //g" | sed "s/ $//g")   
    album=$(echo "(misk) ${basename}" | grep -E -o "\([^)]+\)" | tail -n 1 | cut -c 2- | rev | cut -c 2- | rev)
    date=$(stat -c "%y" "${file}" | cut -d ' ' -f1)
    hash=$(sha256sum "${file}" | cut -c 1-16)

    mkdir -p "${2}/images/"
    mkdir -p "${2}/by-date/${date}/by-album/${album}/by-title/"
    mkdir -p "${2}/by-date/${date}/by-title/"
    mkdir -p "${2}/by-album/${album}/by-date/${date}/by-title/"
    mkdir -p "${2}/by-album/${album}/by-title/"
    mkdir -p "${2}/by-title/"

    cp "${file}" "${2}/images"
    mv "${2}/images/${basename}.jpg" "${2}/images/${hash}.jpg"

    cpyPath="${2}/images/${hash}.jpg"    
    ln -s "${cpyPath}" "${2}/by-date/${date}/by-album/${album}/by-title/${name}.jpg"
    ln -s "${cpyPath}" "${2}/by-date/${date}/by-title/${name}.jpg"
    ln -s "${cpyPath}" "${2}/by-album/${album}/by-date/${date}/by-title/${name}.jpg"
    ln -s "${cpyPath}" "${2}/by-album/${album}/by-title/${name}.jpg"
    ln -s "${cpyPath}" "${2}/by-title/${name}.jpg"

done < <(find "${1}" -type f -name "*.jpg")
