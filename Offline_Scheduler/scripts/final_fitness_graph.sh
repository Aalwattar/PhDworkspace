#!/bin/bash

if [ -z "$1" ] ; then 
    echo "Incorrect usage of $0."
    echo "Enter the name of the directory to graph as the first command line argument"
    exit
fi

echo "Choose a parameter to graph"
select param in "Crossover" "Mutation" "Quit"; do
    if [ "$param" = "Quit" ]; then 
        exit
    elif [ "$param" = "Mutation" ] || [ "$param" = "Crossover" ]; then
        break;
    fi
done

echo "Choose a fitness to graph"
select opt in "Average" "Minimum" "Both" "Quit"; do
    if [ "$opt" = "Quit" ]; then 
        exit
    elif [ "$opt" = "Average" ]; then
       opt="Aver"
       break
    elif [ "$opt" = "Minimum" ]; then 
       opt="Min"
       break
    elif [ "$opt" = "Both" ]; then
        echo "Not implemented yet! :D"
    fi
done

rm -rf .toGraph
mkdir .toGraph

for file in $(ls "$1") ; do
    : > .toGraph/$file.dat

    if [ "$param" = "Crossover" ] ; then
        for cross in 0 0.15 0.3 0.45 0.60 0.75 0.90 1 ; do
            tac $1/$file/output/cross_$cross.dat | grep -m 1 "$opt" | \
            awk -v cross="$cross" '{printf "%f\t%f\n", cross, $3 ;}' >> .toGraph/$file.dat
        done
    
    elif [ "$param" = "Mutation" ]; then
        for mut in 0.1 0.075 0.05 0.025 0.01 0.0075 0.005 0.0025 0.001 0 ; do
            tac $1/$file/output/mut_$mut.dat | grep -m 1 "$opt" | \
            awk -v cross="$mut" '{printf "%f\t%f\n", cross, $3 ;}' >> .toGraph/$file.dat
        done
    fi
done


files=$(ls "$1")

gnuplot << INSTRUCTIONS
    filenames = "B2_25.dat B1_50.dat B2_100.dat B1_150.dat"

    reset
    set terminal postscript eps enhanced 
    set output "eps/$1.eps"
    
    set style data linespoints
    #set size 2, 2
    #set origin 0.0, 0

    set title "$param Rate vs Fitness"
    set xlabel "$param Rate"
    set ylabel "Fitness"
    set grid
    
    plot for [data in filenames] ".toGraph/".data using 1:2 title data

            
    reset
    set terminal png enhanced
    set output "pictures/$1.png
    set style data linespoints
    #set size 2, 2
    #set origin 0.0, 0

    set title "$param Rate vs Fitness"
    set xlabel "$param Rate"
    set ylabel "Fitness"
    set grid
    
    plot for [data in filenames] ".toGraph/".data using 1:2 title data
            
INSTRUCTIONS
