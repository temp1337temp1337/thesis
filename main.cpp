
#include <iostream>
#include <gmp.h>
#include <fplll.h>
#include <chrono>

using namespace std;
using namespace fplll;
///
IntMatrix* initialize(int n, double density);
///
void set_parameters (int dimension, double density);
void print_to_file(const char* filename, string logs);
///
int first(IntMatrix *B);
int second_linear(IntMatrix *C, int start, int end, int step);
int second_extreme(IntMatrix *C, int start, int end, int step);
///
string retrieve_execution_logs();
void clear_execution_logs();
string retrieve_init_logs();
void clear_init_logs();
///
ostringstream message;


void print_to_file(const char* filename, string logs) {
    ///remove any previous logfiles
    //if(exists(filename)) remove(filename);

    ///creating the output file
    ofstream logfile;
    logfile.open (filename, ios::out | ios::app | ios::binary);
    logfile<<logs;///output the results
    logfile.close();///close the logfile
}


/**
 * experiment
 * @param number_of_instances
 */
void experiment(int number_of_instances) {
    const int n = 80;
    const double density = 1.0;
    const int start = 30;
    const int end = 62;
    int step = 1;
    const char* filename = "linear_vs_extreme_80.txt";

    for(int i=1; i<=number_of_instances; ++i) {

        cout<<"Start of problem with number: "<<i<<"#########"<<endl;
        IntMatrix* B = initialize(n, density);
        cout<<*B<<endl;
        message<<retrieve_init_logs()<<endl;
        clear_init_logs();
        IntMatrix* C = new IntMatrix(B->get_rows(), B->get_cols());
        *C = *B;

        int success = first(B);
        if (success == -1) {
            second_linear(C, start, end, step);
            second_extreme(C, start, end, step);
        }

        message<<retrieve_execution_logs()<<endl;
        clear_execution_logs();
        message<<"End of problem with number: "<<i<<"#########"<<endl;
        message<<"########################################"<<endl<<endl;
        print_to_file(filename, message.str());
        message.str(""); message.clear();

        delete B;
        delete C;
    }
}


/**
 * main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char* argv[]) {
    experiment(1);
    return EXIT_SUCCESS;
}

