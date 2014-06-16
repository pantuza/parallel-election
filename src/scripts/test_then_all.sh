#!/bin/bash


#
# Script to run tests on parallel approach
#


#
# Variables
#
IN_DIR="input";
OUT_DIR="output";
PAR_BIN="../parallel/analyser";
SEQ_BIN="../sequential/analyser";
DICT="../../in/dictionary";
# candidates
C1="../../in/dilma.txt";
C2="../../in/aecio.txt";
C3="../../in/eduardo.txt";
OUT_FILE="time_execution.csv";


#
# Creates the output directory 
#
mkdir -p $OUT_DIR;


#
# Verifies if there is a file
#
[ ! -f "$PAR_BIN" ] && echo "There is no file $PAR_BIN" && exit 1;
[ ! -f "$SEQ_BIN" ] && echo "There is no file $SEQ_BIN" && exit 2;


#
# Verifies if the file is binary
#
[ ! -x "$PAR_BIN" ] && echo "Not a binary file $PAR_BIN" && exit 3;
[ ! -x "$SEQ_BIN" ] && echo "Not a binary file $SEQ_BIN" && exit 4;


#
# Verifies if the input dir is empty
#
[ ! "$(ls -A $IN_DIR)" ] && echo "No input files on $IN_DIR/" && exit 3;


#
# Creates the output file with CSV headers
#
#echo "Tamanho da entrada,Tempo em segundos" > $OUT_DIR/$OUT_FILE;
echo "" > $OUT_DIR/$OUT_FILE;

#
# Runs the parallel program for each input file 
#
ls -tr $IN_DIR | while read infile;
do
    # Number of lines of this input file
    NLINES=$(wc -l $IN_DIR/$infile | cut -d' ' -f1);
    
    # Execution time of this input file
    PAR_TIME=$(./$PAR_BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep time |cut -d' ' -f3);
    SEQ_TIME=$(./$SEQ_BIN $IN_DIR/$infile $DICT $C1 $C2 $C3 |grep time |cut -d' ' -f3);
    
    # writes to outfile
    echo "$NLINES,$PAR_TIME,$SEQ_TIME" >> $OUT_DIR/$OUT_FILE;

    echo -e "$infile -> ($NLINES, $PAR_TIME, $SEQ_TIME)";
done;

echo -e "Result file placed at $OUT_DIR/$OUT_FILE";
