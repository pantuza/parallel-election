#!/usr/bin/gnuplot
reset

set terminal png

set xlabel 'Tamanho da entrada'
set ylabel 'Tempo em segundos'

# Make the x axis easier to read
set xtics rotate out

# vertical bars as histogram
set style data histogram
set style histogram clustered
set style fill solid border
set key left 
set grid

set output 'output/parallel_time_execution.png'

set datafile separator ','

plot for [COL=2:5] 'output/parallel_time_execution.csv' using COL:xticlabels(1) title columnheader
#plot 'output/parallel_time_execution.csv' using 1:2 title '2 Threads' with boxes, \
#     'output/parallel_time_execution.csv' using 1:3 title '4 Threads' with boxes, \
#     'output/parallel_time_execution.csv' using 1:4 title '6 Threads' with boxes, \
#     'output/parallel_time_execution.csv' using 1:5 title '8 Threads' with boxes
