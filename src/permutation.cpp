//
// Created by davo1 on 13.11.2019.
//

#include "permutation.h"
#include <algorithm>
//std::vector<unsigned int> permutation;



//Generator

PermutationGenerator32::PermutationGenerator32() {
    mutations.reserve(3840);
    generate_final_mutations();
}


bool NextSet(std::vector<unsigned int> &perm)
{
    unsigned int n = perm.size();
    unsigned int j = n - 2;
    unsigned int tmp;
    while (j != -1 && perm[j] >= perm[j + 1]) j--;
    if (j == -1)
        return false; // больше перестановок нет
    int k = n - 1;
    while (perm[j] >= perm[k]) k--;

    tmp = perm[j];
    perm[j] = perm[k];
    perm[k] = tmp;

    int l = j + 1, r = n - 1; // сортируем оставшуюся часть последовательности
    while (l<r) {
        tmp = perm[l];
        perm[l] = perm[r];
        perm[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

void PermutationGenerator32::generate_final_mutations() {
    std::bitset<32> x[5];
    std::bitset<32> mut_x[5];

    x[0] =  65535;
    x[1] =  16711935;
    x[2] =  252645135;
    x[3] =  858993459;
    x[4] =  1431655765;

    Permutation<5> perm = Permutation<5>::ident();

    Permutation<32> mut_vect;

    do {
        for (size_t num = 0; num < 32; ++num) {
            std::bitset<5> neg_num = num;
            mut_x[0] = neg_num[0]?~x[perm[0]]:x[perm[0]];
            mut_x[1] = neg_num[1]?~x[perm[1]]:x[perm[1]];
            mut_x[2] = neg_num[2]?~x[perm[2]]:x[perm[2]];
            mut_x[3] = neg_num[3]?~x[perm[3]]:x[perm[3]];
            mut_x[4] = neg_num[4]?~x[perm[4]]:x[perm[4]];

            for (size_t itr = 0; itr<32 ; ++itr) {
                mut_vect.permutation[31-itr] =   16*mut_x[0][itr]
                                                +  8*mut_x[1][itr]
                                                +  4*mut_x[2][itr]
                                                +  2*mut_x[3][itr]
                                                +    mut_x[4][itr];
            }

            mutations.emplace_back(perm, mut_vect, neg_num);
        }
    } while (NextSet(perm.permutation));
}


PermutationGenerator32::Permutation_vars::Permutation_vars(const Permutation<5> &_order,
                                                           const Permutation<32> &_vector,
                                                           std::bitset<5> _negation)
                                                           : variable_order(_order)
                                                           , vector_permutation(_vector)
                                                           , variable_negation(_negation)
                                                           {
}

PermutationGenerator32::Permutation_vars::Permutation_vars(const PermutationGenerator32::Permutation_vars &copy_from)
    : variable_order(copy_from.variable_order)
    , vector_permutation(copy_from.vector_permutation)
    , variable_negation (copy_from.variable_negation)
{
}
PermutationGenerator32::Permutation_vars::Permutation_vars(PermutationGenerator32::Permutation_vars &&move_from)
    : variable_order(std::move(move_from.variable_order))
    , vector_permutation(std::move(move_from.vector_permutation))
    , variable_negation (std::move(move_from.variable_negation))
{
}



PermutationGenerator32::Permutation_vars PermutationGenerator32::Permutation_vars::operator*(
        const PermutationGenerator32::Permutation_vars &rhs) {

    return PermutationGenerator32::Permutation_vars(
            this->variable_order * rhs.variable_order,
            this->vector_permutation * rhs.vector_permutation,
            this->variable_negation^rhs.variable_negation
            );
}

bool PermutationGenerator32::Permutation_vars::operator==(const PermutationGenerator32::Permutation_vars &rhs) {

    return this->vector_permutation == rhs.vector_permutation;
}


void PermutationGenerator32::print_mutations(const std::string &filename) {
    std::ofstream out(filename.c_str());
    for (auto el : mutations) {
        out << el.vector_permutation.print()
            << "  Perm: " << el.variable_order.print()
            << "  Neg:  " << el.variable_negation[0]<<el.variable_negation[1]
            <<el.variable_negation[2]<<el.variable_negation[3]
            <<el.variable_negation[4]<<std::endl;
    }

}