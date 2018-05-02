#ifndef ERRORS_H
#define ERRORS_H
#include <map>
#include "lexseq.h"
#include <string>
#include "analysistree.h"

enum class Lex_err{BADCHR};
enum class Syn_err{UNDEXP, EXTRA_LEX, ID_OR_NUM_EXPECT};
enum class Sem_err{BAD_TYPE_OF_INDEX, TYPE_MISMATCH,
                   INDEX_NOT_ARRAY, UNTYPED_LEX};

class Error {
public:
    virtual string what() const = 0;
    virtual ~Error();
};

class SemError: public Error
{
    Sem_err code;
    const_node_ptr expression = nullptr;
    string required_type;
    Operation operation = Operation::VAR;
    pos_type pos;
    Lex lex = Lex();
public:
    SemError(Sem_err code, Operation operation, 
             const_node_ptr expression, string required_type = "", pos_type pos = pos_type(0, 0));
    SemError(Sem_err code, const Lex& lex);
    string what() const;
    ~SemError() {}
};

class SynError: public Error
{
    Lex met, exp;
    Syn_err code;
public:
    SynError(Syn_err, const Lex& found, Lex exp = Lex());
    string what() const;
    ~SynError() {}
};

class LexError: public Error
{
    char c;
    pos_type position;
public:
    LexError(int symbol, const pos_type& position);
    string what() const;
    ~LexError() {}
};

#endif // ERRORS_H
