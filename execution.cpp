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

using namespace std;
using namespace fplll;
///
IntMatrix* update(IntMatrix* B);
bool check_solution(IntMatrix *B);
///
StopWatch timer;
///stream used to save the statistic results of execution
ostringstream execution_logs;
///
struct comparator {
    bool operator()(const Integer a, const Integer b) const {
        return a < b;
    }
};

/**
 * first
 * @param B
 * @return
 */
int first(IntMatrix *B) {
    timer.reset();  ///reset the timer
    int block_size; ///represents the number of blocksize
    bool solution;  ///represents the possible solution found

    for(int k=1; k<6; ++k) {
        B = update(B); ///update the matrix with permutation and sorting
        block_size = (int)pow(2.0, k); ///setting the blocksize
        bkz_reduction(*B, block_size); ///execute bkz reduction
        solution = check_solution(B); ///checking for a solution

        if(solution) {
            long timeCounted = timer.elapsed<chrono::seconds>().count();
            int minutes = timeCounted/60;
            double seconds = timeCounted - (minutes * 60.0);
            execution_logs<<"Solution found for round: "<<k<<" : "<<block_size<<endl;
            execution_logs<<"Time: "<<minutes<<" m and "<<seconds<<" s"<<endl;
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
int second_extreme(IntMatrix *C, int start, int end, int step) {
    timer.reset(); ///reset the timer
    bool solution; ///represents the possible solution found
    IntMatrix *D = new IntMatrix(C->get_rows(), C->get_cols());

    for(int k = start; k < end; k += step) {
        *D = *C; ///reset the matrix
        vector<Strategy> strategies;
        ///the directory path of the below instruction should be set to the one containing the "default.json" file
        strategies = load_strategies_json("/home/edaravig/fplll-v5/share/fplll/strategies/default.json");
        BKZParam params(k, strategies);
        bkz_reduction(D, NULL, params); ///bkz reduction
        solution = check_solution(D); ///checking for a solution

        if(solution) {
            long time_counted = timer.elapsed<chrono::seconds>().count();
            int minutes = time_counted/60;
            double seconds = time_counted - (minutes * 60.0);
            execution_logs<<"Solution found with extreme pruning"<<" for round: "<<k-24<<" : "<<k<<endl;
            execution_logs<<"Time: "<<minutes<<" m and "<<seconds<<" s"<<endl;
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
int second_linear(IntMatrix *C, int start, int end, int step) {
    timer.reset(); ///reset the timer
    bool solution; ///represents the possible solution found
    IntMatrix *D = new IntMatrix(C->get_rows(), C->get_cols());
    int s; ///represents the pruning gradient
    int start_pruning = 10;

    for(int k=start; k<end; k+=step) {
        *D = *C;///reset the matrix for linear pruning
        s = (k % 3) + start_pruning;

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
            long time_counted = timer.elapsed<chrono::seconds>().count();
            int minutes = time_counted/60;
            double seconds = time_counted - (minutes * 60.0);
            execution_logs<<"Solution found with linear pruning"<<" for round: "<<k-24<<" : "<<k<<endl;
            execution_logs<<"Time: "<<minutes<<" m and "<<seconds<<" s"<<endl;
            return k;
        }
    }
    return -1;
}


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
 *
 * @return
 */
string retrieve_execution_logs () {
    return execution_logs.str();
}


/**
 *
 */
void clear_execution_logs() {
    execution_logs.str("");
    execution_logs.clear();
}