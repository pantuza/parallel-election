#!/usr/bin/gnuplot

reset

set terminal png

set xlabel 'Tamanho da entrada'
set ylabel 'Tempo em segundos'
set key left

set grid

set output 'output/time_execution.png'

set datafile separator ','

plot 'output/time_execution.csv' using 1:2 title 'sequencial' with lines, \
     'output/time_execution.csv' using 1:3 title 'paralelo' with lines, \
     #'output/time_execution.csv' using 1:4 title 'hadoop (seg/100)' with lines
