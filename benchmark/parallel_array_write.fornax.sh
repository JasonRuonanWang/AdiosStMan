#!/bin/bash

jobid=""

for i in {1..60}
do
	for j in {1..1}
	do
		jobid_last=$jobid
		jobid=$(qsub -W group_list=partner766 -q workq -l walltime=23:00:00,select=${i}:ncpus=12:mem=68gb:mpiprocs=12 -W depend=afterok:${jobid} parallel_array_write.sh)
		echo $jobid submitted with dependency $jobid_last
	done
done
