#include <iostream>
#include <mpi.h>
#include "csr.h"

#define TestRuns 10

using namespace std;

int main(int argc, char** argv) {
	//Usefull variables and setups
	srand(time(0));
	
	double t;
	
	//MPI initialization
	MPI_Init(&argc, &argv);
	
	int rank, P;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	
	if(argc < 2){
		if(rank == 0) {
			cerr << "Usage: mpirun -n P spmv matrix.mtx";
		}
		MPI_Finalize();
		return 1;
	}
	
	//Set the name of the matrix in the .pbs file
	string matrix_name = argv[1];
	
		try{
		//Load Global matrix with process 0
		matrix M;
		if(rank == 0){
			M = matrix(matrix_name);
		}
		
		//Broadcast dimensions
		int dimensions[3];
		if(rank == 0){
			dimensions[0] = M.n_Rows;
			dimensions[1] = M.n_Columns;
			dimensions[2] = M.n_NonZeros;
		}
		MPI_Bcast(dimensions, 3, MPI_INT, 0, MPI_COMM_WORLD);
		
		if(rank != 0){
			M.n_Rows = dimensions[0];
			M.n_Columns = dimensions[1];
			M.n_NonZeros = dimensions[2];
			M.lines.resize(M.n_Rows +1);
			M.columns.resize(M.n_NonZeros);
			M.values.resize(M.n_NonZeros);
		}
		
		MPI_Bcast(M.lines.data(), M.n_Rows +1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(M.columns.data(), M.n_NonZeros, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(M.values.data(), M.n_NonZeros, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
		//Usefull vectors
		vector<double> v(M.n_Columns);
		if(rank == 0){
			for(int i = 0; i < M.n_Columns; ++i){
				v[i] = 1.0;
			}
		}
		vector<double> res(M.n_Columns, 0.0);
		MPI_Bcast(v.data(), M.n_Columns, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
		
		//Partition
		vector<int> global_rows;
		matrix m_local = build_local_csr(M, rank, P, global_rows);
		
		//MPI TIMER
		MPI_Barrier(MPI_COMM_WORLD);
		double t0 = MPI_Wtime();
		double normal_timer = m_local.vector_product(res, v);
		MPI_Barrier(MPI_COMM_WORLD);
		double t1 = MPI_Wtime();
		
		double mpi_time = t1 -t0;
		double max_time;
		
		MPI_Reduce(&mpi_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		
		//Output from rank 0
		if(rank == 0){
			ofstream str("results.csv", ios::app);
			if(str.tellp() == 0){
				str << "matrix, P, n_Rows, n_Columns, n_NonZeros, time_sec," << endl;
			}
			str << matrix_name << ", " << P << ", " << M.n_Rows << ", " << M.n_Columns << ", " << M.n_NonZeros << ", " << max_time << endl;
			str.close();
		}
		cout << "Max Time: " << max_time << " s" << endl;
		
		
		//MPI Ending
		MPI_Finalize();
	}catch(const runtime_error& e){
		cerr << "Rank out of bound!" << endl;
		MPI_Abort(MPI_COMM_WORLD,1);
	}
	return 0;
}



