find /tmp -type f -group $(id -gn) '(' -perm -g=w -o -perm -o=w ')'
