//
// Created by davo1 on 24.11.2019.
// NEEEED IMPROVEMENTS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "mig_generator.h"

#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>
//#include <execution>
#include <omp.h>

#define CONST_MIN_COMPLEXITY 200000

void MIG_Generator::print_to_file(const std::string& filename) {
    std::cout << "Saving to file ... " << std::endl;
    std::ofstream out(filename.c_str());
    for (auto & el : generated_migs) {
            out << el.to_string() << "\n";
    }
}

#ifdef THREAD
Simple_Generator::Simple_Generator(size_t generate_until_this_complexity)
    : complexity_bound(generate_until_this_complexity)
    , threads_tasks(THREAD_COUNT)
    , thread_ready(THREAD_COUNT, true)
{
    std::cout << "Starting generation with Simple_Generation algorithm \n";
    start();
    print_to_file("simple.txt");
}

void Simple_Generator::start() {
    size_t current_complexity = 0; // change when add save/load of generation
    generated_migs.resize(complexity_bound+1);

    if (current_complexity == 0) {
        MIG new_mig;
        new_mig.init_input_nodes();
        new_mig.complexity = 0;
        new_mig.vector = 0;
        new_mig.out = 0;
        new_mig.out_invert = false;
        generated_migs[0][0] = new_mig;

        for (size_t itr = 1; itr < 6 ; ++itr) {
            new_mig.vector = new_mig.nodes[itr].impl_func;
            new_mig.out = itr;
            generated_migs[0][new_mig.vector.to_ulong()] = new_mig;
        }
        ++current_complexity;
    }

    for ( ; current_complexity <= complexity_bound; ++ current_complexity ) {
        for (auto & el : get_input_complexity_list(current_complexity))  {
            async_brute_force(*this, current_complexity, el);
        }
        for (size_t itr = 0; itr < THREAD_COUNT; ++itr) {
            if (threads_tasks[itr].joinable())
                threads_tasks[itr].join();
        }
        std::cout<<"End with complexity = " << current_complexity <<std::endl;
    }



}


size_t Simple_Generator::wait_free_thread (std::vector<std::thread> &threads_tasks,
                                           std::vector<bool> &thread_ready) {
    std::thread tmp;
    for(;;) {
        for (size_t itr = 0; itr < THREAD_COUNT; ++itr) {
            if (thread_ready[itr]) {
                if (threads_tasks[itr].joinable())
                    threads_tasks[itr].join();
                return itr;
            }
        }
    }
}


// very long code
void Simple_Generator::async_brute_force (Simple_Generator& this_gen, uint8_t level, const InputComplexity in_compl){

    for  (auto & mig_left: this_gen.generated_migs[in_compl.left]) {
        for  (auto & mig_mid: this_gen.generated_migs[in_compl.mid]) {
            for  (auto & mig_right: this_gen.generated_migs[in_compl.right]) {
                uint32_t thread_n = this_gen.wait_free_thread(this_gen.threads_tasks, this_gen.thread_ready);
                this_gen.thread_ready[thread_n] = false;

                this_gen.threads_tasks[thread_n] = std::move(std::thread(compute_migs,std::ref (mig_left.second),
                                                                std::ref (mig_mid.second),
                                                                std::ref (mig_right.second),
                                                                in_compl,std::ref(this_gen),level, thread_n));

            }
        }
    }
}


void Simple_Generator::compute_migs( MIG & mig_left, MIG & mig_mid, MIG & mig_right,
        const Simple_Generator::InputComplexity& in_compl, Simple_Generator& this_gen,
        const uint8_t level, uint32_t thread_num) {


    MIG mig_res;

    mig_res.init_input_nodes();
    mig_res.complexity = level;
    mig_res.type=true;


    for (size_t itr = 6; itr < 6+in_compl.left; ++itr) {
        mig_res.nodes[itr] = mig_left.nodes[itr];
    }

    for (size_t itr = 6; itr < 6+in_compl.mid; ++itr) {
        Node new_node(mig_mid.nodes[itr]);
        if (new_node.left > 5) new_node.left += in_compl.left;
        if (new_node.mid > 5) new_node.mid += in_compl.left;
        if (new_node.right > 5) new_node.right += in_compl.left;
        mig_res.nodes[itr+in_compl.left] = new_node;
    }

    for (size_t itr = 6; itr < 6+in_compl.right; ++itr) {
        Node new_node(mig_mid.nodes[itr]);
        if (new_node.left > 5) new_node.left += in_compl.left + in_compl.mid;
        if (new_node.mid > 5) new_node.mid += in_compl.left + in_compl.mid;
        if (new_node.right > 5) new_node.right += in_compl.left + in_compl.mid;
        mig_res.nodes[itr+in_compl.left+in_compl.mid]=new_node;;
    }

    mig_res.nodes[5+level].left = mig_left.out;

    if (mig_mid.out > 5) mig_res.nodes[5+level].mid = mig_mid.out + in_compl.left;
    else mig_res.nodes[5+level].mid = mig_mid.out;

    if (mig_right.out > 5) mig_res.nodes[5+level].right = mig_right.out
                                                                 + in_compl.left + in_compl.mid;
    else mig_res.nodes[5+level].right = mig_right.out;


    mig_res.out = level+5;
    mig_res.out_invert = false;

    for (size_t itr = 0 ; itr < 8 ; ++itr) {
        mig_res.nodes[5+level].left_inv = (itr / 4)==1;  // false : 0 1 2 3 // true 4 5 6 7
        mig_res.nodes[5+level].mid_inv = ( (itr % 4) / 2 )==1; // false : 0 1 4 5 // true 2 3 6 7
        mig_res.nodes[5+level].right_inv = (itr % 2)==1; // false: 0 2 4 6 // true 1 3 5 7

        mig_res.compute_node_order();
        mig_res.compute();
        mig_res.vector = mig_res.nodes[level+5].impl_func;
        bool better;
        uint32_t min = CONST_MIN_COMPLEXITY;
        for (size_t l_itr = 0 ; l_itr < level ; ++l_itr) {
            if (this_gen.generated_migs[l_itr].count(mig_res.vector.to_ullong()) !=0 ) {
                if (this_gen.generated_migs[l_itr][mig_res.vector.to_ullong()].complexity < min) {
                    std::lock_guard<std::mutex> lock(this_gen.generated_migs_mutex);
                    min = this_gen.generated_migs[l_itr][mig_res.vector.to_ullong()].complexity;
                }
            }
        }
        better = (level<min);

        //critical section
        if (better)
        {
            std::lock_guard<std::mutex> lock(this_gen.generated_migs_mutex);
            //std::cout << mig_res.vector.to_ullong() <<std::endl;
            this_gen.generated_migs[level][mig_res.vector.to_ullong()]=mig_res;
        }
    }
    this_gen.thread_ready[thread_num] = true;
}



Simple_Generator::InputComplexity::InputComplexity(uint8_t _left, uint8_t _mid, uint8_t _right)
    : left(_left)
    , mid(_mid)
    , right(_right)
{

}

// lazy iteration
std::vector<Simple_Generator::InputComplexity> Simple_Generator::get_input_complexity_list(uint8_t cur_complexity) {
    std::vector<Simple_Generator::InputComplexity> res;
    res.reserve(cur_complexity);
    for (uint8_t it_0 = 0 ; it_0 < cur_complexity; ++it_0) {
        for (uint8_t it_1 = 0 ; it_1 < cur_complexity; ++it_1) {
            for (uint8_t it_2 = 0 ; it_2 < cur_complexity; ++it_2) {
                if (it_0 >= it_1 && it_1 >= it_2 && (it_0+it_1+it_2 == cur_complexity -1)) {
                    res.emplace_back(it_0,it_1,it_2);
                }
            }
        }
    }
    return  res;
}
#endif



Shannon::Shannon() : mig4("4vars"), mig5_l("data", true), sm() {
    std::cout<< "Copy STAGE" << std::endl;
    copy_to_vector();
    generated_migs.resize(mig5.size());
}

void Shannon::gen_migs(NodeNumber decomp_var) {
    std::cout<< "Parallel STAGE" << std::endl;
    size_t j= 0;
    uint32_t var;
    int shift;
    switch (decomp_var) {
        case 1:
            var = 65535;
            shift = 16;
            break;
        case 2:
            var = 16711935;
            shift = 8;
            break;
        case 3:
            var = 252645135;
            shift = 4;
            break;
        case 4:
            var = 858993459;
            shift = 2;
            break;
        case 5:
            var = 1431655765;
            shift = 1;
            break;
        default: throw;
    }
    std::bitset<32> mask2 = var;
    std::bitset<32> mask1 = ~var;


    #pragma omp parallel
    {
        #pragma omp for
        for (size_t itr = 0; itr < mig5.size(); ++itr) { //
            auto f  = std::bitset<32>(mig5[itr]);
            auto f1 = (f&mask1) | ((f&mask1)>>shift);
            auto f2 = (f&mask2) | ((f&mask2)<<shift);

            auto mig1 = std::move(sm.get_MIG(f1, mig4));
            auto mig2 = std::move(sm.get_MIG(f2, mig4));

            generated_migs[itr] = std::move(MIG::mig_union(mig1,mig2,decomp_var));

            #pragma omp critical
            if (itr%10000 == 0) std::cout<< omp_get_thread_num()<< "  j = "<< j++ << std::endl;
        }
    }

}

void Shannon::copy_to_vector() {
    mig5.reserve(mig5_l.dict.size());
    for (auto& el: mig5_l.dict) {
        mig5.emplace_back(el.second.vector.to_ulong());
    }
}