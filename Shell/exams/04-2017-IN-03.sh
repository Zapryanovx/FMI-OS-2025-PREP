cat example-passwd | grep -E "^.*[A-Za-z]+a[,:].*\/home\/Inf"| cut -c 3-4 | sort -n | uniq -c | sort -nr | head -n 1
