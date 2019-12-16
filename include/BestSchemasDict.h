//
// Created by davo1 on 25.11.2019.
//

#ifndef MIG_GENERATOR_BESTSCHEMASDICT_H
#define MIG_GENERATOR_BESTSCHEMASDICT_H

struct BestSchema;
struct SchemaNode;
struct BestSchemasDict;

#include "mig.h"
#include <map>
#include <string>
struct SchemaNode { // size 4 byte
    unsigned node_left:8;
    unsigned node_mid:8;
    unsigned node_right:8;
    unsigned invert:8;

    // 0 - no invert , 1 - invert_right, 2 - invert_mid , 3 - invert right and mid
    // 4 - invert_left, 5 - left and right , 6 - invert left and mid
    // 7 - invert left, mid and right
};

struct BestSchema { // 6 byte
    unsigned mincode:32;
    unsigned complexity:8;
    unsigned out_node:7;
    unsigned out_invert:1;
};

struct BestSchemasDict {
    std::map<uint32_t, MIG> dict;
    std::string filename;

    explicit BestSchemasDict(std::string _filename);

    bool add (MIG &mig);

    void load();
    void save();
    void save_to_txt(const char * file_name);
    void out_statistic(const std::string & message);

    ~BestSchemasDict();
};


#endif //MIG_GENERATOR_BESTSCHEMASDICT_H
