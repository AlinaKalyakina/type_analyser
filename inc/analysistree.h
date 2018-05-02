#ifndef ANALYSISTREE_H
#define ANALYSISTREE_H
#include <memory>
#include "lexseq.h"
#include <string>
#include <vector>

enum class Operation{VAR, PAREN, PLUS, MUL, INDEXING};
const string NO_TYPE = "";

using std::string;
using std::vector;

class Node {
    Lex lex = Lex();
    std::shared_ptr<const Node> children[2] = {nullptr, nullptr};
    string exp_type;
    Operation operation = Operation::VAR;
public:
    Node(const Lex& lex, string type_of_expression = NO_TYPE);
    Node(Operation operation, string type_of_expression, std::shared_ptr<const Node> child1,
           std::shared_ptr<const Node> child2 = nullptr);
    pos_type get_pos() const;
    string get_look() const;
    string write_subexpressions() const;
    string get_type() const;
    const std::shared_ptr<const Node> *get_children() const;
};

typedef std::shared_ptr<const Node> const_node_ptr;
typedef std::shared_ptr<Node> node_ptr;
typedef const_node_ptr const_tree;

void write_subexpressions(const_node_ptr tree);
#endif // ANALYSISTREE_H
