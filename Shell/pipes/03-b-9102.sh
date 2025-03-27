find songs/ -type f | cut -d '-' -f2 | cut -d '.' -f1 | cut -d '(' -f1 | sed 's/ //'
