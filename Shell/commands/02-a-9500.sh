tar -c -f 'c_start.tar' $(find ~/myetc -type f -iname "c*")
rm -r -i ~/myetc
rm -i ~/c_start.tar