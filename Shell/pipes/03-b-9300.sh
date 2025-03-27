cat emails.txt | grep -E "^[_a-zA-Z0-9]+((\.){0,1}[_a-zA-Z0-9-])*@[_a-zA-Z0-9]+((\.){0,1}[_a-zA-Z0-9-])*$"

cat emails.txt | grep -E '^[_a-zA-Z0-9]+(\.[_a-zA-Z0-9-]+)*@[_a-zA-Z0-9]+(\.[_a-zA-Z0-9-]+)*$'



