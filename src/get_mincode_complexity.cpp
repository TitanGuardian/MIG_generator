
#include <iostream>
#include <fstream>
#include <sstream>

#include "mig.h"


int main (int argc, char** argv) {
    BestSchemasDict ml("data");
    std::string fname;
    if (argc > 3) {
        int workers = std::stoi(argv[3]);
        std::ofstream out[workers];
        for (int itr = 0; itr < workers; ++itr) {
            fname = std::string(argv[1]) + std::to_string(itr);
            out[itr].open(fname.c_str());
        }
        int c = std::stoi(argv[2]);
        int counter = 0;
        for (auto &el : ml.dict) {
            if (c == el.second.complexity) {
                out[counter % workers] << el.first << " " << c - 1 << " ";
                ++counter;
            }
        }
    }
    else {
        std::ofstream out(argv[1]);
        int c = std::stoi(argv[2]);
        for (auto &el : ml.dict) {
            if (c == el.second.complexity) {
                out << el.first << " " << c - 1 << " ";
            }
        }
    }
    return 0;
}



