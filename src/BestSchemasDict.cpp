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
        char * tmp = new char[6];
        tmp[0] = buffer[it++];
        tmp[1] = buffer[it++];
        tmp[2] = buffer[it++];
        tmp[3] = buffer[it++];
        tmp[4] = buffer[it++];
        tmp[5] = buffer[it++];

        BestSchema new_schema = *((BestSchema*)tmp);
        delete[] tmp;

        std::vector<SchemaNode> new_schema_nodes;
        new_schema_nodes.reserve(new_schema.complexity);
        //read
        const auto nodes_count = (uint8_t)new_schema.complexity;
        for (uint_fast8_t node_it = 0; node_it < nodes_count ; ++ node_it) {
            tmp = new char[4];
            tmp[0] = buffer[it++];
            tmp[1] = buffer[it++];
            tmp[2] = buffer[it++];
            tmp[3] = buffer[it++];
            new_schema_nodes.push_back(*((SchemaNode*)tmp));
            delete[] tmp;
        }

        //constructing mig
        MIG new_mig(new_schema,new_schema_nodes);

        dict[new_schema.mincode] = std::move(new_mig);
    }
}

void BestSchemasDict::save() {
    std::ofstream output( filename.c_str(), std::ios::binary );
    for (auto& pair : dict) {
        BestSchema best_schema;
        best_schema.mincode = pair.first;
        best_schema.complexity = pair.second.complexity;
        best_schema.out_invert = pair.second.out_invert;
        best_schema.out_node = pair.second.out;


        output.write((char*)&best_schema,6); // out + out_invert
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

void BestSchemasDict::out_statistic(const std::string & message) {
    std::ofstream out ((filename+"_stat.txt").c_str(), std::ios::app);
    if (!out.is_open()) out.open((filename+"_stat.txt").c_str(), std::ios::out);
    std::map<uint32_t, uint32_t > complexity_count;
    for (auto & pair : dict) {
        ++complexity_count[pair.second.complexity];
    }
    out<< "\n";
    out<< "-------------------------------------------\n";
    out<<message<<"\n"<<"MIG count "<<dict.size()<<"\n";
    for (auto &el : complexity_count) {
        out << "Complexity " << el.first << " : " << el.second <<"\n";
    }
    out<< "-------------------------------------------\n";
}




BestSchemasDict::~BestSchemasDict() {
    save();
}


