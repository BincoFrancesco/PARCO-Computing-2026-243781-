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
  1- loads compiler and MPI modules
  2- compiles the code
  3- executes strong scaling and weak scaling benchmarks
  4- Collects timing results

## 3. Inputs and Outputs





