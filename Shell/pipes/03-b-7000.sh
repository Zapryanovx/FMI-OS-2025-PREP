find /usr/include -type f -name "*.[ch]" | wc -l

cat $(find /usr/include -type f -name "*.[ch]") | wc -l
