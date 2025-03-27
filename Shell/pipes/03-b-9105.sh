find songs/ -type f | cut -d '/' -f2 | grep -E "^(Pink|Beatles) -" | wc -l
