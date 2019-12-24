#include "mig.h"

#include <fstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <memory>
#include <sstream>
#include <list>

#define MAX(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))


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
    compute_node_order();
    for (uint32_t i=6 ; i < compute_seq.size() ; ++i  ) {
        //if (nodes[compute_seq[i]].computed) throw;
        nodes[compute_seq[i]].compute(*this);
    }

}


bool MIG::is_correct() {
    return (out_invert?~nodes[out].impl_func:nodes[out].impl_func) == vector;
}


MIG::MIG (const std::vector<std::string>& mig_str)  {

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
    compute_node_order();

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

bool MIG::bunch_check_update(const std::string& filename, BestSchemasDict & mig_lib
        , SearchMutation & sm, bool computed) {
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
            if (mig.complexity!=mig.nodes.size()-6) {
                std::cout << "wrong complexity";
                mig.complexity = mig.nodes.size()-6;
            }
            //
            if (!computed) {
                mig.vector = mig.out_invert?~mig.nodes[mig.out].impl_func:mig.nodes[mig.out].impl_func;

            }

            //
            if (mig.is_correct()) {
                ++ok_cnt;
                auto min_mut = sm.find_mincode(mig.vector);
                if (min_mut.vector!=(mig.out_invert?~mig.vector:mig.vector))
                    mig_apply(mig, min_mut);
                out_log <<mig.vector.to_ulong()<<" : OK" <<+" implemented: "
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

    std::ofstream out_log_file((filename+".log").c_str());
    out_log_file << "Circuits whole count: "<<count<<std::endl;
    out_log_file << "Invalid circuits count: "<<wrong_cnt<<std::endl;
    out_log_file << "Circuits with errors count: "<<error_cnt<<"\n\n"<<std::endl;
    out_log_file << out_log.str();

    return wrong_cnt==0;
}


bool MIG::mig_apply(MIG & mig, const Mutation & mutation) {
    auto m_vec = mig.vector;
    for (auto itr = 6; itr < mig.nodes.size() ; ++itr){
        auto& el = mig.nodes[itr];
        if (el.left>0 && el.left<=5) {
            el.left_inv = el.left_inv ^ mutation.negation[el.left - 1];
            el.left = mutation.variables[el.left-1]+1;
        }
        if (el.mid>0 && el.mid<=5) {
            el.mid_inv = el.mid_inv ^ mutation.negation[el.mid - 1];
            el.mid = mutation.variables[el.mid-1]+1;
        }
        if (el.right>0 && el.right<=5) {
            el.right_inv = el.right_inv ^ mutation.negation[el.right - 1];
            el.right = mutation.variables[el.right-1]+1;
        }

    }

    if (mig.out>0 && mig.out<=5) {
        mig.out_invert = mig.out_invert ^ mutation.negation[mig.out - 1];
        mig.out = mutation.variables[mig.out-1]+1;
    }

    mig.out_invert = mig.out_invert^mutation.invert_out;

    mig.compute();


    //delete for increase speed
//    if ((mig.out_invert?~mig.nodes[mig.out].impl_func:mig.nodes[mig.out].impl_func)!=mutation.vector) {
//        std::cout<<"\n"<< mig.out << std::endl;
//        for (auto & node : mig.nodes) {
//            std::cout<< node.second.impl_func << std::endl;
//        }
//        std::cout<< m_vec << std::endl;
//        std::cout<< mig.nodes[mig.out].impl_func << std::endl;
//        std::cout<< mutation.vector << std::endl;
//        std::cout<< mutation.variables <<std::endl;
//        std::cout<< mutation.negation<<std::endl;
//
//        throw std::logic_error("Wrong mutation");
//    }

    mig.vector = mig.out_invert?~mig.nodes[mig.out].impl_func:mig.nodes[mig.out].impl_func;

    return true;
}

std::string MIG::to_string() {
    std::ostringstream outs;
    outs << vector.to_ulong() << "\n";
    outs << "MIG\n";
    outs << complexity << "\n";
    outs << out << " " << out_invert << "\n";
    for (size_t itr = 6 ; itr < complexity+6 ; ++itr) {
        outs << (int) nodes[itr].left << " " <<nodes[itr].left_inv << " ";
        outs << (int) nodes[itr].mid << " " << nodes[itr].mid_inv << " ";
        outs << (int) nodes[itr].right << " " << nodes[itr].right_inv << " ";
    }
    return outs.str();
}

MIG MIG::mig_union(MIG &mig1, MIG &mig2, NodeNumber decomp) {
    MIG mig_res;
    mig_res.init_input_nodes();
    mig_res.complexity = mig1.complexity + mig2.complexity+3;
    mig_res.type=true;

    for (size_t itr = 6; itr < 6+mig1.complexity; ++itr) {
        mig_res.nodes[itr] = mig1.nodes[itr];
    }

    for (size_t itr = 6; itr < 6+mig2.complexity; ++itr) {
        Node new_node(mig2.nodes[itr]);
        if (new_node.left > 5) new_node.left += mig1.complexity;
        if (new_node.mid > 5) new_node.mid += mig1.complexity;
        if (new_node.right > 5) new_node.right += mig1.complexity;
        mig_res.nodes[itr+mig1.complexity] = std::move(new_node);
    }

    Node n1,n2,n3;
    n1.left = decomp;
    n1.left_inv = true;
    n1.mid = mig1.out;
    n1.mid_inv = mig1.out_invert;
    n1.right = 0;
    n1.right_inv = 0;
    mig_res.nodes[6+mig1.complexity+mig2.complexity] = std::move(n1);

    n2.left = decomp;
    n2.left_inv = false;
    n2.mid = (mig2.out>5?mig2.out+mig1.complexity:mig2.out);
    n2.mid_inv = mig2.out_invert;
    n2.right = 0;
    n2.right_inv = false;
    mig_res.nodes[7+mig1.complexity+mig2.complexity] = std::move(n2);

    n3.left = 6+mig1.complexity+mig2.complexity;
    n3.left_inv = false;
    n3.mid = 7+mig1.complexity+mig2.complexity;
    n3.mid_inv = false;
    n3.right = 0;
    n3.right_inv = true;
    mig_res.nodes[8+mig1.complexity+mig2.complexity] = std::move(n3);

    mig_res.out = 8+mig1.complexity+mig2.complexity;
    mig_res.out_invert = false;

    mig_res.compute();
    mig_res.vector = mig_res.out_invert?~mig_res.nodes[mig_res.out].impl_func:mig_res.nodes[mig_res.out].impl_func;


    return mig_res;
}


