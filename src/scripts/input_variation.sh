#!/bin/bash

#
# Script for create a variation on input files. 
# Used for generate Execution time Graphs
#


#
# Variables
#
IN_DIR="input";
FNAME='input_';
NLINES=0;
TIMES=30000;
i=$TIMES;
j=0;


#
# Prints the usage of the script
#
function usage ()
{
    echo "USAGE: $0 full_input_file.txt";
    exit 1;
}


#
# Create the input folder if not exists
#
mkdir -p $IN_DIR;


#
# Prints usage message if arg is missing
#
[ ! -n "$1" ] && echo -e "Missing argument" && usage;


#
# Assert if the file is valid
#
[ ! -f "$1" ] && echo -e "Not a valid file" && usage;


#
# Get the line counter of the file
#
NLINES=$(wc -l "$1" | cut -d' '  -f1);


#
# If $NLINES is less than $TIMES ask for change the variable
#
[ $NLINES -le $TIMES ] && echo -e "Change the $TIMES variable" && usage;


#
# Creating input files with size $i + $TIMES lines until $NLINES
#
echo -e "Creating files:\n"
while [ $i -lt $NLINES ]
do
    # Create an input file
    head -$i "$1" > $IN_DIR/$FNAME$j;
    echo -e "$IN_DIR/$FNAME$j";

    # increment $i e $j
    i=$(($i + $TIMES));
    j=$(($j + 1));
done;
