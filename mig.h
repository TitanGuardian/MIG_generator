#ifndef MIG_H
#define MIG_H

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <bitset>

//do not change
#define FANIN 5
#define  MAXLOOP 100

typedef std::bitset<32> FuncVect;
typedef uint32_t NodeNumber;

struct MIG;

struct Node {
    class ExceptionCanNotCompute: std::exception {};
	NodeNumber number;
    NodeNumber left;
	bool left_inv;
    NodeNumber mid;
    bool mid_inv;
    NodeNumber right;
    bool right_inv;
    bool computed;
    explicit operator NodeNumber ();
    FuncVect impl_func;

    Node () = default;
    Node (  NodeNumber  _number,
            NodeNumber _left, bool _left_inv,
            NodeNumber _mid, bool _mid_inv,
            NodeNumber _right, bool _right_inv
            );

    Node (  NodeNumber _number,
            const FuncVect &&_impl_func
            );

    Node (const Node& copy) = default;
    Node& operator=(const Node& copy) = default;

    void compute(MIG &);

};

struct MIG {
    class ExceptionInvalidInput: std::exception {};
    std::map<uint32_t,Node> nodes;
    FuncVect vector;
    uint32_t complexity;
    uint32_t out;
    bool out_invert;
    bool type;
    std::vector<uint32_t> compute_seq;


    explicit MIG (const std::vector<std::string>& mig_str);

    bool is_correct();
//    static std::bitset<32> convert_to_bitset(uint64_t number);
//    static uint32_t convert_to_uint32 (const std::bitset<32> & vector);
    void init_input_nodes();
    void compute();
    static bool bunch_check_update(const std::string& filename);

};



#endif //MIG_H