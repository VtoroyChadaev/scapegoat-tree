#!/usr/bin/bash
for size in {1000..10000..1000}
do
./program -f -a 500 -s $size -n 1000 -k 5 >> find_time.txt
done
echo All done

