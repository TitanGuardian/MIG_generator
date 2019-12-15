//
// Created by davo1 on 22.11.2019.
//


// to complete



#ifndef MIG_GENERATOR_SEARCHTREE_H
#define MIG_GENERATOR_SEARCHTREE_H

#include "permutation.h"
#include <vector>
#include <map>
#include <memory>

struct Mutation {
    std::bitset<32> vector;
    Permutation<5> variables;
    std::bitset<5> negation;
    bool invert_out;
};

class SearchMutation {
public:
    struct TreeNode {
        std::vector<PermutationGenerator32::Permutation_vars*> available_mutations;
        std::map<unsigned int, std::shared_ptr<TreeNode>> next_level_nodes;
    };

    std::shared_ptr<TreeNode> decision_tree_root;
    size_t tree_deep;


    std::vector<PermutationGenerator32::Permutation_vars> mutations;

    SearchMutation();


    void compute_all_nodes();

    Mutation find_mincode(std::bitset<32> func_vector) const;
    Mutation find_mutation(std::bitset<32> find_from,std::bitset<32> find_this) const;
};




#endif //MIG_GENERATOR_SEARCHTREE_H
