#!/bin/bash

if [[ $# -ne 0 ]]; then
    echo "no args expected"
    exit 1
fi

find /home/students -type f -printf "%u %f %T@\n" 2>/dev/null | sort -k 3nr | head -n 1|cut -d ' ' -f1,2

