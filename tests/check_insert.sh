#!/usr/bin/bash
for size in {1000..10000..100}
do
./program -i -a 500 -s $size -n 1000 >> insert_time.txt
done
echo All done

