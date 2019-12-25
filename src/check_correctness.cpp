#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mig.h"

std::string get_file_content(const char *filename)
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

void check (char* name) {
    std::istringstream ss (get_file_content(name));
    std::vector<std::string> mig_str(5);
    std::getline(ss, mig_str[0]);
    std::getline(ss, mig_str[1]);
    std::getline(ss, mig_str[2]);
    std::getline(ss, mig_str[3]);
    std::getline(ss, mig_str[4]);

    MIG mig(mig_str);
    mig.compute();
    if (mig.is_correct()) {
        std::cout<<"Correct!\n";
    }
    else {

        std::cout<<"Not Correct!\n";
        auto f = mig.out_invert?~mig.nodes[mig.out].impl_func:mig.nodes[mig.out].impl_func;
        std::cout<<"Implemented "<<f<<" "<<f.to_ulong()<<"\n";
        std::cout<<"Considered  "<<mig.vector<<" "<<mig.vector.to_ulong()<<"\n";
        SearchMutation sm;
        auto s = sm.find_mincode(mig.vector);
        std::cout<<"MINCODE vec "<<s.vector<<" "<<s.vector.to_ulong()<<"\n";
        s = sm.find_mincode(f);
        std::cout<<"MINCODE f   "<<s.vector<<" "<<s.vector.to_ulong()<<"\n";
    }
}


int main (int argc, char** argv) {

    if (argv[1]=="-d") {

    } else check(argv[1]);


    return 0;
}



