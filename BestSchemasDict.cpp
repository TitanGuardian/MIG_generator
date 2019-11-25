//
// Created by davo1 on 25.11.2019.
//

#include "BestSchemasDict.h"

#include <fstream>

BestSchemasDict::BestSchemasDict(std::string _filename)
    : filename(std::move(_filename))
{
    load();
}



void BestSchemasDict::load() {
    std::ifstream input( filename.c_str(), std::ios::binary );
    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    uint_fast64_t it = 0 ;
    uint_fast64_t end_of_file = buffer.size();
    while (it < end_of_file) {

        // read Schema
        BestSchema new_schema;
        new_schema.mincode = (uint32_t)buffer[it]        |
                             (uint32_t)buffer[it+1] << 8 |
                             (uint32_t)buffer[it+2] << 16|
                             (uint32_t)buffer[it+3] << 24;
        it = it + 4;
        new_schema.complexity = buffer[it++];
        new_schema.out_node = buffer[it] >> 1;
        new_schema.out_invert = buffer[it] & 1;
        ++it;
        std::vector<SchemaNode> new_schema_nodes;
        new_schema_nodes.reserve(new_schema.complexity);
        //read
        const auto nodes_count = (uint8_t)new_schema.complexity;
        for (uint_fast8_t node_it = 0; node_it < nodes_count ; ++ node_it) {
            SchemaNode new_node;
            new_node.node_left = buffer[it++];
            new_node.node_mid = buffer[it++];
            new_node.node_right = buffer[it++];
            new_node.invert = buffer[it++];
            new_schema_nodes.push_back(new_node);
        }

        //constructing mig
        MIG new_mig(new_schema,new_schema_nodes);
//        new_mig.compute();
        dict[new_schema.mincode] = std::move(new_mig);
    }
}

void BestSchemasDict::save() {
    std::ofstream output( filename.c_str(), std::ios::binary );
    for (auto& pair : dict) {
        output.write((char *)&pair.first,4); // mincode
        output.write((char *)&pair.second.complexity,1); // complexity
        unsigned char tmp = (uint8_t)pair.second.out << 1 | (uint8_t)pair.second.out_invert;
        output.write((char*)&tmp,1); // out + out_invert
        for (size_t it = 6; it < pair.second.nodes.size(); ++it) {
            SchemaNode schema_node;
            schema_node.node_left = pair.second.nodes[it].left;
            schema_node.node_mid = pair.second.nodes[it].mid;
            schema_node.node_right = pair.second.nodes[it].right;
            schema_node.invert = 4*pair.second.nodes[it].left_inv
                    + 2*pair.second.nodes[it].mid_inv
                    + 1*pair.second.nodes[it].right_inv;
            output.write((char*)&schema_node, 4);
        }
    }
}

bool BestSchemasDict::add(MIG &mig) {
    bool better = false;
    if (dict.count(mig.vector.to_ulong())) {

        better = dict[mig.vector.to_ulong()].complexity > mig.complexity;
        if (better) dict[mig.vector.to_ulong()] = std::move(mig);
    }
    else {
        better = true;
        dict[mig.vector.to_ulong()] = std::move(mig);
    }
    return better;
}

BestSchemasDict::~BestSchemasDict() {
    save();
}


