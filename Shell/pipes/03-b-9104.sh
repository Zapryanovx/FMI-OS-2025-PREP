find songs/ -type f | cut -d '(' -f2 | cut -d ')' -f1 | sort -t ',' -k 2n | cut -d ',' -f1 | uniq
