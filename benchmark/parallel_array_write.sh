#!/bin/bash

QUOTA="2000000000000"
prefix=$scratch/stripe8
JOBSCHEDULER="slurm"

for rows in $(seq 1000 200 6000) 
do
	for length in $(seq 2000 200 2000)
	do

		for i in $(seq 1 10)
		do

			echo $rows $length $length $prefix/${rows}rows_${length}length_${SLURM_JOBID}_${i}.casa

			CHECK=$(du -sb $prefix | cut -f1)

			if [ "$CHECK" -gt "$QUOTA" ]; then
				echo "$CHECK bytes in $prefix, reaching disk quota $QUOTA, cleaning up ..."
				rm -rf $prefix/*
			fi

			if [ "$JOBSCHEDULER" == "slurm" ]; then
				aprun -B $SLURM_SUBMIT_DIR/parallel_array_write $rows $length $length $prefix/${rows}rows_${length}length_${SLURM_JOBID}_${i}.casa  >> $SLURM_SUBMIT_DIR/log
			fi

			if [ "$JOBSCHEDULER" == "pbs" ]; then
				mpirun $PBS_O_WORKDIR/parallel_array_write $rows $length $length $prefix/${rows}rows_${length}length_${PBS_JOBID}_${i}.casa  >> $PBS_O_WORKDIR/log
			fi

		done

	done
done


