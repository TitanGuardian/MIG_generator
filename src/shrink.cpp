#include <iostream>
#include <fstream>
#include <sstream>
#include "mig.h"

int main (int argc, char** argv) {
    SearchMutation sm;
    for (size_t i=1; i<argc; ++i ) {
        MIG::shrink_file(argv[i],sm);
    }
    return 0;
}



