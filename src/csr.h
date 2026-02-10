#ifndef __CSR_H__
#define __CSR_H__
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <set>
#include <string>
#include <time.h>
#include <chrono>
#include <algorithm>
#include <stdexcept>

using namespace std;

//Functions
void extract(string file_name, vector<double>& lines, vector<double>& columns, vector<double>& values, int& index, int& n_Rows, int& n_Columns, int& n_NonZeros);
int partition(vector<double>& l, vector<double>& c, vector<double>& values, int low, int high);
void quickSort(vector<double>& l, vector<double>& c, vector<double>& values, int low, int high);
void compress(vector<double>& v);
void product(vector<double> const lines, vector<double> const columns, vector<double> const values, bool const csr, vector<double> const v, vector<double>& res);
void store(double const value, string const file_name);
double near_last(vector<double> v);
void synthetize(string file_name);

class matrix{
private:
	string matrix_name;
	int index;
	bool csr;
public:
	//Public parameters
	int n_Rows, n_Columns, n_NonZeros;
	vector<double> lines;
	vector<double> columns;
	vector<double> values;
	
	
	//Constructor and Distructors
	matrix();
	matrix(string file_name);
	~matrix();
	//Methods to set/add elements
	void set_Columns(int dim);
	void set_Rows(int dim);
	void set_NonZeros(int dim);
	//Methods to get stuff
	int get_index()const;
	int get_Columns()const;
	int get_Rows()const;
	int get_NonZeros()const;
	bool get_CSR()const;
	//Printing method
	void print_matrix()const;
	void print_CSR_lines()const;
	double vector_product(vector<double>&res, vector<double> v);
	
};

//functions for MPI
matrix build_local_csr(const matrix& m_global, int rank, int P, vector<int>& global_rows);

#endif

