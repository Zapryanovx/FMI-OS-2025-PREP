cat emp.data | awk '{for(i = NF; i > 0; i--)printf "%s\t", $i} {printf "\n"}'
