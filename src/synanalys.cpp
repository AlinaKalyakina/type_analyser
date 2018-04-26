#include "synanalys.h"
#include "lexseq.h"
#include "analysistree.h"
#include "typedetector.h"
#include "errors.h"

const_node_ptr create_node(const Lex& lex) {
    return std::make_shared<Node>(lex, TypeDetector::type_determine(lex));
}

const_node_ptr create_node(Operation op, const_node_ptr ch1, const_node_ptr ch2, const_node_ptr ch3) {
    switch (op) {
    case(Operation::PLUS):
    case(Operation::MUL): {
        auto type = ch1->exp_type;
        if (type != "i" && type != "ci") {
            throw SemError(Sem_err::TYPE_MISMATCH, ch1, ch2, "i"); // ERROR
        }
        type = ch3->exp_type;
        if (type != "i" && type != "ci") {
            throw SemError(Sem_err::TYPE_MISMATCH, ch3, ch2, "i");
        }
        return std::make_shared<Node>("i", ch1, ch2, ch3);
    }
    case(Operation::INDEXING): {
        auto type = ch1->exp_type;
        if (type[0] != 'a') {
            throw SemError(Sem_err::INDEX_NOT_ARRAY, ch1, ch2);
        }
        type = ch2->children[1]->exp_type;
        if (type != "ci" && type != "i") {
            throw SemError(Sem_err::BAD_TYPE_OF_INDEX, ch1, ch2, "i");
        }
        type = ch1->exp_type;
        return std::make_shared<Node>(type.erase(0,1), ch1, ch2);
    }
    case(Operation::SQBLANC):
    case(Operation::NO_OP):
        return std::make_shared<Node>(NO_TYPE, ch1, ch2, ch3);
    case(Operation::PAREN):
        return std::make_shared<Node>(ch2->exp_type, ch1, ch2, ch3);
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
        const_node_ptr chld2 = create_node(lex);//PLUS
        next_lex();
        const_node_ptr chld3 = A();
        chld1 = create_node(Operation::PLUS, chld1, chld2, chld3);
    }
    return chld1;
}


const_node_ptr SynAnalyser::A() {//MUL
    const_node_ptr chld1 = B();
    while (lex.type == LexType::MUL) {
        const_node_ptr chld2 = create_node(lex);
        next_lex();
        const_node_ptr chld3 = B();
        chld1 = create_node(Operation::MUL, chld1, chld2, chld3);
    }
    return chld1;
}

const_node_ptr  SynAnalyser::B() { // ID+SQBRACK
    const_node_ptr chld1 = C();
    while (lex.type == LexType::LSQBRACKET) {
        const_node_ptr chld2 = create_node(lex);
        next_lex();
        const_node_ptr chld3 = S();
        if (lex.type == LexType::RSQBEACKET) {
            chld2 = create_node(Operation::SQBLANC, chld2, chld3, create_node(lex));
            next_lex();
        } else {
            throw SynError(Syn_err::UNDEXP, lex, Lex(LexType::RSQBEACKET));
        }
        chld1 = create_node(Operation::INDEXING, chld1, chld2);
    }
    return chld1;
}

const_node_ptr SynAnalyser::C() {//LPAREN
    if (lex.type == LexType::LPAREN) {
        const_node_ptr chld1 = create_node(lex);
        next_lex();
        const_node_ptr chld2 = S();
        if (lex.type == LexType::RPAREN) {
            chld1 = create_node(Operation::PAREN, chld1, chld2, create_node(lex));
            next_lex();
            return chld1;
        } else {
            throw SynError(Syn_err::UNDEXP, lex, Lex(LexType::RPAREN));
        }
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
