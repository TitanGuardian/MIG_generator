
#include <iostream>
#include <fstream>
#include <sstream>


#include "mig.h"
#include "schema_info.h"

int main (int argc, char** argv) {
    SearchMutation sm;
    auto info1 = Info::GetMonotoneInfo(54279363,sm); //380225912
    auto info = Info::GetSymmetryInfo(54279363*65536+54279363,sm);
    std::cout << info;
    return 0;
}
