//
// Created by davo1 on 22.11.2019.
//

#include "SearchMutation.h"
#include <map>
#include <set>

SearchMutation::SearchMutation () {
    {
        PermutationGenerator32 my_gen;
        mutations = std::move(my_gen.mutations);
    }
}


void SearchMutation::compute_all_nodes() {
    size_t  level = 0;
    bool need_new_level = true;
    std::vector<std::shared_ptr<TreeNode>> list_for_compute;
    list_for_compute.push_back(decision_tree_root);
    while (need_new_level) {
        need_new_level = false;
        std::vector<std::shared_ptr<TreeNode>> next_list_for_compute;
        for (auto &node : list_for_compute) {
            std::map<unsigned int, TreeNode> new_nodes;
            for (auto mutation : node->available_mutations) {
                new_nodes[mutation->vector_permutation.permutation[level]].available_mutations.push_back(
                        mutation);
            }
            size_t it = 0 ;
            for (auto& el : new_nodes) {
                if (el.second.available_mutations.size() > 1) {
                    need_new_level = true;
                }
                next_list_for_compute.push_back(std::make_shared<TreeNode>(el.second));
                node->next_level_nodes[el.first] = next_list_for_compute[it];
                ++it;
            }
        }
        list_for_compute = std::move(next_list_for_compute);
        ++level;
    }
    tree_deep = level;
}




Mutation SearchMutation::find_mincode(std::bitset<32> func_vector) const {
    std::vector<std::shared_ptr<TreeNode>> nodes_for_look,next_nodes_for_look;

    size_t count_of_1 =0;
    for (size_t it = 0 ; it < 32 ; ++it) {
        if (func_vector[it]) ++count_of_1;
    }
    bool invert_out = 0;


    std::bitset<32> min = -1;

    const PermutationGenerator32::Permutation_vars *min_mutation;

    for (auto & el : mutations) {
        if (el.vector_permutation.apply(func_vector).to_ulong() < min.to_ulong()) {
            min_mutation = &el;
            min = el.vector_permutation.apply(func_vector).to_ulong();
            invert_out = 0;
        }
        if (el.vector_permutation.apply(~func_vector).to_ulong() < min.to_ulong()) {
            min_mutation = &el;
            min = el.vector_permutation.apply(~func_vector).to_ulong();
            invert_out = 1;
        }
    }


    Mutation res;
    res.vector = min;

    res.invert_out = invert_out;
    res.negation = min_mutation ->variable_negation;
    res.variables = min_mutation->variable_order;
    return res;
}

Mutation SearchMutation::find_mutation(std::bitset<32> find_from, std::bitset<32> find_this) const {
    size_t count_of_1_from =0;
    size_t count_of_1_this =0;
    size_t count_of_0_this =0;
    size_t count_of_0_from =0;
    for (size_t it = 0 ; it < 32 ; ++it) {
        if (find_from[it]) ++count_of_1_from;
        else ++count_of_0_from;
        if (find_this[it]) ++count_of_1_this;
        else ++count_of_0_this;
    }
    bool ok = false;

    bool invert_out = count_of_1_from > count_of_1_this;
    if (invert_out) {
        find_from = ~find_from;
    }


    const PermutationGenerator32::Permutation_vars *that_mutation;
    for (auto & el : mutations) {
        if (el.vector_permutation.apply(find_from) == find_this) {
            that_mutation = &el;
            ok = true;
            break;
        }
    }
    if (!ok) throw std::logic_error("no such mutation");
    Mutation res;
    if (that_mutation) {
        res.vector = that_mutation->vector_permutation.apply(find_from);
        res.invert_out = invert_out;
        res.negation = that_mutation->variable_negation;
        res.variables = that_mutation->variable_order;
    }
    return res;
}

MIG SearchMutation::get_MIG(std::bitset<32> func_vector, BestSchemasDict& min_mig) {
    auto min_mutation = find_mincode(func_vector);
    MIG new_mig = min_mig.dict[min_mutation.vector.to_ulong()];
    min_mutation.negation=min_mutation.variables.apply(min_mutation.negation);
    min_mutation.variables = min_mutation.variables.reverse();
    MIG::mig_apply(new_mig, min_mutation);
    return new_mig;
}




