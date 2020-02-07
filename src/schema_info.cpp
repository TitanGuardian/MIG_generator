#include "schema_info.h"

SymmetryInfo Info::GetSymmetryInfo(uint32_t mincode, SearchMutation & sm) {
    SymmetryInfo res;
    //std::cout << mincode <<"\n";
    for (size_t i = 0; i<4 ; ++i) {
        for (size_t j = i+1; j<=4 ; ++j) {
            std::vector<PermutationGenerator32::Permutation_vars> mp;
            std::vector<int> mpNeg;
            std::vector<unsigned int> my_mut = {0,1,2,3,4};
            my_mut[i] = j;
            my_mut[j] = i;
            std::bitset<5> my_neg1=0;
            my_neg1[i] = 1;
            std::bitset<5> my_neg2=0;
            my_neg2[j] = 1;
            std::bitset<5> my_neg3=0;
            my_neg3[i] = 1;
            my_neg3[j] = 1;

            for (auto a : sm.mutations) {
                if (a.variable_order==Permutation<5>(my_mut)) {
                    if (a.variable_negation==0) {
                        mp.push_back(a);
                        mpNeg.push_back(0);
                    }
                    if (a.variable_negation==my_neg1) {
                        mp.push_back(a);
                        mpNeg.push_back(1);
                    }
                    if (a.variable_negation==my_neg2) {
                        mp.push_back(a);
                        mpNeg.push_back(2);
                    }
                    if (a.variable_negation==my_neg3) {
                        mp.push_back(a);
                        mpNeg.push_back(3);
                    }
                }
            }
            if (mp.size()==0) continue;

            for (size_t itr =0 ; itr < mp.size() ; ++itr) {
                //std::cout<<mp[itr].vector_permutation.apply(mincode).to_ullong()<<"\n";
                if (mp[itr].vector_permutation.apply(mincode).to_ullong() == mincode) {
                    res.emplace_back(std::pair(i,j), mpNeg[itr] );
                }
            }
        }
    }
    return res;
}


MonotoneInfo Info::GetMonotoneInfo(uint32_t mincode, SearchMutation & sm) {
    MonotoneInfo res;
    std::bitset<32> vector = mincode;
    bool monotone=true;
    bool antimonotone=true;
    for (std::bitset<5> alpha = 0 ;  ; alpha=alpha.to_ulong()+1) {
        for (std::bitset<5> beta = alpha;; beta = beta.to_ulong() + 1) {
            bool less = true;
            for (int i = 0; i < 5; ++i) {
                if (alpha[i] > beta[i]) {
                    less = false;
                }
            }
            if (less && monotone && (vector[31 - alpha.to_ulong()] > vector[31 - beta.to_ulong()])) {
                monotone = false;
            }
            if (less && antimonotone && (vector[31 - alpha.to_ulong()] < vector[31 - beta.to_ulong()])) {
                antimonotone = false;
            }
            if (beta == -1) break;
        }
        if (alpha == -1) break;
    }
    std::cout<< monotone << " " << antimonotone << "\n";

    // TODO add search for every boolean variable

    //std::cout << mincode <<"\n";

/*
    for (int variable = 0; variable < 5; ++variable) {
        for (std::bitset<4> alpha = 0 ;  ; alpha=alpha.to_ulong()+1) {
            for (std::bitset<4> beta = alpha;; beta = beta.to_ulong() + 1) {
                std::bitset<5> tmp1,tmp2;
                tmp1 = alpha.to_ulong();
                for (int variable_c = 0; variable_c < variable; ++variable_c) {

                }

                if (beta == -1) break;
            }
            if (alpha == -1) break;
        }
    }

*/
    return res;
}



std::ostream& operator<<(std::ostream & out, const SymmetryInfo& info) {
    for (auto [first,second] : info) {
        out<< int(first.first) << " " << int(first.second)<<" " <<second<<"\n";
    }
    return out;
}