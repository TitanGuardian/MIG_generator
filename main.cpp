
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <sstream>

#include "permutation.h"
#include "mig.h"
int main () {
    MIG::bunch_check_update("final_1.txt");

//    MIG m;
//    m.compute();
//    std::cout << m.is_correct();

//    Permutation<6> a({1,4,5,2,0,3}), b({3,0,2,5,4,1}),c,d;
//
//
//    a.print();
//    b.print();
//    (a*b).print();
//    (c = ((a*b)*(Permutation<6>({5,4,3,2,1,0})))).print();
//    (d = c.reverse()).print();
//    (c*d).print();
//    Permutation<15>::ident().print();
//
//    Permutation<3> v({1,0,2});
//    std::vector<bool> rec =  v.apply<bool>({true, false , false});
//    for (auto it : rec) std::cout << it;

    return 0;
}