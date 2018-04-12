#ifndef TYPEDETECTOR_H
#define TYPEDETECTOR_H
#include <string>
#include "lexseq.h"
#include "analysistree.h"
#include "synanalys.h"

using std::string;

class TypeDetector
{
public:
    TypeDetector();
    static string get_type(const Lex&);
    static string op_check_and_res(Operation, node_ptr x1, node_ptr op, node_ptr x2 = nullptr);
    static string say_type(string);
};

#endif // TYPEDETECTOR_H
