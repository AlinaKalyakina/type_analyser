#include "errors.h"
#include <iostream>
#include "analysistree.h"
#include "typedetector.h"

string say_pos(pos_type pos) {
    return "line " + std::to_string(pos.first) + ", position " + std::to_string(pos.second);
}

LexError::LexError(Lex_err er, const Lex &x): lex(x), code(er) {}
LexError::LexError(Lex_err er, int x, const pos_type &pos): code(er) {
    lex.set_look(x);
    lex.set_pos(pos);
}

std::map<Lex_err, string> LexError::messages {{Lex_err::BADCHR, "Bad char: "}, {Lex_err::BADLEX, "Bad lexem: "}};

string LexError::what() const {
    return messages.find(code)->second + lex.get_look() + " at " + say_pos(lex.get_pos());
}

SynError::SynError(Syn_err er, const Lex &found, const Lex &expected): met(found), exp(expected), code(er) {}

string SynError::what() const {
    string res = "";
    switch (code) {
    case (Syn_err::UNDEXP):
        res += "Lexem " + exp.get_look() + "was expected at " + say_pos(exp.get_pos()) + ", but ";
        if (met.empty()) {
            return res += "not found";
        } else {
            return res += "lexem " + met.get_look() + " was found\n";
        }
    case(Syn_err::EXTRA_LEX):
        return "Extra lexem " + met.get_look() + " was found at " + say_pos(met.get_pos());
     case(Syn_err::ID_OR_NUM_EXPECT):
        res += "Identificator or number was expected at " + say_pos(met.get_pos());
        if (met.empty()) {
            return res + ", but nothing was found";
        } else {
            return res + ", but lexem " + met.get_look() + " was found";
        }
    }
}

SemError::SemError(Sem_err er,  string t, node_ptr x1, node_ptr oper):
    code(er), exp(x1), op(oper), type(t) {}

string SemError::what() const {
    string res = "", exp_look = "", op_look = "";
    node_ptr res_node;
    switch (code) {
    case (Sem_err::TYPE_MISMATCH):
        write_expression(exp,exp_look);
        write_expression(op,op_look);
        return "Operator " + op_look + " (" + say_pos(op->get_pos()) + ") requires type " +
                TypeDetector::say_type(type) + ", but " + exp_look + " (" + say_pos(exp->get_pos()) + ") has type " +
                TypeDetector::say_type(exp->get_type());
    case (Sem_err::BAD_TYPE_OF_INDEX):
        res_node = AnalysisTree::create_node("", exp, op);
        write_expression(res_node, exp_look);
        write_expression(op->get_children()[1], op_look);
        return "Error in expression " + exp_look + " (" + say_pos(res_node->get_pos()) +
                "). Index must have type " + TypeDetector::say_type(type) +
                ",but " + op_look + " has type "
                + TypeDetector::say_type(op->get_children()[1]->get_type());
    case(Sem_err::INDEX_NOT_ARRAY):
        res_node = AnalysisTree::create_node("", exp, op);
        write_expression(res_node, op_look);
        write_expression(exp, exp_look);
        return  "Error in expression " + op_look + " (" + say_pos(res_node->get_pos()) +
                "). Only arrays can be indexed, but " +
                exp_look + " has type " + TypeDetector::say_type(exp->get_type());

    }
}
