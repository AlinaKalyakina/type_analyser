#ifndef ANALYSISTREE_H
#define ANALYSISTREE_H
#include <memory>

enum class Expresion_output { SUBEXPR,  };

#include "lexseq.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Node {
    std::shared_ptr<Node> children[3];
    Lex lex;
    string type;
public:
    Node(std::shared_ptr<Node> ch1 = nullptr, std::shared_ptr<Node> ch2 = nullptr, std::shared_ptr<Node> ch3 = nullptr, Lex l = EMPTY_LEX,string t = "");
    Node(const Node&);
    const std::shared_ptr<Node>* get_children() const;
    string get_type() const;
    string get_look() const;
    pos_type get_pos() const;
};

typedef std::shared_ptr<Node> node_ptr;

void write_expression(node_ptr, string&);

void write_subexpressions(node_ptr);

class AnalysisTree
{
    node_ptr peak;
public:
    AnalysisTree(node_ptr p = nullptr) {
        peak = p;
    }
    static node_ptr create_node(string, Lex);
    static node_ptr create_node(string s = "",
                             node_ptr ch1 = nullptr, node_ptr ch2 = nullptr, node_ptr ch3 = nullptr);
    string write_expression() const;
    void write_subexpressions() const;
};

#endif // ANALYSISTREE_H
