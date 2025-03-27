find . -perm $(find /etc -type f -printf "%s %m\n" 2>/dev/null | sort -t ' ' -k 1n | tail -n 1 | cut -d ' ' -f2)
