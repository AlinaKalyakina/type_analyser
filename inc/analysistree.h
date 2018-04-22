#ifndef ANALYSISTREE_H
#define ANALYSISTREE_H
#include <memory>
#include "lexseq.h"
#include <string>
#include <vector>

const string NO_TYPE = "";

using std::string;
using std::vector;

class Node {
    Lex lex = Lex();
public:
    string exp_type;
    std::shared_ptr<const Node> children[3] = {nullptr, nullptr, nullptr};
    Node(const Lex& lex, string type_of_expression = NO_TYPE);
    Node(string type_of_expression, std::shared_ptr<const Node> child1 = nullptr,
           std::shared_ptr<const Node> child2 = nullptr, std::shared_ptr<const Node> child3 = nullptr);
    pos_type get_pos() const;
    string get_look() const;
    string write_subexpressions() const;
};

typedef std::shared_ptr<const Node> const_node_ptr;
typedef std::shared_ptr<Node> node_ptr;
typedef const_node_ptr const_tree;

void write_subexpressions(const_node_ptr tree);
#endif // ANALYSISTREE_H
