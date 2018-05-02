#include <iostream>
#include "lexseq.h"
#include <vector>
#include "errors.h"
#include "synanalys.h"
#include "analysistree.h"
#include "typedetector.h"
#include <memory>
#include <string>
#include <utility>

int main()
{
    try {
        LexSeq x;
        auto it1 = x.cbegin(); 
        auto it2 = x.cbegin();
        std::cout << "ONE MORE BEGIN WAS CREATED" << std::endl;
    }
    catch(const char* x) { 
        std::cout << x << std::endl;
    } 
    Lex lex(LexType::MUL, std::make_pair(0,0));
    auto node = create_node(lex);
    std::cout << "This node has semantic type: " << TypeDetector::say_type(node->get_type()) << std::endl;
    auto lex1 = Lex(LexType::ID, pos_type(1, 1), "i");
    auto lex2 = Lex(LexType::ID, pos_type(1, 3), "i");
    auto node_plus = Node(Operation::PLUS, "i", std::make_shared<const Node>(lex1, "i"), std::make_shared<const Node>(lex1, "i"));
    auto node_mul = Node(Operation::MUL, "i", std::make_shared<const Node>(lex1, "i"), std::make_shared<const Node>(lex1, "i"));
    std::cout << node_plus.get_look() << std::endl << node_mul.get_look();
}
