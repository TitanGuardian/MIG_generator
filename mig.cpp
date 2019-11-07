#include <fstream>
#include "mig.h"
#include <algorithm>


Node::Node(std::uint32_t _number, std::uint32_t _left, bool _left_inv, std::uint32_t _mid, bool _mid_inv,
           std::uint32_t _right, bool _right_inv)
           :    number(_number)
           ,    left(_left)
           ,    left_inv(_left_inv)
           ,    mid(_mid)
           ,    mid_inv(_mid_inv)
           ,    right(_right)
           ,    right_inv(_right_inv)
           {

           }

Node::Node(std::uint32_t _number, std::vector<bool> _impl_func)
    :   number(_number)
    ,   impl_func(_impl_func)
{
}



Node::operator uint32_t() {
    return number;
}


MIG::MIG (std::string fname) {
    file_name = fname;
    read_from_file();
}

void MIG::read_from_file() {
    std::ifstream input(file_name);
    uint32_t mincode;
    input >> mincode;
    vector = convert_to_bool(mincode);
    std::string type_str;
    input >> type_str;
    type = type_str == "mig";
    input >> complexity;
    input >> out;
    input >> out_invert;
    init_input_nodes();

    // read nodes from file
    for (uint32_t it = 6 ; input.eof() ; ++it) {
        uint32_t  left,mid, right;
        bool left_inv, mid_inv, right_inv;
        input >> left >> left_inv;
        input >> mid >> mid_inv;
        input >> right >> right_inv;
        nodes[it] = Node(it, left, left_inv, mid, mid_inv, right, right_inv);

    }

}

std::vector<bool> MIG::convert_to_bool(uint32_t number) {
    std::vector<bool> res;
    for (int i=0 ; i < 32 ; ++i) {
        res.push_back(number%2);
        number /= 2;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

void MIG::init_input_nodes() {
    nodes[1] = Node(1,
            {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
                       true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true}
             );
    nodes[2] = Node(2,
            {false,false,false,false,false,false,false,false,true,true,true,true,true,true,true,true,
                       false,false,false,false,false,false,false,false,true,true,true,true,true,true,true,true}
             );
    nodes[3] = Node(3,
                    {false,false,false,false,true,true,true,true,false,false,false,false,true,true,true,true,
                               false,false,false,false,true,true,true,true,false,false,false,false,true,true,true,true}
    );
    nodes[4] = Node(4,
                    {false,false,true,true,false,false,true,true,false,false,true,true,false,false,true,true,
                               false,false,true,true,false,false,true,true,false,false,true,true,false,false,true,true}
    );
    nodes[5] = Node(5,
                    {false,true,false,true,false,true,false,true,false,true,false,true,false,true,false,true,
                               false,true,false,true,false,true,false,true,false,true,false,true,false,true,false,true}
    );
}

