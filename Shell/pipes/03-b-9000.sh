touch eternity
find . -type f ! -name '.*' -mmin -15 -printf '%p %T@\n'
