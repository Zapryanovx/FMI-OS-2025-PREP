cat population.csv | grep -E ',2016,' | awk -F ',' 'p < $NF {p = $NF; c = $1} END{printf "%s: %d\n", c, p}'   

cat population.csv | grep -E ',2016,' | cut -d '"' -f3 | sort -t ',' -k4n | head -n 1

cat population.csv | grep -E ',2016,' | cut -d '"' -f3 | sort -t ',' -k4n | tail -n 1
