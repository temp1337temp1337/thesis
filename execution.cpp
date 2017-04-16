//
// Created by edaravig on 12/12/2016.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <random>
#include <map>
#include <gmp.h>
#include <fplll.h>
#include "StopWatch.h"
#include "Parameters.h"
#include "initMatrix.h"

using namespace std;
using namespace fplll;

struct comparator {
    bool operator()(const Integer a, const Integer b) const {
        return a < b;
    }
};


/**
 * check_solution: checks for a solution given the matrix of the problem
 * @param B: the matrix of the problem
 * @return -1 if no solution found otherwise returns the solution
 */
bool check_solution(IntMatrix *B) {

    const int n = B->get_rows()-1;  ///retrieve dimension of problem
    Integer z;                      ///used in order to examine each number of the matrix
    Integer z1; z1 = 1;             ///constant variable for Integer equal to 1
    Integer z0; z0 = 0;             ///constant variable for Integer equal to 0
    bool check = false;             ///temporal value representing the evaluation of the conditions of a possible solution

    for(int j = 0; j < n; ++j) { ///iterate at the n first columns of the first row
        z.abs(B[0][0][j]); ///setting to the absolute value
        if (z.cmp(z1) == 0) { ///reassure that all values are 0
            check = true; ///continuing
        }
        else { ///otherwise return
            return false; ///no solution was found
        }
    }

    if (check) {
        Integer zn1; zn1.abs(B[0][0][n+1]); ///constant variable for Integer equal to the value of B[1, n+2]
        Integer zn;  zn.abs(B[0][0][n]);    ///constant variable for Integer equal to the value of B[1, n+1]
        Integer zn2; zn2.abs(B[0][0][n+2]); ///constant variable for Integer equal to the value of B[1, n+3]
        if ((zn1.cmp(z1) == 0) && (zn.cmp(z0) == 0) && (zn2.cmp(z0) == 0)) { ///checking the evaluation of the remaining conditions
            return true; ///the solution was found properly
        }
    }
    return false; ///if any of the remaining conditions fail, no solution was found
}


/**
 *
 * @param B
 * @return
 */
IntMatrix *update(IntMatrix* B) {

    int n = B->get_rows()-1;    ///retrieve dimension of problem
    Integer z0; z0 = 0;         ///constant variable for Integer equal to 0
    vector<int> permuted;       ///contains the rows to be permuted
    vector<int> sorted;         ///contains the rows to be sorted

    ///we first permute the rows of the basis matrix such that the first rows have a nonzero entry in column n+2,
    ///and then sort the vectors of the basis according to their length preserving the particular initial rows.
    for(int i=0; i<n+1; ++i) {
        if(B[0][i][n+1]!=z0) {
            permuted.push_back(i);
        }
        else {
            sorted.push_back(i);
        }
    }

    ///sort scope
    multimap<Integer, int, comparator> norms;
    Integer squared, sum;   ///represents the variables for the norm computation
    for(vector<int>::iterator it = sorted.begin(); it!=sorted.end(); ++it) { ///iteratively compute the length of the rows to be sorted
        sum = 0;
        squared = 1;
        for(int j=0; j<n+3; ++j) { ///compute the length of row iteratively
            squared.mul(B[0][*it][j], B[0][*it][j]);
            sum.add(sum, squared);
        }
        norms.insert(make_pair(sum, *it));
    }

    ///permutation scope
    minstd_rand gen;
    shuffle(permuted.begin(), permuted.end(), gen); ///shuffle permuted vector
    IntMatrix* C = new IntMatrix(n+1, n+3); ///the updated matrix

    int to = permuted.size(); ///represents the ending index of permuted rows
    for(int i=0; i<to; ++i) { ///set the rows to the new matrix
        for(int j=0; j<n+3; ++j) {
            C[0][i][j] = B[0][permuted.at(i)][j];
        }
    }
    multimap<Integer, int>::iterator itr = norms.begin();
    for(int i=to; i<n+1; ++i) {
        int row = itr->second;
        for(int j=0; j<n+3; ++j) {
            C[0][i][j] = B[0][row][j];
        }
        ++itr;
    }
    return C; ///return the updated matrix
}


/**
 * first
 * @param B
 * @return
 */
int first(IntMatrix *B, StopWatch timer) {

    timer.reset(); ///count the time
    int block_size; ///represents the number of blocksize
    bool solution;  ///represents the possible solution found

    for(int k=1; k<6; ++k) {
        B = update(B); ///update the matrix with permutation and sorting
        block_size = (int)pow(2.0, k); ///setting the blocksize
        bkz_reduction(*B, block_size); ///execute bkz reduction
        solution = check_solution(B); ///checking for a solution

        if(solution) {
            long time_counted_seconds = timer.elapsed<chrono::seconds>().count();
            long time_counted_minutes = timer.elapsed<chrono::minutes>().count();
            time_counted_seconds -= time_counted_minutes * 60;
            cerr<<"Solution found for round: "<<k<<" : "<<block_size<<endl;
            cerr<<"Time: "<<time_counted_minutes<<" m and "<<time_counted_seconds<<" s"<<endl;
            return block_size;
        }
    }
    return -1;
}


/**
 *
 * @param C
 * @param start
 * @param end
 * @param step
 * @return
 */
int second_extreme(IntMatrix *C, int start, int end, int step, vector<Strategy> strategies, StopWatch timer) {

    timer.reset(); ///reset the timer
    bool solution; ///represents the possible solution found
    IntMatrix *D = new IntMatrix(C->get_rows(), C->get_cols());

    for(int k = start; k < end; k += step) {
        *D = *C; ///reset the matrix
        BKZParam params(k, strategies);
        bkz_reduction(D, NULL, params); ///bkz reduction
        solution = check_solution(D); ///checking for a solution

        if(solution) {
            long time_counted_seconds = timer.elapsed<chrono::seconds>().count();
            long time_counted_minutes = timer.elapsed<chrono::minutes>().count();
            time_counted_seconds -= time_counted_minutes * 60;
            cerr<<"Solution found with extreme pruning"<<" for round: "<<k-24<<" : "<<k<<endl;
            cerr<<"Time: "<<time_counted_minutes<<" m and "<<time_counted_seconds<<" s"<<endl;
            return k;
        }
    }
    return -1;
}


/**
 *
 * @param C
 * @param start
 * @param end
 * @param step
 * @return
 */
int second_linear(IntMatrix *C, int start, int end, int step, StopWatch timer) {

    timer.reset(); ///reset the timer
    bool solution; ///represents the possible solution found
    IntMatrix *D = new IntMatrix(C->get_rows(), C->get_cols());
    int s; ///represents the pruning gradient
    int start_pruning = 10;

    for(int k=start; k<end; k+=step) {
        *D = *C;///reset the matrix for linear pruning
        s = (k % 3) + start_pruning;

        ///TODO: below should be probably only once instasiated
        vector<Strategy> strategies;
        for (int b = 0; b < k; b++) {
            strategies.emplace_back(move(Strategy::EmptyStrategy(b)));
        }
        Strategy strategy;
        strategy.pruning_parameters.emplace_back(Pruning::LinearPruning(k, s)); ///linear pruning
        strategies.emplace_back(move(strategy));
        BKZParam params(k, strategies);
        bkz_reduction(D, NULL, params); ///bkz reduction
        solution = check_solution(D); ///checking for a solution

        if (solution) {
            long time_counted_seconds = timer.elapsed<chrono::seconds>().count();
            long time_counted_minutes = timer.elapsed<chrono::minutes>().count();
            time_counted_seconds -= time_counted_minutes * 60;
            cerr<<"Solution found with linear pruning"<<" for round: "<<k-24<<" : "<<k<<endl;
            cerr<<"Time: "<<time_counted_minutes<<" m and "<<time_counted_seconds<<" s"<<endl;
            return k;
        }
    }
    return -1;
}


/**
 *
 * @param p
 */
void experiment(Parameters p) {

    const int n = p.get_dimension();                                ///dimension of the problem
    const double density = p.get_density();                         ///density of the problem
    bool first_b = p.get_first();                                   ///first stage
    bool s_linear = p.get_second_linear();                          ///second linear
    bool s_extreme = p.get_second_extreme();                        ///second extreme
    const char* strategies_filename = p.get_strategies_filename();  ///strategies filename
    const int start = p.get_second_start();                         ///starting round for block size of the second round
    const int end = p.get_second_end();                             ///ending round for block size of the second round
    int step = p.get_second_step();                                 ///step for the iteration of the second round
    int number_of_instances = p.get_number_of_instances();          ///number of instances to be executed of the experiment

    for(int noi=1; noi<=number_of_instances; ++noi) {

        initMatrix im;
        im.initialize(n, density);

        cerr<<"Start of problem with number: "<<noi<<" ###############################################################"<<endl<<endl;
        cerr<<"Generated Matrix is shown below"<<endl<<endl;
        cerr<<*im.get_matrix()<<endl<<endl;
        cerr<<"Solution to be found is shown below"<<endl<<endl;
        cerr<<*im.get_solution()<<endl<<endl;

        StopWatch timer;

        int success;
        if(first_b) {
            success = first(im.get_matrix(), timer);
        }

        if (success == -1) {

            if(s_linear) {
                success = second_linear(im.get_matrix(), start, end, step, timer);
            }
            if(s_extreme) {
                vector<Strategy> strategies;
                strategies = load_strategies_json(strategies_filename);
                success = second_extreme(im.get_matrix(), start, end, step, strategies, timer);
            }
        }
        cerr<<"End of problem with number: "<<noi<<"#########"<<endl;
        cerr<<"#######################################################################################################"<<endl<<endl;
    }
}