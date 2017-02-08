//
// Created by edaravig on 12/12/2016.
//
#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>
#include <gmp.h>
#include <fplll.h>

using namespace std;
using namespace fplll;

#define z1 1;
#define z2 2;
///
vector<Integer>* generate_problem(int n, double density);
vector<int>* random_vector(int n);
Integer dot_product(vector<Integer> *A, vector<int> *B);
IntMatrix* create_matrix(vector<Integer> *a, Integer inner, int n, int N);
///stream used to save the statistic results of execution
ostringstream initial_logs;

/**
 * generate_problem
 * @return
 */
vector<Integer>* generate_problem(int n, double density) {
    const int d = density;                                                  ///reform density to its integer value
    vector<Integer> *a = new vector<Integer>;                               ///represents the vector a of the problem
    random_device rd;                                                       ///random device used for initializing uniform distribution
    unsigned int random_num;                                                ///random number for seed
    unsigned int lower_bound = 1;                                           ///lower bound for seed
    unsigned int upper_bound = USHRT_MAX;                                   ///upper bound for seed
    uniform_int_distribution<unsigned int> unif(lower_bound, upper_bound);  ///declaration of uniform distribution
    int bit = n * (2 - d);                                                  ///number of bits

    mp_bitcnt_t bits; bits = bit;   ///number of bits for gmp
    mpz_t seed;                     ///represents the seed used in each iteration
    mpz_t random_int;               ///represents the random integer retrieved from gmp
    gmp_randstate_t grt;            ///represents the state of the gmp random machine

    for(int i=0; i<n; ++i) { ///generate random coefficients in the range [1, 2^n]
        Integer z;                              ///used for the Integer value of the random number
        random_num = unif(rd);                  ///get a random number for seeding
        mpz_init_set_ui(seed, random_num);      ///set the number to the seed
        gmp_randinit_default(grt);              ///initialize state
        gmp_randseed(grt, seed);                ///initialize random machine
        mpz_init(random_int);                   ///initialize the random integer
        mpz_urandomb(random_int, grt, bits);    ///get a random num of n bits
        z = random_int;                         ///turn it to Integer
        gmp_randclear(grt);                     ///clear generator
        a->push_back(move(z));                  ///append it to vector
    }
    return a; ///return the vector
}


/**
 * initialize: initializing the values for the execution
 * @return B: the matrix (IntMatrix) used for the problem
 */
IntMatrix* initialize(int n, double d) {

    vector<Integer> *a = generate_problem(n, d);///generating the coefficients

    ///print density of the problem
    Integer max_v; max_v=0;
    vector<Integer>::iterator itM;
    for(itM=a->begin(); itM<a->end(); ++itM) {
        max_v = (*itM > max_v) ? *itM : max_v;
    }
    double logged = log2(max_v.get_d());
    double density = n/logged;
    initial_logs<<"Density is: "<<density<<endl;

    vector<int> *solution = random_vector(n);///generating a subset sum solution
    Integer inner = dot_product(a, solution); ///getting the inner product
    int N = ceil(sqrt(n)) * 2 - 2;  ///computation of N
    IntMatrix* B = create_matrix(a, inner, n, N); ///creating the basic matrix

    ///deleting the unnecessary object
    delete a;
    delete solution;

    ///returning the generated matrix
    return B;
}


/**
 * create_matrix: creates a matrix (IntMatrix) according to the paper (to elaborate)
 * @param a: vector containing random coefficients
 * @return B: the matrix (IntMatrix) used for the problem
 */
IntMatrix* create_matrix(vector<Integer> *a, Integer inner, int n, int N) {

    Integer zN; zN = N;
    Integer zN2; zN2 = n/2;

    IntMatrix* B = new IntMatrix(n+1, n+3);///the basis matrix
    Integer z;///used as an assisting variable

    for(int i=0; i<n; ++i) {
        B[0][i][i] = z2;
        B[0][n][i] = z1;
        ///for a coefficients
        z = a->at(i); z.mul(zN, z);
        B[0][i][n] = z;
        B[0][i][n+2] = zN;
    }
    B[0][n][n+1] = z1;
    z.mul(zN, inner); B[0][n][n] = z;
    z.mul(zN2, zN); B[0][n][n+2] = z;
    return B;
}


/**
 * random_vector: generates a solution of subset sum problem
 * @return randV: a permuted vector with H '1' and N-H '0'
 */
vector<int>* random_vector(int n) {
    int ham = n/2; ///hamming weight of the problem
    vector<int> *random_v = new vector<int>; ///represents the vector a

    for(int i=0; i<n-ham; ++i) {
        random_v->push_back(0);
    }
    for(int i=n-ham; i<n; ++i) {
        random_v->push_back(1);
    }

    random_device rd;
    shuffle(random_v->begin(), random_v->end(), rd);
    return random_v; ///return the vector
}


/**
 * dot_product: returns the inner product of two vectors
 * @param A: the first vector containing the a coefficients
 * @param B: the second vector containing the solution
 */
Integer dot_product(vector<Integer> *A, vector<int> *B) {
    Integer a; Integer b;
    Integer mul; mul=0;
    Integer sum; sum=0;

    ///set the sizes
    int lenA = A->size();
    int lenB = B->size();

    if (lenA!=lenB) { ///checks if the sizes of both vectors are equal in order to compute the inner product
        cerr<<"Function: dotProduct"<<endl<<"Vectors sizes are unequal"<<endl;
    }

    for(int i=0; i<lenA; ++i) {
        a = A->at(i);
        b = B->at(i);
        mul.mul(a, b);
        sum.add(sum, mul);
    }
    return sum;
}


string retrieve_init_logs() {
    return initial_logs.str();
}


void clear_init_logs() {
    initial_logs.str("");
    initial_logs.clear();
}