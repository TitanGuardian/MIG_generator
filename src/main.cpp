
#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <sstream>

#include "mig.h"
#include "SearchMutation.h"
#include "BestSchemasDict.h"
#include "mig_generator.h"


#include <set>




int main () {
    BestSchemasDict my_lib("./data");
    SearchMutation sm;
    my_lib.out_statistic("some little circuits");
    std::cout<<my_lib.dict.size()<<"\n";

    return 0;
}



//

//    std::cout << "hfak";

//    std::cout <<sm.find_mincode(12).vector;

//    PermutationGenerator32::generate_permutation1();


//    my_lib.out_statistic("add aig db");


//    Simple_Generator my_gen(4);

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

//
//    SearchMutation my_searcher;
//    std::cout<<my_searcher.find_mincode(12300001).vector;
//    SearchMutation sm;
//    std::set<uint32_t> mincodes;
//    int prog = 0 ;
//    uint64_t crap = 4294967296 / 1000000;
//    for (uint64_t itr1 = 0 ; itr1 < 4294967296 ; ++itr1) {
//        mincodes.insert(sm.find_mincode(itr1).vector.to_ulong());
//        if (itr1%crap==0) {++prog;std::cout << prog <<std::endl; }
//        if (prog==10) break;
//    }
//    std::cout << mincodes.size();