#include "synanalys.h"
#include "lexseq.h"
#include "analysistree.h"
#include "typedetector.h"
#include "errors.h"

SynAnalyser::SynAnalyser(LexSeq seq) {
    begin = seq.begin();
    end = seq.cend();
    lex = *begin;
}

Lex SynAnalyser::next_lex() {
    ++begin;
    lex = *begin;
    return lex;
}


node_ptr  SynAnalyser::S() {//PLUS
    node_ptr chld1 = A();
    while (lex == LexType::PLUS) {
        node_ptr chld2 = AnalysisTree::create_node(TypeDetector::get_type(lex), lex);//PLUS
        next_lex();
        node_ptr chld3 = A();
        chld1 = AnalysisTree::create_node(TypeDetector::op_check_and_res
                  (Operation::PLUS, chld1, chld2, chld3), chld1, chld2, chld3);
    }
    return chld1;
}


node_ptr  SynAnalyser::A() {//MUL
    node_ptr chld1 = B();
    while (lex == LexType::MUL) {
        node_ptr chld2 = AnalysisTree::create_node(TypeDetector::get_type(lex), lex);
        next_lex();
        node_ptr chld3 = B();
        chld1 = AnalysisTree::create_node(TypeDetector::op_check_and_res(Operation::MUL,
                                      chld1, chld2, chld3), chld1, chld2, chld3);
    }
    return chld1;
}

node_ptr  SynAnalyser::B() {//LPAREN
    if (lex == LexType::LPAREN) {
        node_ptr chld1 = AnalysisTree::create_node(TypeDetector::get_type(lex), lex);
        next_lex();
        node_ptr chld2 = S();
        if (lex == LexType::RPAREN) {
            chld1 = AnalysisTree::create_node(chld2->get_type(), chld1, chld2,
                               AnalysisTree::create_node(TypeDetector::get_type(lex), lex));
            next_lex();
            return chld1;
        } else {
            throw SynError(Syn_err::UNDEXP, lex, Lex(") ", lex.get_pos()));;//EXCEPTION
        }
    } else {
        return C();
    }
}

node_ptr  SynAnalyser::C() { // SQBLANCK OR NUM
    node_ptr chld1, chld2, chld3;
    switch (lex.get_type()) {
        case (LexType::NUM):
        case (LexType::ID):
            chld1 = AnalysisTree::create_node(TypeDetector::get_type(lex), lex);
            while (next_lex() == LexType::LSQBRACKET) {
                chld2 = AnalysisTree::create_node(TypeDetector::get_type(lex), lex);
                next_lex();
                chld3 = S();
                if (lex == LexType::RSQBEACKET) {
                    chld2 = AnalysisTree::create_node(TypeDetector::get_type(lex),chld2, chld3,
                                AnalysisTree::create_node(TypeDetector::get_type(lex), lex));
                } else {
                    throw SynError(Syn_err::UNDEXP, lex, Lex("] ", lex.get_pos()));
                }
                chld1 = AnalysisTree::create_node(TypeDetector::op_check_and_res (Operation::INDEX, chld1, chld2), chld1, chld2);
            }
            return chld1;
    default:
        throw SynError(Syn_err::ID_OR_NUM_EXPECT, lex);
    }
}


AnalysisTree SynAnalyser::analyse () {
    node_ptr peak = nullptr;
    if(begin != end) {
        peak = S();
    }
    if (begin != end) {
        throw SynError(Syn_err::EXTRA_LEX, lex); //EXCEPTION
    }
    return AnalysisTree(peak);
}
