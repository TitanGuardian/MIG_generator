
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <omp.h>
#include <iostream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
using namespace std::chrono;

int main (int argc, char** argv) {
    std::ifstream in(argv[1]);
    std::vector<std::pair<std::string , std::string> > list;



    while (!in.eof()) {
        std::pair<std::string, std::string> tmp;
        in >> tmp.first  ;
        if (in.eof() || tmp.first=="" || tmp.first == "\n") break;
        in >> tmp.second ;
        if (in.eof() || tmp.first=="" || tmp.first == "\n") break;
        list.emplace_back(std::move(tmp));
    }
    auto start = high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic, 1)
    for (auto itr = 0 ; itr < list.size() ; ++itr){
        system(("./sat "+ list[itr].first +" " +list[itr].second+" "+argv[2]).c_str());
    }
    std::ofstream out ("out.txt", std::ios::app) ;

    std::string path = "./nets";
    for (const auto & entry : fs::directory_iterator(path)) {
        std::ifstream ifs(entry.path());
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        out << content;
    }
    auto stop = high_resolution_clock::now();
    std::cout<<"\nTime total: " << (duration_cast<seconds>(stop - start)).count()<<"\n";
    return 0;
}