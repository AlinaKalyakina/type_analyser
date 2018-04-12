#ifndef SYNANALYS_H
#define SYNANALYS_H
#include "lexseq.h"
#include "analysistree.h"

enum class Operation{PLUS, MUL, INDEX};

class SynAnalyser
{
    LexIt begin, end;
    Lex lex;
    AnalysisTree tree;
    node_ptr S();
    node_ptr A();
    node_ptr B();
    node_ptr C();
    Lex next_lex();
public:
    SynAnalyser(LexSeq);
    AnalysisTree analyse();
};

#endif // SYNANALYS_H
