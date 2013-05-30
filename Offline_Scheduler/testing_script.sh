#!/bin/bash

touch param_info.txt

for pop in {6, 10, 16, 20, 30, 40, 50, 60, 80, 100, 120, 150};
    do

    for mut in {0.001, 0.005, 0.01, 0.03, 0.05, 0.07, 0.010};
        do

        touch .average.txt

            for i in {1..30};
                do
                    ./OfflineScheduler.exe -ps=$pop -mr=$mut > .run_results.txt
                    grep "fitness = " .run_results.txt -m 1 >> .average.txt
                done

        head -12 .run_results.txt >> param_info.txt

        echo "" >> param_info.txt
        awk '{s += $4} END {print s}' .average.txt >> param_info.txt
        echo "" >> param_info.txt
        echo "====================================================" >>  param_info.txt

        rm .run_results.txt .average.txt

        done

    done
