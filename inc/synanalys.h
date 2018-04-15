#ifndef SYNANALYS_H
#define SYNANALYS_H
#include "lexseq.h"
#include "analysistree.h"

enum class Operation{PLUS, MUL, INDEX};

class SynAnalyser
{
    const_Lex_it begin, end;
    const_lex_ptr lex;
    const_tree tree;
    const_node_ptr S();
    const_node_ptr A();
    const_node_ptr B();
    const_node_ptr C();
    const_lex_ptr next_lex();
public:
    SynAnalyser(LexSeq);
    const_tree analyse();
};

#endif // SYNANALYS_H
