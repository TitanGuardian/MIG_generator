//
// Created by davo1 on 24.11.2019.
//

#ifndef MIG_GENERATOR_MIG_GENERATOR_H
#define MIG_GENERATOR_MIG_GENERATOR_H

#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <thread>

#include "mig.h"


#define THREAD_COUNT 2048

class MIG_Generator {
public:

    std::vector<std::map<uint64_t,MIG>> generated_migs;

    void print_to_file(const std::string& filename);
};


class Simple_Generator : public MIG_Generator {
public:
    size_t complexity_bound;
    explicit Simple_Generator(size_t generate_until_this_complexity);

    std::mutex generated_migs_mutex;
    std::mutex cout_mutex;
    std::vector<bool> thread_ready;
    std::vector<std::thread> threads_tasks;
    void start();


    struct InputComplexity {
        uint8_t left;
        uint8_t mid;
        uint8_t right;

        InputComplexity(uint8_t _left, uint8_t _mid, uint8_t _right);

    };

    size_t wait_free_thread (std::vector<std::thread>& threads_tasks, std::vector<bool> &thread_ready);


    static void async_brute_force (Simple_Generator& this_gen, uint8_t level, InputComplexity in_comp); // async
    static std::vector<InputComplexity> get_input_complexity_list (uint_fast8_t cur_complexity);
    static void compute_migs( MIG & mig_left, MIG & mig_mid, MIG & mig_right,
                             const Simple_Generator::InputComplexity& in_compl,
                             Simple_Generator& this_gen,
                             uint8_t level, uint32_t thread_num);


};







#endif //MIG_GENERATOR_MIG_GENERATOR_H
