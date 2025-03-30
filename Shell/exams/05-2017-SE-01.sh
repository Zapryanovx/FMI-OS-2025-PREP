find . -type f | xargs -I{} stat -c "%h %n" {} | sort -t ' ' -k 1nr | head -n 5 | cut -d ' ' -f2
