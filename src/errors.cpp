#include "errors.h"
#include <iostream>
#include "analysistree.h"
#include "typedetector.h"
#include "synanalys.h"

Error::~Error() {}

string say_pos(pos_type pos) {
    return "line " + std::to_string(pos.first) + ", position " + std::to_string(pos.second);
}

LexError::LexError(int x, const pos_type &pos) : c(x), position(pos) {}

string LexError::what() const {
    return "Bad symbol " + std::string(1, c) + " at " + say_pos(position);
}

//SynError
SynError::SynError(Syn_err er, const Lex& found, Lex expected): met(found), exp(expected), code(er) {}

string SynError::what() const {
    string res = "";
    switch (code) {
    case (Syn_err::UNDEXP):
        res += "Lexem " + exp.look + " was expected at " + say_pos(met.pos) + ", but ";
        if (met.type == LexType::EMPTY) {
            res += "nothing";
        } else {
            res += "lexem " + met.look;
        }
        return res += " was found\n";
    case(Syn_err::EXTRA_LEX):
        return "Extra lexem " + met.look + " was found at " + say_pos(met.pos);
     case(Syn_err::ID_OR_NUM_EXPECT):
        res += "Identificator or number was expected at " + say_pos(met.pos);
        if (met.type == LexType::EMPTY) {
            return res + ", but nothing was found";
        } else {
            return res + ", but lexem " + met.look + " was found";
        }
    }
}

SemError::SemError(Sem_err er, const_node_ptr x1, const_node_ptr oper,  string t):
    code(er), expression(x1), operation(oper), required_type(t) {}

SemError::SemError(Sem_err err, const Lex& l) : code(err), lex(l) {}

string SemError::what() const {
    const_node_ptr res_node;
    switch (code) {
    case (Sem_err::TYPE_MISMATCH):
        return "Operator " + operation->get_look() + " (" + say_pos(operation->get_pos()) +
                ") requires type " + TypeDetector::say_type(required_type) + ", but " +
                expression->get_look() + " (" + say_pos(expression->get_pos()) + ") has type " +
                TypeDetector::say_type(expression->exp_type);
    case (Sem_err::BAD_TYPE_OF_INDEX):
        res_node = create_node(Operation::NO_OP, expression, operation);
        return "Error in expression " + res_node->get_look()+ " (" + say_pos(res_node->get_pos()) +
                "). Index must have type " + TypeDetector::say_type(required_type) +
                ", but " + operation->children[1]->get_look() + " has type "
                + TypeDetector::say_type(operation->children[1]->exp_type);
    case(Sem_err::INDEX_NOT_ARRAY):
        res_node = create_node(Operation::NO_OP, expression, operation);
        return  "Error in expression " + res_node->get_look() + " (" + say_pos(res_node->get_pos())
                + "). Only arrays can be indexed, but " + expression->get_look() + " has type " +
                TypeDetector::say_type(expression->exp_type);
    case(Sem_err::UNTYPED_LEX):
        return "Untyped lexem " + lex.look + " was found at " + say_pos(lex.pos);
    }
}
