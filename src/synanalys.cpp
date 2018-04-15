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

const_lex_ptr SynAnalyser::next_lex() {
    ++begin;
    lex = *begin;
    return lex;
}


const_node_ptr SynAnalyser::S() {//PLUS
    const_node_ptr chld1 = A();
    while (*lex == LexType::PLUS) {
        const_node_ptr chld2 = create_node(lex);//PLUS
        next_lex();
        const_node_ptr chld3 = A();
        chld1 = create_node(TypeDetector::op_check_and_res
                  (Operation::PLUS, chld1, chld2, chld3), chld1, chld2, chld3);
    }
    return chld1;
}


const_node_ptr SynAnalyser::A() {//MUL
    const_node_ptr chld1 = B();
    while (*lex == LexType::MUL) {
        const_node_ptr chld2 = create_node(lex);
        next_lex();
        const_node_ptr chld3 = B();
        chld1 = create_node(TypeDetector::op_check_and_res(Operation::MUL,
                                      chld1, chld2, chld3), chld1, chld2, chld3);
    }
    return chld1;
}

const_node_ptr SynAnalyser::B() {//LPAREN
    if (*lex == LexType::LPAREN) {
        const_node_ptr chld1 = create_node(lex);
        next_lex();
        const_node_ptr chld2 = S();
        if (*lex == LexType::RPAREN) {
            chld1 = create_node(chld2->get_type(), chld1, chld2, create_node(lex));
            next_lex();
            return chld1;
        } else {
            throw SynError(Syn_err::UNDEXP, lex, create_lex(LexType::RPAREN));//EXCEPTION
        }
    } else {
        return C();
    }
}

const_node_ptr  SynAnalyser::C() { // SQBLANCK OR ID
    const_node_ptr chld1, chld2, chld3;
    switch (lex->get_type()) {
        case (LexType::NUM):
        case (LexType::ID):
            chld1 = create_node(lex);
            while (*next_lex() == LexType::LSQBRACKET) {
                chld2 = create_node(lex);
                next_lex();
                chld3 = S();
                if (*lex == LexType::RSQBEACKET) {
                    chld2 = create_node(NO_TYPE,chld2, chld3, create_node(lex));
                } else {
                    throw SynError(Syn_err::UNDEXP, lex, create_lex(LexType::RSQBEACKET));
                }
                chld1 = create_node(TypeDetector::op_check_and_res (Operation::INDEX, chld1, chld2), chld1, chld2);
            }
            return chld1;
    default:
        throw SynError(Syn_err::ID_OR_NUM_EXPECT, lex);
    }
}


const_tree SynAnalyser::analyse() {
    const_node_ptr peak = nullptr;
    if(begin != end) {
        peak = S();
    }
    if (begin != end) {
        throw SynError(Syn_err::EXTRA_LEX, lex); //EXCEPTION
    }
    return peak;
}
