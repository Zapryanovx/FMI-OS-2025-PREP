cat population.csv | grep -E "Bulgaria" | awk -F ',' 'p < $NF {p = $NF; y = $(NF - 1);} END{print(y)}'
