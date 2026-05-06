#!/usr/bin/bash
for size in {100..1000..100}
do
./program -S -A 0.67 -a 500 -s $size -n 100 >> spec_find_time_067.txt
done
echo All done

