find /bin/. -type f -exec file {} \; | grep "shell script" | wc -l

find /bin/. -type f -exec file {} \; | grep "ASCII text" | wc -l