#ifndef MIG_GENERATOR_PERMUTATION_H
#define MIG_GENERATOR_PERMUTATION_H

#include <vector>
#include <numeric>
#include <iostream>


template <int size = 32>
class Permutation {
public:
    std::vector<unsigned int> permutation;

    Permutation ();
    Permutation (std::vector<unsigned int>&& _permutation);
    Permutation (const std::vector<unsigned int>& _permutation) ;

    Permutation (const Permutation & _permutation);
    Permutation (Permutation&& _permutation);

    Permutation& operator= (const Permutation & rhs) ;
    Permutation& operator= ( Permutation && rhs) ;

    Permutation operator*(const Permutation & rhs) const;

    Permutation reverse () const;
    static Permutation ident();

    template <class T>
    std::vector<T> apply(const std::vector<T> & vector);

    void print() const;


};

template <int size>
class PermutationGenerator {
public:

    std::vector< Permutation<size> > aaa;


    PermutationGenerator ();

    void generate_permutation (int n);

};

#include "permutation.hpp"

#endif //MIG_GENERATOR_PERMUTATION_H
