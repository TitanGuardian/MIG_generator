#ifndef MIG_GENERATOR_SCHEMA_INFO_H
#define MIG_GENERATOR_SCHEMA_INFO_H

#include "SearchMutation.h"

using SymmetryInfo = std::vector<std::pair<std::pair<uint8_t,uint8_t>, int>>;
using MonotoneInfo = std::vector<std::pair<std::pair<int, int>,int>>;

std::ostream& operator<<(std::ostream & out, const SymmetryInfo& info);


struct Info {
    static SymmetryInfo GetSymmetryInfo (uint32_t mincode, SearchMutation & sm);
    static MonotoneInfo GetMonotoneInfo (uint32_t mincode, SearchMutation & sm);
};


#endif //MIG_GENERATOR_SCHEMA_INFO_H
