find . -type f -size 0 -exec rm {} \;
 
find ~ -type f -exec wc -c {} \; | sort -t ' ' -k 1n | tail -n 5 | cut -f2 | xargs rm
