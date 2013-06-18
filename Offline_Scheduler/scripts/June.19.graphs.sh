# #!/bin/bash

# run this file with the directory large_pop_tests

if [ -z "$1" ] ; then 
    echo "Incorrect usage of $0."
    echo "Enter the name of the directory to graph as the first command line argument"
    exit
fi

mkdir .toGraph
for dir in $(ls "$1") ; do
    echo "# Generation_#  Average  Min" > .toGraph/$dir.dat
    cat $1/$dir/output/cross_0.90.dat | grep "Aver\|Min\|-----------" | \
    awk '{if(NR % 3 == 1) printf "%d\t", $3 ; 
            else if(NR % 3 == 2) printf "%f\t", $3 ;
            else print $3; }' >> .toGraph/$dir.dat
done


for file in $(ls "$1") ; do

    gnuplot << INSTRUCTIONS
    reset
    set terminal postscript eps enhanced 
    set output "eps/$file.eps"
    
    set style data linespoints
    #set size 2, 2
    #set origin 0.0, 0

    set title "Generations vs Fitness"
    set xlabel "Generation"
    set ylabel "Fitness"
    set grid
    
    plot ".toGraph/$file.dat" using 1:2 title 'Average', \
            ".toGraph/$file.dat" using 1:3 title 'Minimum'

            
    reset
    set terminal png enhanced
    set output "pictures/$file.png
    set style data linespoints
    #set size 2, 2
    #set origin 0.0, 0

    set title "Generations vs Fitness"
    set xlabel "Generation"
    set ylabel "Fitness"
    set grid
    
    plot ".toGraph/$file.dat" using 1:2 title 'Average', \
            ".toGraph/$file.dat" using 1:3 title 'Minimum'
            

INSTRUCTIONS

done
