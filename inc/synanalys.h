#ifndef SYNANALYS_H
#define SYNANALYS_H
#include "lexseq.h"
#include "analysistree.h"

enum class Operation{NO_OP, PAREN, PLUS, MUL, SQBLANC, INDEXING};

const_node_ptr create_node(const Lex &lex);
const_node_ptr create_node(Operation,
       const_node_ptr ch1, const_node_ptr ch2, const_node_ptr ch3 = nullptr);

class SynAnalyser
{
    const_Lex_it begin, end;
    Lex lex;
    const_tree tree;
    const_node_ptr S();
    const_node_ptr A();
    const_node_ptr B();
    const_node_ptr C();
    Lex next_lex();
public:
    SynAnalyser(LexSeq);
    const_tree analyse();
};

#endif // SYNANALYS_H
