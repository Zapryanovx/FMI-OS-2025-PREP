cat /etc/passwd | head -n 12
cat /etc/passwd | head -n 1 | cut -c 1-26
cat /etc/passwd | head -n -4
cat /etc/passwd | tail -n 17
cat /etc/passwd | head -n 151 | tail -n 1
cat /etc/passwd | head -n 13 | tail -n 1 | rev | cut -c 1-4 | rev
