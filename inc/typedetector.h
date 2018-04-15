#ifndef TYPEDETECTOR_H
#define TYPEDETECTOR_H
#include <string>
#include "lexseq.h"
#include "analysistree.h"
#include "synanalys.h"

using std::string;

const string NO_TYPE = "";

namespace TypeDetector {
    string type_of_lex(const_lex_ptr lex);
    string op_check_and_res(Operation, const_node_ptr x1, const_node_ptr op, const_node_ptr x2 = nullptr);
    string say_type(string);
}

#endif // TYPEDETECTOR_H
