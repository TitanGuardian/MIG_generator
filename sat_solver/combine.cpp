
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std::chrono;

int main (int argc, char** argv) {
    std::ofstream out (argv[1], std::ios::app) ;
    std::string path = "./nets";
    for (const auto & entry : fs::directory_iterator(path)) {
        std::ifstream ifs(entry.path());
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        out << content;
    }
    return 0;
}