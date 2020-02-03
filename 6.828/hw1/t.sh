ls > y
cat < y | /usr/bin/sort | /usr/bin/uniq | /usr/bin/wc > y1
cat y1
rm y1
ls |  /usr/bin/sort | /usr/bin/uniq | /usr/bin/wc
rm y