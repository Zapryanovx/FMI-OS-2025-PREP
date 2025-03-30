cp /srv/sample-data/01-shell/2016-SE-01/philip-j-fry.txt ~

grep -E "^[^a-w]*[02468][^a-w]*$" philip-j-fry.txt | wc -l
