cat population.csv | grep -E ',2008,' | awk -F ',' '{sum += $NF} END{print(sum)}'

cat population.csv | grep -E ',2016,' | awk -F ',' '{sum += $NF} END{print(sum)}'

