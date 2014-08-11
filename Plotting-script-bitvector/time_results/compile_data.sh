#!/bin/bash

readonly DATA_DIR=$1
readonly MAX_EDITS=$2

for i in $(seq 0 $MAX_EDITS); do
    dataFs1=$(ls | grep _${i}_ | grep result$)
    dataFs1Array=($dataFs1)
    dataFs1Len=${#dataFs1Array[*]} 

    dataFs2=$(ls | grep _${i}_ | grep swps$)
    dataFs2Array=($dataFs2)
    dataFs2Len=${#dataFs2Array[*]} 

    dataFs3=$(ls | grep fastq_AF$)
    dataFs3Array=($dataFs3)
    dataFs3Len=${#dataFs3Array[*]}

    if [ $dataFs1Len -ne $dataFs3Len ]
	then
	echo "ERROR: data files not found!"
	echo $dataFs1Len "*_"$i"_*results files were found"
	echo $dataFs2Len "*_"$i"_*swps files were found"
	echo $dataFs3Len "*fastq_AF files were found"
	exit
    fi

    # run the python script to parse data
    ./parse_data.py $i ${i}_errors.dat $dataFs1Len $dataFs1 $dataFs2 $dataFs3
    mv ${i}_errors.dat $DATA_DIR

done

echo "Finished! The gnuplot ready data files are in" $DATA_DIR
