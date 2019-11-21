//
// Created by davo1 on 13.11.2019.
//

#include "permutation.h"
//#include <algorithm>
//std::vector<unsigned int> permutation;

template <int size >
Permutation<size>::Permutation () : permutation(size) {

}

template<int size>
Permutation<size>::Permutation (const Permutation<size> & _permutation) {
    permutation = _permutation.permutation;
}

template<int size>
Permutation<size>::Permutation (Permutation<size> && _permutation)
    : permutation(std::move(_permutation.permutation)) {

}


template <int size>
Permutation<size>::Permutation (std::vector<unsigned int>&& _permutation) :
permutation(std::move(_permutation))
{}

template <int size>
Permutation<size>::Permutation (const std::vector<unsigned int>& _permutation) :
permutation(_permutation)
        {}

template <int size>
Permutation<size>& Permutation<size>::operator= (const Permutation<size> & rhs) {
    permutation = rhs.permutation;
    return  *this;
}


template <int size>
Permutation<size>& Permutation<size>::operator= ( Permutation<size> && rhs) {
    permutation = std::move(rhs.permutation);
    return  *this;
}


template <int size>
Permutation<size> Permutation<size>::operator* (const Permutation & rhs) const {
    std::vector<unsigned int> res(size);
    for (size_t itr = 0 ; itr < size ; ++itr) {
        res[itr] = this->permutation[rhs.permutation[itr]];
    }
    return res;
}


template <int size>
void Permutation<size>::print() const {
    for (auto item : permutation)
        std::cout << item <<" ";
    std::cout<<std::endl;
}

template <int size>
Permutation<size> Permutation<size>::ident() {
    Permutation res;
    std::iota(res.permutation.begin(), res.permutation.end(), 0);
    return res;
}


template <int size>
Permutation<size> Permutation<size>::reverse() const {
    Permutation<size> res_reverse;
    for (size_t itr = 0 ; itr < size ; ++itr) {
        res_reverse.permutation[this->permutation[itr]] = itr;
    }
    return res_reverse;
}

template <int size>
template <class T>
std::vector<T> Permutation<size>::apply(const std::vector<T> & vector) {
    std::vector<T>  new_vect(size);
    for (size_t itr = 0 ; itr < size ; ++itr) {
        new_vect[itr] = vector[this->permutation[itr]];
    }
    return new_vect;
}