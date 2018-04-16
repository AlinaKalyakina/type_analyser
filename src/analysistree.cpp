#include "analysistree.h"
#include <iostream>
#include "typedetector.h"

//NODE
Node::Node(string t) : type(t) {}

string Node::get_type() const{
    return type;
}

Node::~Node() {}

//Inner_node
Inner_node::Inner_node(string type, const_node_ptr ch1,
                       const_node_ptr ch2, const_node_ptr ch3) : Node(type){
    children[0] = ch1;
    children[1] = ch2;
    children[2] = ch3;
}

const const_node_ptr* Inner_node::get_children() const {
    return children;
}

pos_type Inner_node::get_pos() const {
    return children[0]->get_pos();
}

string Inner_node::get_look() const {
    string res = "";
    for (int i = 0; i < 3; i++) {
        if (children[i] != nullptr) {
            res += children[i]->get_look();
        } else {
            break;
        }
    }
    return res;
}

string Inner_node::write_subexpressions() const {
    string exp = "";
    for (int i = 0; i < 3; i++) {
        if (children[i] != nullptr) {
            exp += children[i]->write_subexpressions();
        } else {
            break;
        }
    }
    if (this->get_type() != NO_TYPE) {
        std::cout << exp << " : " <<
                    TypeDetector::say_type(this->get_type()) << std::endl;
    }
    return exp;
}

//Leaf
Leaf::Leaf(const_lex_ptr l) : Node(TypeDetector::type_of_lex(l)) {
    lex = l;
}

const const_node_ptr* Leaf::get_children() const{
    return nullptr;
}

pos_type Leaf::get_pos() const {
    return lex->get_pos();
}

string Leaf::get_look() const {
    auto type = lex->get_type();
    if (type == LexType::PLUS || type == LexType::MUL) {
        return " " + lex->get_look() + " ";
    }
    return lex->get_look();
}

string Leaf::write_subexpressions() const {
    if (*lex == LexType::ID || *lex == LexType::NUM) {
        std::cout << this->get_look() << " : " << TypeDetector::say_type(this->get_type()) << std::endl;
    }
    return this->get_look();
}

const_node_ptr create_node(const_lex_ptr lex) {
    return std::make_shared<Leaf>(lex);
}

const_node_ptr create_node(const string& type, const_node_ptr ch1, const_node_ptr ch2, const_node_ptr ch3) {
    return std::make_shared<Inner_node>(type, ch1, ch2, ch3);
}

void write_subexpressions(const_node_ptr peak) {
    if (peak) {
        peak->write_subexpressions();
    } else {
        std::cout << "No any subexpressions";
    }
}
