#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "1 arg expected"
    exit 1
fi

if [[ ! ${1} =~ ^[0-9]+$ ]]; then
    echo "number expected"
    exit 1
fi

if [[ ${1} -lt 2 ]]; then
    echo "number must be >= 2"
    exit 1
fi

#if [[ $(whoami) != "oracle" && $(whoami) != "grid" ]]; then
#    echo "must be executed by oracle or grid"
#    exit 1
#fi

if [[ -z ${ORACLE_HOME} ]]l then
    echo "Envar doesnt exist!"
    exit 1
fi

if [[ ! -f "${ORACLE_HOME}/bin/adrci" ]]; then
    echo "adrci file doesnt exist"
    exit 1
fi

if [[ ! -x "${ORACLE_HOME}/bin/adrci" ]]; then
    echo "file not executable"
    exit 1
fi

diag_test="/u01/app/$(whoami)"
res=$(mktemp)

"${ORACLE_HOME}/bin/adrci" exec="SET BASE ${diag_test}; SHOW HOMES" >> "${res}"

if [[ $(cat "${res}" | wc -l) -eq 1 ]]; then
    echo "No results were found"
    rm "${res}"
    exit 1
fi

interesting=$(mktemp)
cat "${res}" | tail -n +2 | tr -s ' ' | sed -E "s/^[ ]*//g" | sed -E "s/[ ]*$//g" | grep -E "^[^/]+/(crs|tnslsnr|kfod|asm|rdbms)/" >> "${interesting}"

while read path; do
    mins=$(( ${1} * 60 ))
    "${ORACLE_HOME}/bin/adrci" exec="SET BASE ${diag_test}; SET HOMEPATH ${path}; PURGE -AGE ${minutes}"
done < <(cat "${interesting}")

