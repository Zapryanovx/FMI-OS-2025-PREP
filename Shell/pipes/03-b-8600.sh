find /usr -type f -name "*.sh" -exec head -n 1 {} \; | grep "^#!"| cut -d '!' -f 2 | sort | uniq -c | sort -nr | head -n 1 | awk '{print($2)}'
