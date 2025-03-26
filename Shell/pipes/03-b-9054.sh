cat population.csv | grep -E ',1968,' | cut -d '"' -f3 | sort -t ',' -k 4nr | head -n 42 | tail -n 1 | awk -F ',' '{printf("%s: %d\n", $1, $NF)}'
