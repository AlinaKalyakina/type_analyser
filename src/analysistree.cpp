#include "analysistree.h"
#include <iostream>
#include "typedetector.h"

Node::Node(const Lex& l, string type) : lex(l), exp_type(type) {};

Node::Node(Operation operation, string type, const_node_ptr ch1, const_node_ptr ch2) :
    exp_type(type), operation(operation)  {
    children[0] = ch1;
    children[1] = ch2;
}

pos_type Node::get_pos() const {
    if (operation == Operation::VAR) {
        return lex.pos;
    } else {
        return children[0]->get_pos();
    }
}

const std::shared_ptr<const Node>* Node::get_children() const{
    return children;
}

string Node::get_type() const{
    return exp_type;
}

string Node::get_look() const {
    switch (operation) {
    case(Operation::VAR) :
        return lex.look;
    case(Operation::PAREN) :
        return "(" + children[0]->get_look() + ")";
    case(Operation::PLUS) :
        return children[0]->get_look() + " + " + children[1]->get_look();
    case(Operation::MUL) :
        return children[0]->get_look() + " * " + children[1]->get_look();
    case(Operation::INDEXING) :
        return children[0]->get_look() + "[" + children[1]->get_look() + "]";
    }
}

string Node::write_subexpressions() const {
    string look;
    switch (operation) {
    case(Operation::VAR) :
        look = lex.look;
        break;
    case(Operation::PAREN) :
        look = "(" + children[0]->write_subexpressions() + ")";
        break;
    case(Operation::PLUS) :
         look = children[0]->write_subexpressions() + " + " + children[1]->write_subexpressions();
        break;
    case(Operation::MUL) :
         look = children[0]->write_subexpressions() + " * " + children[1]->write_subexpressions();
        break;
    case(Operation::INDEXING) :
         look = children[0]->write_subexpressions() + "[" + children[1]->write_subexpressions() + "]";
    }
    if (operation != Operation::PAREN) {
        std::cout << look << " : " << TypeDetector::say_type(exp_type)
              << std::endl;
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
