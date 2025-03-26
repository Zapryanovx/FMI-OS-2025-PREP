find ~ -type f -printf '%T@ %f\n' | sort -n -r | head | cut -d ' ' -f 2

find ~ -type f -printf '%A@ %f\n' | sort -n -r | head | cut -d ' ' -f 2
