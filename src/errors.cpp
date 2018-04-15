#include "errors.h"
#include <iostream>
#include "analysistree.h"
#include "typedetector.h"

Error::~Error() {}

string say_pos(pos_type pos) {
    return "line " + std::to_string(pos.first) + ", position " + std::to_string(pos.second);
}
//LexError
std::map<Lex_err, string> LexError::messages {{Lex_err::BADCHR, "Bad char: "}, {Lex_err::BADLEX, "Bad lexem: "}};

LexError::LexError(Lex_err er, const_lex_ptr x): lex(x), code(er) {}

LexError::LexError(Lex_err er, int x, const pos_type &pos): code(er) {
    lex = std::make_shared<Id_lex>(x, pos);
}

string LexError::what() const {
    return messages.find(code)->second + lex->get_look() + " at " + say_pos(lex->get_pos());
}

//SynError
SynError::SynError(Syn_err er, const_lex_ptr found, const_lex_ptr expected): met(found), exp(expected), code(er) {}

string SynError::what() const {
    string res = "";
    switch (code) {
    case (Syn_err::UNDEXP):
        res += "Lexem " + exp->get_look() + " was expected at " + say_pos(met->get_pos()) + ", but ";
        if (met->empty()) {
            res += "nothing";
        } else {
            res += "lexem " + met->get_look();
        }
        return res += " was found\n";
    case(Syn_err::EXTRA_LEX):
        return "Extra lexem " + met->get_look() + " was found at " + say_pos(met->get_pos());
     case(Syn_err::ID_OR_NUM_EXPECT):
        res += "Identificator or number was expected at " + say_pos(met->get_pos());
        if (met->empty()) {
            return res + ", but nothing was found";
        } else {
            return res + ", but lexem " + met->get_look() + " was found";
        }
    }
}

SemError::SemError(Sem_err er,  string t, const_node_ptr x1, const_node_ptr oper):
    code(er), exp(x1), op(oper), type(t) {}

string SemError::what() const {
    const_node_ptr res_node;
    switch (code) {
    case (Sem_err::TYPE_MISMATCH):
        return "Operator " + op->get_look() + " (" + say_pos(op->get_pos()) + ") requires type " +
                TypeDetector::say_type(type) + ", but " + exp->get_look() + " (" + say_pos(exp->get_pos()) + ") has type " +
                TypeDetector::say_type(exp->get_type());
    case (Sem_err::BAD_TYPE_OF_INDEX):
        res_node = create_node(NO_TYPE, exp, op);
        return "Error in expression " + res_node->get_look()+ " (" + say_pos(res_node->get_pos()) +
                "). Index must have type " + TypeDetector::say_type(type) +
                ",but " + op->get_children()[1]->get_look() + " has type "
                + TypeDetector::say_type(op->get_children()[1]->get_type());
    case(Sem_err::INDEX_NOT_ARRAY):
        res_node = create_node(NO_TYPE, exp, op);
        return  "Error in expression " + res_node->get_look() + " (" + say_pos(res_node->get_pos()) +
                "). Only arrays can be indexed, but " +
                exp->get_look() + " has type " + TypeDetector::say_type(exp->get_type());
    }
}
