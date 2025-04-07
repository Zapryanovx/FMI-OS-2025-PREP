find ~ -printf "%i %f %d\n" | grep $(find ~ -printf "%T@ %i\n" | sort -n | head -n 1 | cut -d ' ' -f2) | sort -k 3n | head -n 1 | cut -d ' ' -f2
