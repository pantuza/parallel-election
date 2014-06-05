#!/bin/bash


#
# Script to run tests on sequential approach
#


#
# Variables
#
IN_DIR="input";
OUT_DIR="output";
BIN="../sequential/analyser";
DICT="../../in/dictionary";
# candidates
C1="../../in/dilma.txt";
C2="../../in/aecio.txt";
C3="../../in/eduardo.txt";
OUT_FILE="sequential_time_execution.csv";


#
# Prints the usage of the script
#
function usage ()
{
    echo -e "USAGE: $0 ";
    exit 1;
}


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
echo "Input,Time" > $OUT_DIR/$OUT_FILE;


#
# Runs the sequential program for each input file 
#
ls -tr $IN_DIR | while read infile;
do
    # Number of lines of this input file
    NLINES=$(wc -l $IN_DIR/$infile | cut -d' ' -f1);
    
    # Execution time of this input file
    TIME=$(./$BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep time |cut -d' ' -f3);
    
    # writes to outfile
    echo "$NLINES,$TIME" >> $OUT_DIR/$OUT_FILE;

    echo -e "$infile -> ($NLINES, $TIME)";
done;

echo -e "Result file placed at $OUT_DIR/$OUT_FILE";
