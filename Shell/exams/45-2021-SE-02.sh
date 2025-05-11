#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "Script expects args!"
    exit 1
fi

for arg in ${@}; do
   if [[ ! -f "${arg}" ]]; then
        echo "Not a zonal file!"
        continue
    fi

    if ! $(cat "${arg}" | sed -E "s/;.*$//g" | head -n 1 | grep -Eq "SOA"); then
        echo "${arg} is not a correct zonal file! The 1st record isn't SOA!"
    fi

    mod_date=""

    if cat "${arg}" | sed -E "s/;.*$//g" | grep -Eq "\("; then
        mod_date=$(cat "${arg}" | head -n 2 | tail -n 1 | tr '\t' ' ' | tr -s ' ' | sed -E "s/^[ ]+//" | sed -E "s/;.*$//" | sed -E "s/[ ]*$//")
    else
        mod_date=$(cat "${arg}" | head -n 1 | tr '\t' ' ' | tr -s ' ' | sed -E "s/;.*$//" | sed -E "s/^.*(NS|SOA|A|AAAA)//" |\
        sed -E "s/^[ ]+//" | tr -s ' ' '\n' | sed -E 's/^[ ]+//' |grep -E "^[0-9]+$" | head -n 1)
    fi

    curr_date=$(date +"%Y%m%d")
    old_date=$(echo "${mod_date}" | head -c -3)

    if [[ "${curr_date}" > "${old_date}" ]]; then
        sed -iE "s/\b${mod_date}\b/${curr_date}00/" "${arg}"
    elif [[ "${curr_date}" == "${old_date}"  ]]; then
        tt=$(echo "${mod_date}" | tail -c 3)
        tt=$(( ${tt} + 1 ))

        if [[ ${tt} -eq 100 ]]; then
            echo "Cannot make more than 100 changes per day!"
        fi

        if [[ ${tt} -lt 10 ]]; then
            tt="0${tt}"
        fi

        sed -iE "s/\b${mod_date}\b/${curr_date}${tt}/" "${arg}"
    fi
done
