cat ~/mypasswd.txt | grep '.*/home/SI/.*' | cut -d ':' -f 1 | cut -c 2- | sort -n > si.txt
