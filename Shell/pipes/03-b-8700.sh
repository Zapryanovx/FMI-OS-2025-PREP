cat /etc/passwd | cut -d ':' -f4 | sort -n | uniq -c | sort -nr | head -n 5 | awk '{print $NF}' | xargs -I {} grep -E ':{}:' /etc/group | cut -d ':' -f 1
