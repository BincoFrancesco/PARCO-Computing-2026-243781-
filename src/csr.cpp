#include "csr.h"

#define Jump 4

using namespace std;


//-----------------------------External functions------------------------------------
void extract(string file_name, vector<double>& lines, vector<double>& columns, vector<double>& values, int& index, int& n_Rows, int& n_Columns, int& n_NonZeros){
	string trash_lines;
	int n_value;
	double l,c,v;
	//Woriking on the file
	ifstream str(file_name);
	if(!str.is_open()){cout << "Error in operaning the file"; return;}
	//Get the values
	for(int i = 0; i < Jump; ++i){
		if(i<1){
			getline(str,trash_lines);
		}else{
			str>>n_value;
			//Get the number of Rows, Columns and NonZeros
			if(i == 1){n_Rows = n_value;}
			if(i == 2){n_Columns = n_value;}
			if(i == 3){n_NonZeros = n_value;}
		}
	}
	//The last n_value indicates the number of non zero points in the matrix
	index = n_value;
	
	for(int i = 0; i < index; ++i){
		str >> l; str >> c; str >> v;
		lines.push_back(l);
		columns.push_back(c);
		values.push_back(v);
	}
	
	str.close();
}//Working
//----------------------------------------------------------------------------------
int partition(vector<double>& l, vector<double>& c, vector<double>& values, int low, int high){
	int pivot = l[low];
    int i = low - 1;
    int j = high + 1;

    while(true){
        //Let's find the leftmost element >= pivot
        do{
            ++i;
        }while(l[i]< pivot);
        //Find the rightmost element <= pivot
        do{
            --j;
        }while(l[j]>pivot);

        //If the 2 pointers have crossed return j
        if(i >= j){return j;}

        swap(l[i], l[j]);
        swap(c[i], c[j]);
        swap(values[i], values[j]);
    }
}//Working
//----------------------------------------------------------------------------------
void quickSort(vector<double>& l, vector<double>& c, vector<double>& values, int low, int high){
	if(low < high){
        //Returns the index of the pivot
        int pi = partition(l, c, values, low, high);
        //Recursion calls for smaller and bigger elements' subarrays
        quickSort(l, c, values, low, pi);
        quickSort(l, c, values, pi+1, high);
    }
}//Working
//----------------------------------------------------------------------------------
void compress(vector<double>& v){
	vector<double> compressV(v.back()+1,0);
	for(auto& item : v){
		++compressV[item];
	}
	v.clear();
	v = compressV;
}//Working
//----------------------------------------------------------------------------------
void product(vector<double> const lines, vector<double> const columns, vector<double> const values, bool const csr, vector<double> const v, vector<double>& res){
	//Make sure that we have a CSR matrix to work with
    if(csr == false){cout << "!!You need to convert the Matrix in the CSR format!!";return;}
    //v is generated with the same number of colums of matrix, so we do not need to worry about it
    //res is the vector we need to apply the moltiplication to
    //Useful parameters
    int sum = 0; //The sum
    int pointer = 0; //The index the marks where to start the new moltiplication on the value's vector
	for(int i = 0; i < lines.size(); ++i){ //We pass each element of lines
	    for(int j = 0; j < lines[i]; ++j){//For a number of time = to the specified element of lines
            sum += (values[ pointer + j ]*v[columns[pointer + j]-1]);
        }
        res.push_back(sum);
        pointer += lines[i];
        sum = 0;
    }
}//Working
//----------------------------------------------------------------------------------
void store(double const value, string const file_name){
	ofstream str(file_name, ios::app);
	if(!str){cerr << "Error in storing results"; return;}
	str << value << endl;
	str.close();
}//Working
//----------------------------------------------------------------------------------
double near_last(vector<double> v){
	sort(v.begin(), v.end());
	return v[v.size() - 2];
}//Working
//----------------------------------------------------------------------------------
void synthetize(string file_name){
	vector<double> results(10,0);
	vector<double> final_results;
	double temp;
	int i = 0, sum = 0;
	ifstream str(file_name);
	if(!str.is_open()){cout << "Error in opening the file"; return;}
	while(str>>temp){
		++i;
		if(i==10){
			final_results.push_back(near_last(results));
			i = 0;
			fill(results.begin(), results.end(),0);
		}else{
			results.at(i) = temp;
		}
	}
	str.close();
	for(auto& item : final_results){
		store(item,"Final_Results.txt");
	}
}//Working


//-------------------------------Constructors---------------------------------------
matrix::matrix(){
	matrix_name = "Empty";
	index = 0; n_Rows = 0; n_Columns = 0; n_NonZeros = 0;
	csr = false;
	cout << "Created " << matrix_name << " matrix" << endl;
}
//----------------------------------------------------------------------------------
matrix::matrix(string file_name){
	matrix_name = file_name; cout << "Done ";
	extract(file_name, lines, columns, values,index,n_Rows, n_Columns, n_NonZeros); cout << "Done ";
	quickSort(lines, columns, values, 0, lines.size()-1); cout << "Done ";
	compress(lines); cout << "Done "<< endl;
	csr = true;
	cout << "Created " << matrix_name << " CSR matrix with " << index << " elements " << endl;
}
//---------------------------------Distructor---------------------------------------
matrix::~matrix(){cout << "Matrix " << matrix_name << " destroyed" << endl;}


//----------------------------------Methods-----------------------------------------
int matrix::get_index()const{return index;}
//----------------------------------------------------------------------------------
int matrix::get_Columns()const{return n_Columns;}
//----------------------------------------------------------------------------------
int matrix::get_Rows()const{return n_Rows;}
//----------------------------------------------------------------------------------
int matrix::get_NonZeros()const{return n_NonZeros;}
//----------------------------------------------------------------------------------
void matrix::set_Columns(int dim){ n_Columns = dim; }
//----------------------------------------------------------------------------------
void matrix::set_Rows(int dim){ n_Rows = dim; }
//----------------------------------------------------------------------------------
void matrix::set_NonZeros(int dim){ n_NonZeros = dim; }
//----------------------------------------------------------------------------------
bool matrix::get_CSR()const{return csr;}
//----------------------------------------------------------------------------------
void matrix::print_matrix()const{
	for(int i = 0; i < index; ++i){
		cout << "[ " << lines[i] << " : " << columns[i] << " - " << values[i] << "]" << endl;
	}
}
//----------------------------------------------------------------------------------
void matrix::print_CSR_lines()const{
	for(auto& item : lines){
		cout << item << " "; 
	}
}
//----------------------------------------------------------------------------------
double matrix::vector_product(vector<double>& res, vector<double> v){
	for(int i = 0; i < n_Columns; ++i){v.push_back(1.0);}
	auto start = chrono::steady_clock::now();
	product(lines, columns, values, csr, v, res);
	auto end = chrono::steady_clock::now();
	chrono::duration<double> time_elapsed = end - start;
	return time_elapsed.count();
}



//-----------------------------Functions for MPI-------------------------------------
matrix build_local_csr(const matrix& m_global, int rank, int P, vector<int>& global_rows){
	if (rank < 0 || rank >= P)
        throw runtime_error("Rank out of bounds");

    matrix m_local;
    m_local.columns = m_global.columns;

    //Determine the owned rows
    global_rows.clear();
    for (int i = rank; i < m_global.n_Rows; i += P) {
        global_rows.push_back(i);
    }

    m_local.n_Rows = static_cast<int>(global_rows.size());

    //Count all Local NonZeros
    int local_NonZeros = 0;
    for (int gr : global_rows) {
        local_NonZeros += m_global.lines[gr + 1] - m_global.lines[gr];
    }
    
    m_local.n_NonZeros = local_NonZeros;

    //Prepare the local CSR matrix
    m_local.lines.resize(m_local.n_Rows + 1);
    m_local.columns.resize(local_NonZeros);
    m_local.values.resize(local_NonZeros);
	
    int NonZeros_pos = 0;
    m_local.lines[0] = 0;

    for (int local_r = 0; local_r < m_local.n_Rows; local_r++) {
        int global_r = global_rows[local_r];

        int row_start = m_global.lines[global_r];
        int row_end   = m_global.lines[global_r + 1];

        for (int j = row_start; j < row_end; j++) {
            m_local.columns[NonZeros_pos] = m_global.columns[j];
            m_local.values[NonZeros_pos]    = m_global.values[j];
            NonZeros_pos++;
        }

        m_local.lines[local_r + 1] = NonZeros_pos;
    }

    return m_local;
}


