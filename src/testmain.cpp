#include <iostream>
#include "lexseq.h"
#include <vector>
#include "errors.h"
#include "synanalys.h"
#include "analysistree.h"
#include "typedetector.h"
#include <memory>
#include <string>

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
    try {
        LexSeq x;
        auto it1 = x.cend();
        auto it2 = x.cend();
        ++it1;
        if (it1 == it2) {
            std::cout << "After making ++ for end iterator it didn't changed" << std::endl;
        } else {
            std::cout << "ERROR WITH +++!!!" << std::endl;
        }
    } 
    catch (...) { std::cout << "ERROR!" << std::endl; }
    LexType type[3] = {LexType::PLUS, LexType::NUM, LexType::ID};
    lex_ptr lex;
    for (auto t : type) {
        lex = create_lex(t, std::make_pair<int, int> (0, 0));
        if (*lex != LexType::EMPTY) {
            std::cout << "Lex (has look '" << lex->get_look() << "') is not empty" << std::endl;
        } else {
            std::cout << "ERROR" << std::endl;
        }
        lex->clear();
        if (lex->get_type() == LexType::EMPTY) {
            std::cout << "Lex (has look '" << lex->get_look() << "') became clear" << std::endl;
        } else {
            std::cout << "ERROR" << std::endl;
        }
    }
    lex = std::make_shared<Delim_lex>(LexType::ID, std::make_pair<int, int>(0, 0));
    std::cout << "Lex look: " << lex->get_look() << std::endl;
    lex = std::make_shared<Delim_lex>(LexType::MUL, std::make_pair<int, int>(0, 0));
    auto node = create_node(lex);
    if (!node->get_children()) {
        std::cout << "Node, consisting of one lex " << node->get_look() << ", doesn't have any children" << std::endl;
    } else {
        std::cout << "ERROR" << std::endl;
    }
    std::cout << "This node has semantic type: " << TypeDetector::say_type(node->get_type()) << std::endl;
}
