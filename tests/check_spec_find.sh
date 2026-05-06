#!/usr/bin/bash
for size in {100..1000..100}
do
./program -S -a 500 -s $size -n 100 >> spec_find_time.txt
done
echo All done

