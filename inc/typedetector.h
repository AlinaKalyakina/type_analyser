#ifndef TYPEDETECTOR_H
#define TYPEDETECTOR_H
#include <string>
#include "lexseq.h"
#include "analysistree.h"
#include "synanalys.h"

using std::string;

namespace TypeDetector {
    string type_determine(const Lex& lex);
    string say_type(string);
}

#endif // TYPEDETECTOR_H
