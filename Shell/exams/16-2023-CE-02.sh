find my_logs -type f | grep -E '[a-zA-Z0-9_]+_[1-9][0-9]*.log' | xargs -I{} cat {} | grep -o 'error' | wc -l
