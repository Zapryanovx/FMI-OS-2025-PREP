tar -c -f c_start.tar $(find myetc/ -iname 'c*') 

rm -rf myetc/
rm -i c_start.tar
