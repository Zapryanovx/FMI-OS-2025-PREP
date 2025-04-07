#[Note] unix time and home dirs are updated

cat /etc/passwd | grep "SI:/home" | awk -F ':' '{printf "%s\n", $6}' | xargs -I{} find {} -maxdepth 0 -type d -printf "%C@ %p\n" | awk '$1 <1731993600 && $1 > 1722080000 {print $2}'| xargs -I{} grep :{}: /etc/passwd | cut -d ':' -f 1,5 | cut -c 2- | cut -d ',' -f 1 | tr ':' '\t'
