
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <omp.h>


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

#pragma omp parallel for
    for (auto itr = 0 ; itr < list.size() ; ++itr){
        system(("./sat "+ list[itr].first +" " +list[itr].second+" "+argv[2]).c_str());
    }


    return 0;
}