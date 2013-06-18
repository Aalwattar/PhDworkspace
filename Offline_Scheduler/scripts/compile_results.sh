#!/bin/bash

for graph in Aver Min ; do

	for file in B1_50 B1_150 B2_25 B2_100 ; do
		touch $file.dat

		for cross in 0 0.15 0.3 0.45 0.60 0.75 0.90 1 ; do
			tac $file/output/cross_$cross.dat | grep -m 1 "$graph" | awk '{print $3}' >> $file.dat
		done
	done


	echo "0.0 0.15 0.3 0.45 0.60 0.75 0.90 1" > .results.dat

	for file in B2_25 B1_50 B2_100 B1_150 ; do
		awk '{printf "%f ", $0}' $file.dat >> .results.dat
		echo " " >> .results.dat
		rm $file.dat
	done

	cat .results.dat | rs -T > results_$graph.dat
	rm .results.dat
done
