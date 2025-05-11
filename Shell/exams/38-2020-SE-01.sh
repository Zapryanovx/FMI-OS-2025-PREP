c#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "2 args expected";
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "arg 1 must be a file"
    exit 1
fi

if [[ ! -d $2 ]]; then
    echo "arg 2 must be a dir"
    exit 1
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "${1}"

while read file; do
    csv=$(basename "${file}" .log)
    
    data=$(cat "${file}" \
    | grep -E '.+' \
    | sed -E "s/has a?n/has a:/g" \
    | sed -E "s/license/:license/g" \
    | cut -d ':' -f2 \
    | tr '\n' ',' \
    | rev \
    | cut -c 2- \
    | rev \
    | sed "s/, /,/g" \
    | sed "s/ ,/,/g") 
    
    echo "${csv}${data}" >> "${1}"
done < <(find "${2}" -mindepth 1 -type f -name '*.log')
