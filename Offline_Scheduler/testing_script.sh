#!/bin/bash

for i in {1..15};
do
	./OfflineScheduler.exe > results.txt
        grep "fitness = " results.txt -m 1 >> average.txt
done

rm results.txt
