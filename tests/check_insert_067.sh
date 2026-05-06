#!/usr/bin/bash
for size in {1000..10000..1000}
do
./program -i -A 0.67 -a 50 -s $size -n 1000 >> insert_time_067.txt
done
echo All done

