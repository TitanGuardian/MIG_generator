#ifndef MIG_GENERATOR_PERMUTATION_H
#define MIG_GENERATOR_PERMUTATION_H

#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <fstream>

template <size_t size = 32>
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
    bool operator== (const Permutation & rhs);
    Permutation operator*(const Permutation & rhs) const;

    Permutation reverse () const;
    static Permutation ident();

    std::bitset<32> apply(const std::bitset<32> & vector);

    std::string print() const;


};

class PermutationGenerator32 {
public:
    struct Permutation_vars {
        Permutation<5> variable_order;
        Permutation<32> vector_permutation;
        std::bitset<5> variable_negation;

        Permutation_vars (  const Permutation<5> & _order,
                            const Permutation<32> & _vector,
                            std::bitset<5> _negation = 0
                        );

        Permutation_vars (  const Permutation_vars& copy_from);
        Permutation_vars ( Permutation_vars && move_from);


        Permutation_vars operator* (const Permutation_vars & rhs);

        bool operator== (const Permutation_vars& rhs);

    };

    std::vector<Permutation_vars> all_permutations;
    std::vector<Permutation_vars> negation_mutations;
    std::vector<Permutation_vars> mutations;

    PermutationGenerator32 ();

    void generate_permutation ();
    void generate_negation();
    void generate_final_mutations();

    void print_mutations(const std::string &filename);
};


//template definitions

template <size_t size>
Permutation<size>::Permutation () : permutation(size) {

}

template <size_t size>
Permutation<size>::Permutation (const Permutation<size> & _permutation) {
    permutation = _permutation.permutation;
}

template <size_t size>
Permutation<size>::Permutation (Permutation<size> && _permutation)
        : permutation(std::move(_permutation.permutation)) {

}


template <size_t size>
Permutation<size>::Permutation (std::vector<unsigned int>&& _permutation) :
        permutation(std::move(_permutation))
{}

template <size_t size>
Permutation<size>::Permutation (const std::vector<unsigned int>& _permutation) :
        permutation(_permutation)
{}

template <size_t size>
Permutation<size>& Permutation<size>::operator= (const Permutation<size> & rhs) {
    permutation = rhs.permutation;
    return  *this;
}


template <size_t size>
Permutation<size>& Permutation<size>::operator= ( Permutation<size> && rhs) {
    permutation = std::move(rhs.permutation);
    return  *this;
}


template <size_t size>
Permutation<size> Permutation<size>::operator* (const Permutation & rhs) const {
    std::vector<unsigned int> res(size);
    for (size_t itr = 0 ; itr < size ; ++itr) {
        res[itr] = this->permutation[rhs.permutation[itr]];
    }
    return res;
}


template <size_t size>
bool Permutation<size>::operator==(const Permutation &rhs) {
    return this->permutation == rhs.permutation;
}

template <size_t size>
std::string Permutation<size>::print() const {
    std::ostringstream out;
    for (auto& item : permutation)
        out << item <<" ";
    return  out.str();
}

template <size_t size>
Permutation<size> Permutation<size>::ident() {
    Permutation res;
    std::iota(res.permutation.begin(), res.permutation.end(), 0);
    return res;
}


template <size_t size>
Permutation<size> Permutation<size>::reverse() const {
    Permutation<size> res_reverse;
    for (size_t itr = 0 ; itr < size ; ++itr) {
        res_reverse.permutation[this->permutation[itr]] = itr;
    }
    return res_reverse;
}

template <size_t size>
std::bitset<32> Permutation<size>::apply(const std::bitset<32> & vector) {
    std::bitset<32> new_vect;
    for (size_t itr = 0 ; itr < size ; ++itr) {
        new_vect[size-1-itr] = vector[size-1-this->permutation[itr]];
    }
    return new_vect;
}

#endif //MIG_GENERATOR_PERMUTATION_H
