#!/bin/bash


#
# Script to run tests on parallel approach
#


#
# Variables
#
IN_DIR="input";
OUT_DIR="output";
BIN="../parallel/analyser";
DICT="../../in/dictionary";
# candidates
C1="../../in/dilma.txt";
C2="../../in/aecio.txt";
C3="../../in/eduardo.txt";
OUT_FILE="parallel_time_execution.csv";
BLOCK=500000;

#
# Creates the output directory 
#
mkdir -p $OUT_DIR;


#
# Verifies if there is a file
#
[ ! -f "$BIN" ] && echo "There is no file $BIN" && exit 1;


#
# Verifies if the file is binary
#
[ ! -x "$BIN" ] && echo "Not a binary file $BIN" && exit 2;


#
# Verifies if the input dir is empty
#
[ ! "$(ls -A $IN_DIR)" ] && echo "No input files on $IN_DIR/" && exit 3;


#
# Creates the output file with CSV headers
#
echo "Entrada,2 Threads,4 Threads,8 Threads, 12 Threads" > $OUT_DIR/$OUT_FILE;


#
# Runs the parallel program for each input file 
#
ls -tr $IN_DIR | while read infile;
do
    # Number of lines of this input file
    NLINES=$(wc -l $IN_DIR/$infile | cut -d' ' -f1);
    
    # Execution time of this input file
    T0=$((echo "2"; echo $BLOCK) | ./$BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep Tempo |cut -d' ' -f4);
    T1=$((echo "4"; echo $BLOCK) | ./$BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep Tempo |cut -d' ' -f4);
    T2=$((echo "8"; echo $BLOCK) | ./$BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep Tempo |cut -d' ' -f4);
    T3=$((echo "12"; echo $BLOCK) | ./$BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep Tempo |cut -d' ' -f4);
    # writes to outfile
    echo "$NLINES,$T0,$T1,$T2,$T3" >> $OUT_DIR/$OUT_FILE;

    echo -e "$infile -> ($NLINES, $T0, $T1, $T2, $T3)";
done;

echo -e "Result file placed at $OUT_DIR/$OUT_FILE";
