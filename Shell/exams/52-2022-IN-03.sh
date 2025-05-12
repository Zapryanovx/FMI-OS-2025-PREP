#!/bin/bash

options=$(mktemp)
filename=""
args=$(mktemp)

flagJar=false
flagJarFile=false

for arg in $@; do
    if [[ "${arg}" == "-jar" ]]; then
        flagJar=true
    elif [[ "${flagJar}" == "true" && -f "${arg}" ]]; then
        flagJarFile=true
        filename="${arg}"
    elif [[ "${flagJar}" == "true" && "${arg}" =~ ^-.+ ]]; then
        echo "${arg}" >> "${options}"
    elif [[ "${flagJarFile}" == "true" ]]; then
        echo "${arg}" >> "${args}"
    fi
done

if [[ "${flagJar}" == "false" ]]; then
    echo "No -jar flag"
    exit 1
fi

if [[ "${flagJarFile}" == "false" ]]; then
    echo "No jar file"
    exit 1
fi

res=$(mktemp)
cat "${options}" >> "${res}"
echo "-jar" >> "${res}"
echo "${filename}" >> "${res}"
cat "${args}" >> "${res}"
cmd=$(cat "${res}" | tr -s '\n' ' ')

java "${cmds}"

rm "${args}"
rm "${options}"
rm "${res}"
