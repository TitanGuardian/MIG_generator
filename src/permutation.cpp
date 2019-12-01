//
// Created by davo1 on 13.11.2019.
//

#include "permutation.h"
#include <algorithm>
//std::vector<unsigned int> permutation;



//Generator

PermutationGenerator32::PermutationGenerator32() {
    // permutations
    all_permutations.reserve(120); //5!

    all_permutations.emplace_back(Permutation<5>({0,1,2,3,4}),
                                  Permutation<32>({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
                                                   24,25,26,27,28,29,30,31}));
    all_permutations.emplace_back(Permutation<5>({1,0,2,3,4}),
            Permutation<32>({0,1,2,3,4,5,6,7,16,17,18,19,20,21,22,23,
                                                 8,9,10,11,12,13,14,15,24,25,26,27,28,29,30,31}));
    all_permutations.emplace_back(Permutation<5>({4,0,1,2,3}),
            Permutation<32>({0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23,
                                                 8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31}));

    generate_permutation();

    negation_mutations.reserve(32);

    negation_mutations.emplace_back(
              Permutation<5>({0,1,2,3,4})
            , Permutation<32> ({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
                              24,25,26,27,28,29,30,31})
            , std::bitset<5>("00000")
                    );


    negation_mutations.emplace_back(
            Permutation<5>({0,1,2,3,4})
            , Permutation<32> ({16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15})
            , std::bitset<5>("00001")
    );

    negation_mutations.emplace_back(
            Permutation<5>({0,1,2,3,4})
            , Permutation<32> ({8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
                                24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23})
            , std::bitset<5>("00010")
    );

    negation_mutations.emplace_back(
            Permutation<5>({0,1,2,3,4})
            , Permutation<32> ({4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11,
                                20, 21, 22, 23, 16, 17, 18, 19, 28, 29, 30, 31, 24, 25, 26, 27 })
            , std::bitset<5>("00100")
    );

    negation_mutations.emplace_back(
            Permutation<5>({0,1,2,3,4})
            , Permutation<32> ({2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13, 18, 19,
                                16, 17, 22, 23, 20, 21, 26, 27, 24, 25, 30, 31, 28, 29
                               })
            , std::bitset<5>("01000")
    );

    negation_mutations.emplace_back(
            Permutation<5>({0,1,2,3,4})
            , Permutation<32> ({1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16,
                                19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 29, 28, 31, 30})
            , std::bitset<5>("10000")
    );

    generate_negation();

    mutations.reserve(3840);
    generate_final_mutations();
}


void PermutationGenerator32::generate_permutation() {
    size_t it=0, it_end=3;
    it_end = all_permutations.size();

    for (size_t it_a = 1; it_a < it_end ; ++it_a) {
        for (size_t it_b = 1; it_b < it_end; ++it_b) {
            Permutation_vars composition(all_permutations[it_a] * all_permutations[it_b]);
            auto find_rez = std::find(all_permutations.begin(),all_permutations.end(), composition);
            if (find_rez==all_permutations.end()) {
                all_permutations.emplace_back(std::move(composition));
            }
        }
        it_end = all_permutations.size();
    }
}


void PermutationGenerator32::generate_negation() {
    size_t it=0, it_end=3;
    it_end = negation_mutations.size();

    for (size_t it_a = 1; it_a < it_end ; ++it_a) {
        for (size_t it_b = 1; it_b < it_end; ++it_b) {
            Permutation_vars composition(negation_mutations[it_a] * negation_mutations[it_b]);
            auto find_rez = std::find(negation_mutations.begin(),negation_mutations.end(), composition);
            if (find_rez==negation_mutations.end()) {
                negation_mutations.emplace_back(std::move(composition));
            }
        }
        it_end = negation_mutations.size();
    }
}

void PermutationGenerator32::generate_final_mutations() {

    for (auto& perm : all_permutations) {
        for (auto & neg : negation_mutations) {
            Permutation_vars composition(neg*perm);
            auto find_rez = std::find(mutations.begin(), mutations.end(), composition);
            if (find_rez==mutations.end()) {
                mutations.emplace_back(std::move(composition));
            }
        }
    }
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