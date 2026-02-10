# SpMV with MPI (Deliverable 2)
## 0. Disclaimer
This project is made by Binco Francesco 243781 and implements SpMV ( Sparse Matrix Vector multiplication ) by using matrices with the CSR ( Compressed Sparse Row ) format and MPI for parallelization.
This project follows Foster's methodology and includes strong and weak scaling experiments executed on an HPC cluster.

## 1. Compiler, MPI Library and Flags
- Compiler version: GCC 9.1.0
- MPI implementation: MPICH 3.2.1
- Language standard: C++11
- Compilation flags: `O2 -std=c++11`
  
## 2. How to Compile and Run
Al the experiemtns are compiled and run on the cluster using a PBS job script:
1) loads compiler and MPI modules
2) compiles the code
3) executes strong scaling and weak scaling benchmarks
4) Collects timing results
  2- 
  3- 
  4- 

## 3. Input Matrices
The inputs of the program are Sparce matrices in Matrix Market format (.mtx)
Statistics of the matrices used in the project:
( Matrix_name n_Rows n_Columns n_NonZeros)
- strong_matrix.mtx  100k    100k    25M
- weak_matrix_1.mtx  10k     10k     200k
- weak_matrix_2.mtx  14k     14k     400k
- weak_matrix_3.mtx  20k     20k     800k
- weak_matrix_4.mtx  28k     28k     1.6M
- weak_matrix_5.mtx  40k     40k     3.2M
- weak_matrix_6.mtx  57k     57k     6.4M
- weak_matrix_7.mtx  80k     80k     12.8M
- weak_matrix_8.mtx  116k    116k    20M
- weak_matrix_9.mtx  130k    130k    25M

The actual files of the matrices can be found at the following link: https://drive.google.com/drive/folders/1y63WH0aZh6RebCCMP0FkVksn1fACPiCK?usp=sharing
( The matrices are stored in the google drive folder of my university account because they were too big to upload directly to github )

## 4. Implemented Functionalities
- Distributed SpMV with MPI
- Rank 0 reads the entire Matrix Market file and distributes matrix entries to all processes
- Data Distribution: 1D modulo ( cyclic ) partitioning
- Performance evaluation: Strong scaling and weak scaling
- Metrics: Execution time for SpMV, speedup and efficiency, FLOPs

