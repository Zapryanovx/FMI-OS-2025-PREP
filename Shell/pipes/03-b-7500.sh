cat /etc/services | grep -E -o '[a-zA-Z]+' | tr '[:upper:]' '[:lower:]' | sort | uniq -c | sort -nr | head
