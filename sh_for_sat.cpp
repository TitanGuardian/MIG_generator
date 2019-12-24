
#include <fstream>
#include <string>


int main (int argc, char** argv) {
    std::ifstream in(argv[1]);
    std::ofstream out("sat.sh");

    std::string buf = "0";
    while (buf!="" & buf!="\n" & !in.eof()) {
        out << "./sat ";
        in >> buf ;
        out << buf << " ";
        in >> buf;
        out << buf << " &\n";
    }
    
    return 0;
}