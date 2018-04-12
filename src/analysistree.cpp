#include "analysistree.h"
#include <iostream>
#include "typedetector.h"

Node::Node(node_ptr ch1, node_ptr ch2, node_ptr ch3, Lex l, string t) {
    children[0] = ch1;
    children[1] = ch2;
    children[2] = ch3;
    lex = l;
    type = t;
}

Node::Node(const Node& x) {
    children[0] = std::make_shared<Node>(Node(*x.children));
}

const node_ptr *Node::get_children() const{
    return children;
}

string Node::get_type() const{
    return type;
}

string Node::get_look() const {
    auto type = lex.get_type();
    if (type == LexType::PLUS || type == LexType::MUL) {
        return " " + lex.get_look() + " ";
    }
    return lex.get_look();
}

pos_type Node::get_pos() const {
    if (children[0] != nullptr) {
        return children[0]->get_pos();
    } else {
        return lex.get_pos();
    }
}

node_ptr AnalysisTree::create_node(string type, Lex l) {
    return std::make_shared<Node>(nullptr, nullptr, nullptr, l, type);
}

node_ptr AnalysisTree::create_node(string s, node_ptr ch1, node_ptr ch2, node_ptr ch3) {
    return std::make_shared<Node>(ch1, ch2, ch3, EMPTY_LEX,s);
}

string AnalysisTree::write_expression() const {
    string res = "";
    ::write_expression(peak, res);
    return res;
}

void AnalysisTree::write_subexpressions() const {
    if (peak == nullptr) {
        std::cout << "Empty expression" << std::endl;
    }
    ::write_subexpressions(peak);
}

void write_subexpressions(node_ptr peak)  {
    if (peak != nullptr) {
        auto chld = peak->get_children();
        write_subexpressions(chld[0]);
        write_subexpressions(chld[1]);
        write_subexpressions(chld[2]);
        auto type = TypeDetector::say_type(peak->get_type());
        if (type != "") {
            string expression;
            write_expression(peak, expression);
            std::cout << expression << " : " << type << std::endl;
        }
    }
}

void write_expression(node_ptr peak, string& str) {
    if (peak != nullptr) {
        auto chld = peak->get_children();
        if (chld[0] == nullptr) {
            str += peak->get_look();
        } else {
            write_expression(chld[0], str);
            write_expression(chld[1], str);
            write_expression(chld[2], str);
        }
    }
}
