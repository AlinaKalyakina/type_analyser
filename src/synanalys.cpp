#include "synanalys.h"
#include "lexseq.h"
#include "analysistree.h"
#include "typedetector.h"
#include "errors.h"

const_node_ptr create_node(const Lex& lex) {
    return std::make_shared<Node>(lex, TypeDetector::type_determine(lex));
}

const_node_ptr create_node(Operation op, const_node_ptr ch1, const_node_ptr ch2, pos_type op_pos) {
    switch (op) {
    case(Operation::PLUS):
    case(Operation::MUL): {
        auto type = ch1->get_type();
        if (type != "i" && type != "ci") {
            throw SemError(Sem_err::TYPE_MISMATCH, op, ch1, "i", op_pos); // ERROR
        }
        type = ch2->get_type();
        if (type != "i" && type != "ci") {
            throw SemError(Sem_err::TYPE_MISMATCH, op, ch2, "i", op_pos);
        }
        return std::make_shared<Node>(op, "i", ch1, ch2);
    }
    case(Operation::INDEXING): {
        auto type = ch1->get_type();
        if (type[0] != 'a') {
            throw SemError(Sem_err::INDEX_NOT_ARRAY, op, std::make_shared<Node>(op, NO_TYPE, ch1, ch2));
        }
        type = ch2->get_type();
        if (type != "ci" && type != "i") {
            throw SemError(Sem_err::BAD_TYPE_OF_INDEX, op, std::make_shared<Node>(op, NO_TYPE, ch1, ch2), "i");
        }
        type = ch1->get_type();
        return std::make_shared<Node>(op,type.erase(0,1), ch1, ch2);
    }
    case(Operation::PAREN):
        return std::make_shared<Node>(op, ch1->get_type(), ch1);
    case(Operation::VAR) :
        return nullptr;
    }
}


SynAnalyser::SynAnalyser(LexSeq seq) : begin(seq.cbegin()) {
//    begin = seq.cbegin();
    end = seq.cend();
    lex = *begin;
}

Lex SynAnalyser::next_lex() {
    ++begin;
    lex = *begin;
    return lex;
}


const_node_ptr SynAnalyser::S() {//PLUS
    const_node_ptr chld1 = A();
    while (lex.type == LexType::PLUS) {
        auto pos = lex.pos;
        next_lex();
        const_node_ptr chld2 = A();
        chld1 = create_node(Operation::PLUS, chld1, chld2, pos);
    }
    return chld1;
}


const_node_ptr SynAnalyser::A() {//MUL
    const_node_ptr chld1 = B();
    while (lex.type == LexType::MUL) {
        auto pos = lex.pos;
        next_lex();
        const_node_ptr chld2 = B();
        chld1 = create_node(Operation::MUL, chld1, chld2, pos);
    }
    return chld1;
}

const_node_ptr  SynAnalyser::B() { // ID+SQBRACK
    const_node_ptr chld1 = C();
    while (lex.type == LexType::LSQBRACKET) {
        next_lex();
        const_node_ptr chld2 = S();
        if (lex.type != LexType::RSQBEACKET) {
            throw SynError(Syn_err::UNDEXP, lex, Lex(LexType::RSQBEACKET));
        }
        next_lex();
        chld1 = create_node(Operation::INDEXING, chld1, chld2);
    }
    return chld1;
}

const_node_ptr SynAnalyser::C() {//LPAREN
    if (lex.type == LexType::LPAREN) {
        next_lex();
        const_node_ptr chld1 = S();
        if (lex.type != LexType::RPAREN) {
            throw SynError(Syn_err::UNDEXP, lex, Lex(LexType::RPAREN));
        }
        next_lex();
        return create_node(Operation::PAREN, chld1);
    } else {
        if (lex.type == LexType::ID || lex.type == LexType::NUM) {
            auto ch1 =  create_node(lex);
            next_lex();
            return ch1;
        } else {
            throw SynError(Syn_err::ID_OR_NUM_EXPECT, lex);
        }
    }
}

const_tree SynAnalyser::analyse() {
    const_node_ptr peak = nullptr;
    if(begin != end) {
        peak = S();
    }
    if (!(begin == end)) {
        throw SynError(Syn_err::EXTRA_LEX, lex);
    }
    return peak;
}
