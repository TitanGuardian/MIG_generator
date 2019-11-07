#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>


#define FANIN 5

struct Node {
	std::uint32_t number;
	std::uint32_t left;
	bool left_inv;
	std::uint32_t mid;
    bool mid_inv;
    std::uint32_t right;
    bool right_inv;
    bool value;
    explicit operator uint32_t ();
    std::vector<bool> impl_func;

    Node (  std::uint32_t  _number,
            std::uint32_t _left, bool _left_inv,
            std::uint32_t _mid, bool _mid_inv,
            std::uint32_t _right, bool _right_inv
            );

    Node (  std::uint32_t _number,
            std::vector<bool> _impl_func
            );


};

struct MIG {

    std::map<uint32_t,Node> nodes;
    std::vector<bool> vector;
    uint32_t complexity;
    uint32_t out;
    bool out_invert;
    bool type;
    std::string file_name;



    MIG (std::string fname);
    void read_from_file();
    std::vector<bool> convert_to_bool(uint32_t number);
    void init_input_nodes();

};


int main() {


    return 0;
}