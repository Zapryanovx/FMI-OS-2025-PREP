cat /etc/passwd | grep $(id -g)
cat /etc/passwd | grep $(id -u) -B 2
cat /etc/passwd | grep $(id -u) -B 2 -A 3
cat /etc/passwd | grep $(id -u) -B 2 | head -n 1
