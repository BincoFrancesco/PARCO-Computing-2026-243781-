#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <omp.h>
#include <chrono>
#define Jump 16
using namespace std;

int partition(vector<int>& v, vector<int>& c, vector<int>& values, int low, int high);
void quickSort(vector<int>& v, vector<int>& c, vector<int>& values, int low, int high);
void compress(vector<int>& v);
void product(vector<int> const lines, vector<int> const columns, vector<int> const values, bool const csr, vector<int> const v, vector<int>& res);
void store(double value);


class COO{
protected:
    vector<int> lines;
    vector<int> columns;
    vector<int> values;
    int index;
    int nColums;
    bool csr;
public:
    COO(){index = 0; nColums = 0; csr = false;}
    ~COO(){}
    void set_index(int i){index = i;}
    void set_Colums(int c){nColums = c;}
    void adding_elements(int l, int c, int v){ lines.push_back(l); columns.push_back(c); values.push_back(v); }
    int get_index()const{return index;}
    int get_Colums()const{return nColums;}
    bool get_CSR()const{return csr;}
    void print_matrix()const{
        cout << "Matrix = " << endl;
        for(int i = 0; i < index; ++i){
            cout << lines[i] << " / " << columns[i]<< " / " << values[i] << " | ";
        }
        cout << endl;
    }
    void print_sizes()const{
        cout << "sizes: " << lines.size() << " " << columns.size() << " " << values.size() << endl;
    }
    void byRows(){
        quickSort(lines, columns, values, 0, lines.size()-1); //It doesn't need to be ordered also on the colums zone
    }
    void inCSR(){
        compress(lines);;
        csr = true;
    }
    void multiply(vector<int> const v, vector<int>& res){
        product(lines, columns, values, csr, v, res);
    }
};

void extract(COO& matrix, string s);
void product(vector<int> const lines, vector<int> const columns, vector<int> const values, bool const csr, vector<int> const v, vector<int>& res);


int main(int argc, char** argv){
    COO matrix1, matrix2, matrix3, matrix4, matrix5;
    string s1 = "494_bus.mtx", s2 = "ex37.mtx", s3 = "G61.mtx", s4 = "G67.mtx", s5="marine1.mtx";
    
    extract(matrix1,s1); matrix1.byRows(); matrix1.inCSR();
    extract(matrix2,s2); matrix2.byRows(); matrix2.inCSR();
    extract(matrix3,s3); matrix3.byRows(); matrix3.inCSR();
    extract(matrix4,s4); matrix4.byRows(); matrix4.inCSR();
    extract(matrix5,s5); matrix5.byRows(); matrix5.inCSR();
    
    vector<int> v1,v2,v3,v4,v5;
    srand(time(0));
    for(int i = 0; i < matrix1.get_Colums(); ++i){v1.push_back(rand()%10);}
    for(int i = 0; i < matrix2.get_Colums(); ++i){v2.push_back(rand()%10);}
    for(int i = 0; i < matrix3.get_Colums(); ++i){v3.push_back(rand()%10);}
    for(int i = 0; i < matrix4.get_Colums(); ++i){v4.push_back(rand()%10);}
    for(int i = 0; i < matrix5.get_Colums(); ++i){v5.push_back(rand()%10);}
    
    vector<int> res1, res2, res3, res4, res5;
    
    for(int i = 0; i < 10; ++i){
        auto start1 = chrono::steady_clock::now();
        matrix1.multiply(v1,res1);
        auto end1 = chrono::steady_clock::now();

        auto start2 = chrono::steady_clock::now();
        matrix2.multiply(v2,res2);
        auto end2 = chrono::steady_clock::now();

        auto start3 = chrono::steady_clock::now();
        matrix3.multiply(v3,res3);
        auto end3 = chrono::steady_clock::now();

        auto start4 = chrono::steady_clock::now();
        matrix4.multiply(v4,res4);
        auto end4 = chrono::steady_clock::now();

        auto start5 = chrono::steady_clock::now();
        matrix5.multiply(v5,res5);
        auto end5 = chrono::steady_clock::now();

        chrono::duration<double> t1 = end1 - start1;
        chrono::duration<double> t2 = end2 - start2;
        chrono::duration<double> t3 = end3 - start3;
        chrono::duration<double> t4 = end4 - start4;
        chrono::duration<double> t5 = end5 - start5;    

        store(t1.count());
        store(t2.count());
        store(t3.count());
        store(t4.count());
        store(t5.count());
    }
    
    return 0;
}

void extract(COO& matrix, string const s){
    string ss;
    int n_values,n_Colums,l,c; float v;
    ifstream str(s);
    if(!str.is_open()){cout << "Error opening the file";return;}
    for(int i = 0; i < Jump; ++i){
        if(i<13){getline(str, ss);}
        else{if(i==14){str >> n_Colums;}else{str >> n_values;}}
    }
    matrix.set_index(n_values);
    matrix.set_Colums(n_Colums);

    for(int i = 0; i<matrix.get_index(); ++i){
        str >> l; str >> c; str >> v;
        matrix.adding_elements(l,c,v);
    }
    str.close();    
}
int partition(vector<int>& v, vector<int>& c, vector<int>& values, int low, int high){
    int pivot = v[low];
    int i = low -1;
    int j = high +1;

    while(true){
        do{
            ++i;
        }while(v[i]< pivot);
        do{
            --j;
        }while(v[j]>pivot);

        if(i >= j){return j;}

        swap(v[i], v[j]);
        swap(c[i], c[j]);
        swap(values[i], values[j]);
    }

}
void quickSort(vector<int>& v, vector<int>& c, vector<int>& values, int low, int high){
    if(low < high){
        int pi = partition(v, c, values, low, high);
        quickSort(v, c, values, low, pi);
        quickSort(v, c, values, pi+1, high);
    }
}
void compress(vector<int>& v){
    vector<int> w;
    int nE = 0;
    int nL = 1;
    int i = 0;
    for(int i = 0; nL <= v[(v.size()-1)]; ++i){
        if(nL == v[i]){
            ++nE;
        } else {
            --i;
            ++nL;
            w.push_back(nE);
            nE = 0;
        }
    }
    v.clear();
    for(int i = 0; i < w.size(); ++i){
        v.push_back(w[i]);
    }
}
void product(vector<int> const lines, vector<int> const columns, vector<int> const values, bool const csr, vector<int> const v, vector<int>& res){
    if(csr == false){cout << "!!You need to convert the Matrix in the CSR format!!";return;}
    int sum = 0;
    int pointer = 0;
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(static,1)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(static,8)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(static,64)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(static,1)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(static,8)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(static,64)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(static,1)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(static,8)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(static,64)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(static,1)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(static,8)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(static,64)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(static,1)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(static,8)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(static,64)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(static,1)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(static,8)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(static,64)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(guided,1)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(guided,8)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(guided,64)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(guided,1)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(guided,8)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(guided,64)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(guided,1)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(guided,8)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(guided,64)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(guided,1)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(guided,8)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(guided,64)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(guided,1)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(guided,8)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(guided,64)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(guided,1)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(guided,8)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(guided,64)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(dynamic,1)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(dynamic,8)
    //#pragma omp parallel for collapse(2) num_threads(2) schedule(dynamic,64)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(dynamic,1)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(dynamic,8)
    //#pragma omp parallel for collapse(2) num_threads(4) schedule(dynamic,64)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(dynamic,1)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(dynamic,8)
    //#pragma omp parallel for collapse(2) num_threads(8) schedule(dynamic,64)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(dynamic,1)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(dynamic,8)
    //#pragma omp parallel for collapse(2) num_threads(16) schedule(dynamic,64)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(dynamic,1)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(dynamic,8)
    //#pragma omp parallel for collapse(2) num_threads(32) schedule(dynamic,64)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(dynamic,1)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(dynamic,8)
    //#pragma omp parallel for collapse(2) num_threads(64) schedule(dynamic,64)
    for(int i = 0; i < lines.size(); ++i){
        for(int j = 0; j < lines[i]; ++j){
            sum += (values[ pointer + j ]*v[columns[pointer + j]-1]);
        }
        res.push_back(sum);
        pointer += lines[i];
        sum = 0;
    }
}
void store(double value){
    ofstream str("Results.txt", ios::app);
    if(!str){cout << "Error in storing results"; return;}
    str << "Duration: " << value << endl;
    str.close();
}
