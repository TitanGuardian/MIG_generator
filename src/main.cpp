
#include <iostream>
#include <fstream>
#include <sstream>

#include "mig.h"


int main (int argc, char** argv) {
    SearchMutation sm;
    BestSchemasDict ml("data");
    MIG::bunch_check_update("zed.txt", ml,sm,false);

    ml.out_statistic("some schemas to compl 5");
    //ml.save_to_txt("migs.txt");
    return 0;
}
