for dir in 1-pt_tests 2-pt_tests keep_best_tests 2x_tests runtime_only_tests ; do
	cd $dir; bash compile_results.sh; cd ..

	for type in Aver Min ; do

		gnuplot << INSTRUCTIONS
		reset
		set terminal postscript eps enhanced 
		set output "eps/$type.$dir.eps"

		set style data linespoints
		#set size 2, 2
		#set origin 0.0, 0

		set title "$type $dir vs Fitness"
		set xlabel "Crossover Rate"
		set ylabel "Fitness"
		set grid
		f(x) = 842
		g(x) = 2451
		h(x) = 656
		k(x) = 1571
		plot "$dir/results_$type.dat" using 1:2 title 'B2\_25\_40', \
			"$dir/results_$type.dat" using 1:3 title 'B2\_50\_50', \
			"$dir/results_$type.dat" using 1:4 title 'B2\_100\_120', \
			"$dir/results_$type.dat" using 1:5 title 'B2\_150\_200'
                       

		reset
		set terminal png enhanced
		set output "pictures/$type.$dir.png

		set style data linespoints
		#set size 2, 2
		#set origin 0.0, 0

		set title "$type $dir vs Fitness"
		set xlabel "Crossover Rate"
		set ylabel "Fitness"
		set grid
		f(x) = 842
		g(x) = 2451
		h(x) = 656
		k(x) = 1571
		plot "$dir/results_$type.dat" using 1:2 title 'B2\_25\_40', \
			"$dir/results_$type.dat" using 1:3 title 'B2\_50\_50', \
			"$dir/results_$type.dat" using 1:4 title 'B2\_100\_120', \
			"$dir/results_$type.dat" using 1:5 title 'B2\_150\_200'
                       

INSTRUCTIONS

	done
done


for dir in mutation_tests ; do
	cd $dir; bash compile_results.sh; cd ..

	for type in Aver Min ; do

		gnuplot << INSTRUCTIONS
		reset
		set terminal postscript eps enhanced 
		set output "eps/$type.$dir.eps"

		set style data linespoints
		#set size 2, 2
		#set origin 0.0, 0

		set title "$type $dir vs Fitness"
		set xlabel "Mutation Rate"
		set ylabel "Fitness"
		set grid
		plot "$dir/results_$type.dat" using 1:2 title 'B2\_25\_40', \
			"$dir/results_$type.dat" using 1:3 title 'B2\_50\_50', \
			"$dir/results_$type.dat" using 1:4 title 'B2\_100\_120', \
			"$dir/results_$type.dat" using 1:5 title 'B2\_150\_200'

		reset
		set terminal png enhanced
		set output "pictures/$type.$dir.png

		set style data linespoints
		#set size 2, 2
		#set origin 0.0, 0

		set title "$type $dir vs Fitness"
		set xlabel "Mutation Rate"
		set ylabel "Fitness"
		set grid
		plot "$dir/results_$type.dat" using 1:2 title 'B2\_25\_40', \
			"$dir/results_$type.dat" using 1:3 title 'B2\_50\_50', \
			"$dir/results_$type.dat" using 1:4 title 'B2\_100\_120', \
			"$dir/results_$type.dat" using 1:5 title 'B2\_150\_200'

INSTRUCTIONS

	done
done
