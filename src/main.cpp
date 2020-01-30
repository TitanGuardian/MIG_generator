
#include <iostream>
#include <fstream>
#include <sstream>

#include "mig.h"


int main (int argc, char** argv) {
    SearchMutation sm;
    BestSchemasDict ml("datas");
    MIG::bunch_check_update("rejsoned.txt",ml,sm, false);
    ml.out_statistic("sat 14 jan31");
    ml.save_to_txt("jan31migs.txt");
    return 0;
}
