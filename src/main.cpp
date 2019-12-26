
#include <iostream>
#include <fstream>
#include <sstream>

#include "mig.h"


int main (int argc, char** argv) {
    BestSchemasDict ml("data");
    SearchMutation sm;
    MIG::bunch_check_update("out.txt", ml,sm, false);
    ml.out_statistic("add sated 6");
    
    return 0;
}
