#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>

#include "z3++.h"

const unsigned VarToClassNum[] = { 0, 2, 4, 14, 222, 616126 };

const int VAR = 5;
const int FUNC = 1 << VAR;
const int NPN = VarToClassNum[VAR];
z3::context ctx;

using namespace std::chrono;
class Node {
    public:
        Node(int i): id(i), b(ctx), a1(ctx), a2(ctx), a3(ctx), ss(ctx), ps(ctx) 
        {
            initVars();
        }

        void initVars() 
        {
            for (int i = 0; i < FUNC; ++i) {
                std::stringstream name;  name << "b_" << id << "_" << i;
                b.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < FUNC; ++i) {
                std::stringstream name;  name << "a1_" << id << "_" << i;
                a1.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < FUNC; ++i) {
                std::stringstream name;  name << "a2_" << id << "_" << i;
                a2.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < FUNC; ++i) {
                std::stringstream name;  name << "a3_" << id << "_" << i;
                a3.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < 3; ++i) {
                std::stringstream name;  name << "s_" << i << "_" << id;
                ss.push_back(ctx.int_const(name.str().c_str()));
            }
            for (int i = 0; i < 3; ++i) {
                std::stringstream name;  name << "p_" << i << "_" << id;
                ps.push_back(ctx.bool_const(name.str().c_str()));
            }
        }

        // Majority functionality
        void addMajorityFormula(z3::solver& svr)
        {
            for (int i = 0; i < FUNC; ++i) {
                z3::expr bb = b[i], aa1 = a1[i], aa2 = a2[i], aa3 = a3[i];
                z3::expr f = ( bb == ((aa1&&aa2)||(aa2&&aa3)||(aa1&&aa3)) );
                svr.add(f);
            }
        }

        // Input connection: 1.constant  2.variables
        void addConnectionFormula(z3::solver& svr)
        {
            z3::expr_vector* aa;
            for (int i = 0; i < 3; ++i) {
                z3::expr ssi = ss[i];
                svr.add( (ssi < VAR + id) );  
                svr.add( (ssi >= 0) );

                if (i == 0) aa = &a1;
                else if (i == 1) aa = &a2;
                else aa = &a3;
                
                // constant
                for (int j = 0; j < FUNC; ++j) {
                    z3::expr impl = implies( (ssi == 0), ((*aa)[j] == ps[i]) );
                    svr.add(impl);
                }
                // variables
                for (int j = 1; j <= VAR; ++j) {
                    for (int k = 0; k < FUNC; ++k) {
                        bool value = (k >> (j-1)) & 01;
                        z3::expr psi = ps[i];
                        z3::expr impl = implies( (ssi == j), ((*aa)[k] == ((value&&!psi)||(!value&&psi))) );
                        svr.add(impl);
                    }
                }
            }
        }

        // Symmetry breaking
        void addSymmetryFormula(z3::solver& svr)
        {
            svr.add( (ss[0] < ss[1]) );
            svr.add( (ss[1] < ss[2]) );
        }
        void addComplementConstraint(z3::solver& svr) {
            svr.add(!ps[0] || !ps[1]);
            svr.add(!ps[0] || !ps[2]);
            svr.add(!ps[1] || !ps[2]);
        }



        int                 id;
        z3::expr_vector      b;
        z3::expr_vector     a1;
        z3::expr_vector     a2;
        z3::expr_vector     a3;
        z3::expr_vector     ss;
        z3::expr_vector     ps;
};

void add_structural_hashing (z3::solver& svr, Node& n1, Node& n2) {
    svr.add(((n1.ss[0]!=n2.ss[0])||(n1.ss[1]!=n2.ss[1])||(n1.ss[2]!=n2.ss[2])
           ||(n1.ps[0]!=n2.ps[0])||(n1.ps[1]!=n2.ps[1])||(n1.ps[2]!=n2.ps[2])));
}

void add_associativity(z3::solver& svr, Node& n1, Node& n2) {
    for (int a = 0 ; a <=2 ; ++a)
        for (int b = 0 ; b <=2 ; ++b) {
            if (a == b) continue;
            for (int c = 0; c <= 2; ++c) {
                z3::expr impl = implies( ((n2.ss[b]==n1.id+VAR)&&(n2.ss[a]==n1.ss[c])&&(n2.ps[a]==n1.ps[c])),
                        ((n1.ss[3-(c==2?2:1)])<=n2.ss[3-a-b]));
                svr.add(impl);
            }
        }
}


int main(int argc, char** argv) {

    unsigned uTruth = std::stoll(argv[1]);
    unsigned nNode = std::stoll(argv[2]);
    unsigned c = uTruth;

    unsigned timeout = 0;
    if (argc>3) timeout = std::stoll(argv[3]);


    std::cout << "Generating subgraph for class " << c << ",";
    std::cout << " truth = ";  printf("0x%08X,", uTruth);
    std::cout << " complexity = " << nNode << std::endl;

    std::vector<Node> nodes;
    for (int i = 1; i <= nNode; ++i) nodes.push_back(Node(i));

    z3::solver svr(ctx);

    svr.set(":timeout",timeout);

    for (int i = 0; i < nNode; ++i) {
        nodes[i].addMajorityFormula(svr);
        nodes[i].addConnectionFormula(svr);
        nodes[i].addSymmetryFormula(svr);
        nodes[i].addComplementConstraint(svr);
    }


    // Input connection: majority node
    for (int i = 0; i < nNode; ++i) {
        Node* maj = &nodes[i];
        for (int j = 1; j < maj->id; ++j) {
            Node* in = &nodes[j-1]; // topologically lower nodes
            z3::expr_vector* aa;
            for (int k = 0; k < 3; ++k) {
                if (k == 0) aa = &maj->a1;
                else if (k == 1) aa = &maj->a2;
                else aa = &maj->a3;

                z3::expr ssk = maj->ss[k];
                z3::expr psk = maj->ps[k];
                for (int l = 0; l < FUNC; ++l) {
                    z3::expr bl = in->b[l];
                    z3::expr impl = implies( (ssk == VAR + j), ((*aa)[l] == ((bl&&!psk)||(!bl&&psk))) );
                    svr.add(impl);
                }
            }
        }
    }

    // Function Semantics
    z3::expr root_p = ctx.bool_const("root_p");
    Node* root_node = &nodes.back();
    for (int i = 0; i < FUNC; ++i) {
        bool value = (uTruth >> i) & 01;
        z3::expr bi = root_node->b[i];
        z3::expr f = (bi == ((root_p && !value) || (!root_p && value)));
        svr.add(f);
    }

    
    for (int i = 0; i < nNode-1; ++i) {
        for (int j = i+1; j < nNode; ++j) {
            add_structural_hashing(svr, nodes[i], nodes[j]);
            add_associativity(svr,nodes[i],nodes[j]);
        }
    }


    // Subgraphs generation

    auto start = high_resolution_clock::now();
    if (svr.check()) {
        z3::model mod = svr.get_model();
        std::system("mkdir -p nets");

        std::ofstream out(("./nets/"+std::to_string(c)+".txt").c_str(), std::ios::out);

        out << c << "\n";
        out << "MIG" << "\n";
        out << nodes.size() << "\n";
        out << nodes.size()+VAR << " " <<(mod.eval(root_p).to_string()=="true"?1:0) << " " << "\n";
        for (int i = 0; i < nodes.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                out << mod.eval(nodes[i].ss[j]) << " " << (mod.eval(nodes[i].ps[j]).to_string()=="true"?1:0) << " ";
            }
        }
        out << std::endl;
        out.close();

    }
    else {
        std::cout<<"No model found \n";
    }
    auto stop = high_resolution_clock::now();
    std::cout<<"\nTime: " << (duration_cast<seconds>(stop - start)).count()<<"\n";

    return 0;
}
