find /home/pesho -type f -links +1 -exec stat -c "%Y %i" {} \; | sort -t ' ' -k 1n | head -n 1 | cut -d ' ' -f2
