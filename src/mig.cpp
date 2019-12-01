#include "mig.h"

#include <fstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <memory>
#include <sstream>
#include <list>

#define MAX(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))


//bool operator== (const std::vector<bool> &lhs, const std::vector<bool> &rhs) {
//    if (lhs.size()!=rhs.size()) return false;
//    for (size_t i = 0 ; i < lhs.size() ; ++ i ) {
//        if (lhs[i]!=rhs[i]) return false;
//    }
//    return true;
//}

Node::Node(NodeNumber _number, NodeNumber _left, bool _left_inv, NodeNumber _mid, bool _mid_inv,
           NodeNumber _right, bool _right_inv)
           :    number(_number)
           ,    left_inv(_left_inv)
           ,    mid_inv(_mid_inv)
           ,    right_inv(_right_inv)
           ,    left(_left)
           ,    mid(_mid)
           ,    right(_right)
           {
               computed = false;
           }

Node::Node(NodeNumber _number, const FuncVect && _impl_func)
    :   number(_number)
    ,   impl_func(_impl_func)
    ,   left(0)
    ,   mid(0)
    ,   right(0)
    ,   left_inv(false)
    ,   mid_inv(false)
    ,   right_inv(false)
    {
        computed = true;
    }


Node::operator NodeNumber () {
    return number;
}

void Node::compute(MIG & mig) {
    if (!mig.nodes[left].computed || !mig.nodes[mid].computed || !mig.nodes[right].computed)
        throw ExceptionCanNotCompute();


    impl_func =    ((left_inv? ~mig.nodes[left].impl_func: mig.nodes[left].impl_func)
                        & (mid_inv? ~mig.nodes[mid].impl_func: mig.nodes[mid].impl_func))
                 |  ((left_inv? ~mig.nodes[left].impl_func: mig.nodes[left].impl_func)
                        & (right_inv? ~mig.nodes[right].impl_func: mig.nodes[right].impl_func))
                 |  ((mid_inv? ~mig.nodes[mid].impl_func: mig.nodes[mid].impl_func)
                        & (right_inv? ~mig.nodes[right].impl_func: mig.nodes[right].impl_func));

    computed = true;
}

void MIG::compute() {
    for (uint32_t i=6 ; i < compute_seq.size() ; ++i  ) {
        //if (nodes[compute_seq[i]].computed) throw;
        nodes[compute_seq[i]].compute(*this);
    }

}


bool MIG::is_correct() {
    return (out_invert?~nodes[out].impl_func:nodes[out].impl_func) == vector;
}


MIG::MIG (const std::vector<std::string>& mig_str)  {
    //read_from_file();
    //
    //std::cout <<std::endl<< mincode;
    {
        uint32_t mincode;
        std::istringstream input(mig_str[0]);
        input >> mincode;
        vector = std::bitset<32>(mincode);
    }
    {
        std::string type_str;
        std::istringstream input(mig_str[1]);
        input >> type_str;
        type = type_str == "mig"||type_str == "MIG";
    }
    {
        std::istringstream input(mig_str[2]);
        input >> complexity;
    }
    {
        std::istringstream input(mig_str[3]);
        input >> out;
        input >> out_invert;
    }

    init_input_nodes();

    // read nodes from file
    {
        std::istringstream input(mig_str[4]);
        uint32_t it;
        for (it = 6;  it < complexity+6; ++it) {
            uint32_t left, mid, right;
            bool left_inv, mid_inv, right_inv;
            input >> left >> left_inv;
            input >> mid >> mid_inv;
            if (input.eof()) break;
            input >> right >> right_inv;
            nodes[it] = Node(it, left, left_inv, mid, mid_inv, right, right_inv);
        }
        if (it == MAXLOOP) throw ExceptionInvalidInput();
    }
    //

    compute_node_order();
}


MIG::MIG(const BestSchema& schema_info, const std::vector<SchemaNode>& schema_nodes) {

    vector = std::bitset<32>(schema_info.mincode);
    type = true;
    complexity = schema_info.complexity;
    out = schema_info.out_node;
    out_invert = schema_info.out_invert;
    init_input_nodes();
    // read nodes from schema_nodes
    {
        uint_fast32_t it;
        for (it = 0; it < complexity; ++it) {
            bool left_inv, mid_inv, right_inv;
            switch ((uint8_t) schema_nodes[it].invert) {
                case 0:
                    left_inv = false; mid_inv = false; right_inv = false;
                    break;
                case 1:
                    left_inv = false; mid_inv = false; right_inv = true;
                    break;
                case 2:
                    left_inv = false; mid_inv = true; right_inv = false;
                    break;
                case 3:
                    left_inv = false; mid_inv = true; right_inv = true;
                    break;
                case 4:
                    left_inv = true; mid_inv = false; right_inv = false;
                    break;
                case 5:
                    left_inv = true; mid_inv = false; right_inv = true;
                    break;
                case 6:
                    left_inv = true; mid_inv = true; right_inv = false;
                    break;
                case 7:
                    left_inv = true; mid_inv = true; right_inv = true;
                    break;
                default:
                    throw ExceptionInvalidInput();
            }
            nodes[it+6] = Node(it+6, schema_nodes[it].node_left, left_inv,
                               schema_nodes[it].node_mid, mid_inv,
                               schema_nodes[it].node_right, right_inv);
        }
    }
    //
    compute_seq.resize(6);
    compute_seq.reserve(complexity+6);
    std::iota(compute_seq.begin(),compute_seq.begin()+6 , 0);

    compute_node_order();

//    std::sort(compute_seq.begin(),compute_seq.end(),
//              [&](const uint32_t &x, const uint32_t &y) -> bool
//              {
//                  uint32_t  u = MAX(nodes[x].left, nodes[x].mid, nodes[x].right);
//                  uint32_t  v = MAX(nodes[y].left, nodes[y].mid, nodes[y].right);
//                  return  u<v;
//              }
//    );
}

void MIG::compute_node_order() {
    compute_seq.resize(6);
    compute_seq.reserve(complexity+6);
    std::iota(compute_seq.begin(),compute_seq.begin()+6 , 0);


    std::vector<bool> can_compute(complexity+6,false);
    for (size_t it = 0; it < 6 ; ++it ) {
        can_compute[it] = true;
    }
    std::vector<uint32_t > list;
    for (size_t it = 6; it < complexity+6 ; ++it ) {
        list.push_back(it);
    }
    for(;;) {
        size_t start = list.size();
        for (size_t it = 0; it < list.size() ; ++it) {

            if (    can_compute[nodes[list[it]].left] &&
                    can_compute[nodes[list[it]].mid] &&
                    can_compute[nodes[list[it]].right] ) {
                can_compute[list[it]] = true;
                compute_seq.push_back(list[it]);
                list.erase(list.begin()+it);
                --it;
            }
        }
        if (start == 0) break;
        if(start == list.size()) {
            std::cout<<this->vector.to_ulong()<<std::endl;
            throw Node::ExceptionCanNotCompute();
        }
    }
}






//std::bitset<32> MIG::convert_to_bitset(uint64_t number) { //ok
//    std::bitset<32> res;
//    for (size_t i=0 ; i < 32 ; ++i) {
//        res[i]=(number%2);
//        number /= 2;
//    }
//    if (number != res.to_ullong()) std::cout<<"WTF!!!!\n";
//    return res;
//}
//uint32_t MIG::convert_to_uint32 (const std::bitset<32> &vector) { //ok
//    uint32_t  res = 0;
//    for (int i=31 ; i >= 0 ; --i) {
//        res += std::pow(2, 31-i)*vector[i];
//    }
//    return res;
//}


void MIG::init_input_nodes() {
    nodes[0] = Node(0, 0);
    nodes[1] = Node(1, 65535);
    nodes[2] = Node(2,16711935);
    nodes[3] = Node(3,252645135);
    nodes[4] = Node(4,858993459);
    nodes[5] = Node(5,1431655765);
}


std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }

    throw;

}

bool MIG::bunch_check_update(const std::string& filename, BestSchemasDict & mig_lib) {
    size_t  wrong_cnt = 0 ;
    size_t  error_cnt = 0 ;
    size_t  ok_cnt = 0 ;
    uint32_t count = 0;
//    std::string s = ;
    std::istringstream ss (get_file_contents(filename.c_str()));
    std::ostringstream out_log;
    while (!ss.eof()) {
        std::vector<std::string> mig_str(5);
        std::getline(ss, mig_str[0]);
        std::getline(ss, mig_str[1]);
        std::getline(ss, mig_str[2]);
        std::getline(ss, mig_str[3]);
        if (ss.eof()) break;
        std::getline(ss, mig_str[4]);

        try {
            MIG mig(mig_str);
            mig.compute();
            if (mig.is_correct()) {
                ++ok_cnt;
                out_log <<mig_str[0]+" : OK" <<+" implemented: "
                        << (mig.out_invert?~mig.nodes[mig.out].impl_func:mig.nodes[mig.out].impl_func)
                        << (mig_lib.add(mig)?" is better":" is not better")
                        << "\n";
            }
            else {
                ++wrong_cnt;
                out_log << mig_str[0]<<+" : invalid, implemented: "
                        << (mig.out_invert?~mig.nodes[mig.out].impl_func:mig.nodes[mig.out].impl_func)
                        << ", but considered " << mig.vector
                        << "\n";
            }

        }
        catch (Node::ExceptionCanNotCompute&) {
            ++error_cnt;
            out_log <<mig_str[0]+" : error \n";
            std::cout << "Can't compute!\n";
        }
        catch (ExceptionInvalidInput&) {
            ++error_cnt;
            out_log <<mig_str[0]+" : error \n";
        }
        ++count ;
    }

    std::ofstream out_log_file(("log"+filename+".txt").c_str());
    out_log_file << "Circuits whole count: "<<count<<std::endl;
    out_log_file << "Invalid circuits count: "<<wrong_cnt<<std::endl;
    out_log_file << "Circuits with errors count: "<<error_cnt<<"\n\n"<<std::endl;
    out_log_file << out_log.str();

    return wrong_cnt==0;
}