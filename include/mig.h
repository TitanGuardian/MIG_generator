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
#define  MAXLOOP 100

typedef std::bitset<32> FuncVect;
typedef uint8_t NodeNumber;

struct MIG;
struct Node;
#include "BestSchemasDict.h"
#include "SearchMutation.h"


struct MIG {
    class ExceptionInvalidInput: std::exception {};

    // вершины
    std::map<uint32_t,Node> nodes; //
    // bitset<32> вектор, реализуемый MIG
    FuncVect vector;
    // Сложность
    uint32_t complexity;
    // номер вершины выхода
    uint32_t out;
    // инвертируется ли выход
    bool out_invert;
    // бесполезная переменная
    bool type;

    // полезный вектор
    std::vector<uint8_t> compute_seq;


    explicit MIG (const std::vector<std::string>& mig_str);
    MIG (const BestSchema& schema_info, const std::vector<SchemaNode>& schema_nodes);


    MIG () = default;
    MIG (MIG && mig) = default;
    MIG (const MIG & mig) = default;
    MIG& operator= (MIG && mig) = default;
    MIG& operator= (const MIG& mig) = default;
    bool is_correct();

    void init_input_nodes();
    void compute();
    static bool bunch_check_update(const std::string& filename, BestSchemasDict & mig_lib
            , SearchMutation& sm, bool computed = true);
    static bool mig_apply(MIG & mig, const Mutation & mutation);



    void compute_node_order();

    std::string to_string();
    static MIG mig_union(MIG & mig1 , MIG & mig2, NodeNumber decomp);
    static MIG shrink (MIG & mig);
    static void shrink_file (const char * filename, SearchMutation & sm);



};

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

    FuncVect impl_func;


    explicit operator NodeNumber ();
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


#endif //MIG_H