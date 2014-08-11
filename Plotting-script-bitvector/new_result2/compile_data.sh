#!/bin/bash

readonly DATA_DIR=$1
readonly MAX_EDITS=$2

for i in $(seq 0 $MAX_EDITS); do
    dataFs1=$(ls | grep _${i}_ | grep no_AF)
    dataFs1Array=($dataFs1)
    dataFs1Len=${#dataFs1Array[*]} 

    dataFs2=$(ls | grep _${i}_ | grep -E [0-9]_result$)
    dataFs2Array=($dataFs2)
    dataFs2Len=${#dataFs2Array[*]} 

    if [ $dataFs1Len -ne $dataFs2Len ]
	then
	echo "ERROR: data files not paired"
	echo $dataFs1Len "*_"$i"_*results file were found"
	echo $dataFs2Len "*_"$i"_*swps file were found"
	exit
    fi

    # run the python script to parse data
    ./parse_data.py ${i}_errors.dat $dataFs1Len $dataFs1 $dataFs2
    mv ${i}_errors.dat $DATA_DIR

done

echo "Finished! The gnuplot ready data files are in" $DATA_DIR
