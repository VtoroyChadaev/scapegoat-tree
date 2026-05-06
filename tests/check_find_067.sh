#!/usr/bin/bash
for size in {1000..10000..1000}
do
./program -f -A 0.67 -a 500 -s $size -n 1000 -k 5 >> find_time_067.txt
done
echo All done

