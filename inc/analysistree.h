#ifndef ANALYSISTREE_H
#define ANALYSISTREE_H
#include <memory>


#include "lexseq.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Node {
    string type;
public:
    Node(string type);
    virtual const std::shared_ptr<const Node>* get_children() const = 0;
    string get_type() const;
    virtual pos_type get_pos() const = 0;
    virtual string get_look() const = 0;
    virtual string write_subexpressions() const = 0;
    virtual ~Node();
};

typedef std::shared_ptr<const Node> const_node_ptr;
typedef std::shared_ptr<Node> node_ptr;
typedef const_node_ptr const_tree;

class Inner_node: public Node {
    const_node_ptr children[3];
public:
    Inner_node(string t, const_node_ptr chleft, const_node_ptr chcen, const_node_ptr chrignht = nullptr);
    const const_node_ptr* get_children() const;
    pos_type get_pos() const;
    string get_look() const;
    string write_subexpressions() const;
};

class Leaf: public Node {
    const_lex_ptr lex;
public:
    Leaf(const_lex_ptr);
    const const_node_ptr* get_children() const;
    pos_type get_pos() const;
    string get_look() const;
    string write_subexpressions() const;
};

const_node_ptr create_node(const_lex_ptr lex);
const_node_ptr create_node(const string& type,
       const_node_ptr ch1, const_node_ptr ch2, const_node_ptr ch3 = nullptr);
void write_subexpressions(const_node_ptr tree);
#endif // ANALYSISTREE_H
