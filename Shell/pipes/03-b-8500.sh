cat /etc/group | cut -d ':' -f 3 | awk -v gr="$(id -g)" '$1 == gr {print "Hello, " $1 " - I am here"} $1 != gr {print "Hello, " $1}'
