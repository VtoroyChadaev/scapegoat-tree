#!/usr/bin/bash
for size in {10000..100000..10000}
do
./program -r -a 500 -s $size -n 1000 -k 5 >> remove_time.txt
done
echo All done

