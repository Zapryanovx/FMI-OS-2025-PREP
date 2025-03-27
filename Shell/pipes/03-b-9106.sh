mkdir $(find songs/ -type f | sort | cut -d '/' -f2 | cut -d '-' -f1 | sed 's/ //g' | uniq)
