find / -maxdepth 3 -type d >/dev/null 2>sol.txt
cat sol.txt | cut -d ':' -f 2 | tr -d ' ' | cut -c 4- | rev | cut -c 4- | rev
rm sol.txt