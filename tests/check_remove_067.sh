#!/usr/bin/bash
for size in {10000..100000..10000}
do
./program -r -A 0.67 -a 500 -s $size -n 1000 -k 5 >> remove_time_067.txt
done
echo All done

