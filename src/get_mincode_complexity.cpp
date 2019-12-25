
#include <iostream>
#include <fstream>
#include <sstream>

#include "mig.h"


int main (int argc, char** argv) {
    BestSchemasDict ml("data");

    std::ofstream out (argv[1]);
    int c = std::stoi(argv[2]);
    for (auto & el : ml.dict) {
        if (c==el.second.complexity) {
            out<<el.first<<" "<<c-1<<" ";
        }
    }


    return 0;
}



