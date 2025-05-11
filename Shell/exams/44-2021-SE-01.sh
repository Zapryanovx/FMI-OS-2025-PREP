#!/bin/bash

if [[ $# -ne 0 ]]; then
    echo "no args expected"
    exit 1
fi

#if [[ $(whoami) != "oracle" ]] && [[ $(whoami) != "grid" ]]; then
#    echo "script must be executed by oracle or grid"
#    exit 1
#fi

if [[ -z ${ORACLE_HOME} ]]; then
    echo "no oracle home found"
    exit 1
fi

if [[ ! -x "${ORACLE_HOME}/bin/adrci" ]]; then
    echo "no adrci executable file found"
    exit 1
fi

dirs=$(mktemp)
./"${ORACLE_HOME}/bin/adrci" exec="show homes" >> "${dirs}"

if [[ $(cat "${dirs}" | wc -l) -eq 1 ]]; then
    cat "${dirs}"
else
    diag_dest="/u01/app/$(whoami)" 

    while read file; do
        bytes=$(stat -c "%s" "${diag_dest}/${file}")
        mbs=$( echo "scale=2;bytes / 1024 / 1024" | bc )
        echo "${mbs} ${diag_dest}/${file}"
    done < <(cat ${dirs} | tail -n +2 | tr -s ' ' | sed 's/^[ ]+//g' | sed 's/[ ]+$//g')
fi


