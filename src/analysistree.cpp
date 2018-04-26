#include "analysistree.h"
#include <iostream>
#include "typedetector.h"

Node::Node(const Lex& l, string type) : lex(l), exp_type(type) {};

Node::Node(string type, const_node_ptr ch1, const_node_ptr ch2,
           const_node_ptr ch3) : exp_type(type) {
    children[0] = ch1;
    children[1] = ch2;
    children[2] = ch3;
}

pos_type Node::get_pos() const {
    if (children[0] == nullptr) {
        return lex.pos;
    } else {
        return children[0]->get_pos();
    }
}

string Node::get_look() const {
    string res = "";
    if (children[0] == nullptr) {
        if (lex.type == LexType::MUL || lex.type == LexType::PLUS) {
            return res += " " + lex.look + " ";
        }
        return  res += lex.look;
    }
    for (int i = 0; i < 3; i++) {
        if (children[i] != nullptr) {
            res += children[i]->get_look();
        } else {
            break;
        }
    }
    return res;
}

string Node::write_subexpressions() const {
    if (children[0] == nullptr) {
        if (exp_type != "") {
            std::cout << lex.look << " : " << TypeDetector::say_type(exp_type) << std::endl;
        }
        if (lex.type == LexType::PLUS || lex.type == LexType::MUL) {
            return " " + lex.look + " ";
        }
        return lex.look;
    }
    string look;
    for (int i = 0; i < 3; i++) {
        if (children[i] != nullptr) {
            look += children[i]->write_subexpressions();
        } else {
            break;
        }
    }
    if (exp_type != NO_TYPE) {
        if (look[0] != '(' || look[look.size() - 1] != ')') {
            std::cout << look << " : " << TypeDetector::say_type(exp_type)
                  << std::endl;
        }
    }
    return look;
}

void write_subexpressions(const_node_ptr peak) {
    if (peak) {
        peak->write_subexpressions();
    } else {
        std::cout << "No any subexpressions";
    }
}
