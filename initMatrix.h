//
// Created by edaravig on 16/4/2017.
//

#ifndef THESIS_GITHUB_INITMATRIX_H
#define THESIS_GITHUB_INITMATRIX_H

#include <fplll.h>

using namespace fplll;

class initMatrix {
public:
    void initialize(int n, double d);
    IntMatrix *get_matrix();
    vector<int> *get_solution();

private:
    vector<Integer> *generate_problem(int n, double density);
    IntMatrix* create_matrix(vector<Integer> *a, Integer inner, int n, int N);
    vector<int>* random_vector(int n);
    Integer dot_product(vector<Integer> *A, vector<int> *B);
    vector<int> *solution;
    IntMatrix *B;

};


#endif //THESIS_GITHUB_INITMATRIX_H
