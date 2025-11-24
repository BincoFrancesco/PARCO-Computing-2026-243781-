Compiler version: gcc 9.1
In order to seee the results obtain in Deliverable 1 there will be the need to compile and run every iteration of parallelization, that can be found in the code commented line by line.

As input the program takes 5 matrixes: 494_bus, ex37, G61, G7, marine1; and as outputs it will give the duration for each matrix of the operatione Matrix-Vector multiplication

For the testing in the cluster it has been used the following interactive session: qsub -I -q common_cpuQ -l select=1:ncpus=64:mem=16GB

And to find the performances of each parallezation strategy it has been loaded and used the module perf.




