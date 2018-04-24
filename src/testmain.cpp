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
    std::cout << "This node has semantic type: " << TypeDetector::say_type(node->exp_type) << std::endl;
}
