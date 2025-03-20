df -P > 03a3000
cat 03a3000 | head -n -1 | tr -s ' ' | sort -n -t ' ' -k 2