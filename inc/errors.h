#ifndef ERRORS_H
#define ERRORS_H
#include <map>
#include "lexseq.h"
#include <string>
#include "analysistree.h"

enum class Lex_err{BADCHR, BADLEX};
enum class Syn_err{UNDEXP, EXTRA_LEX, ID_OR_NUM_EXPECT};
enum class Sem_err{BAD_TYPE_OF_INDEX, TYPE_MISMATCH, INDEX_NOT_ARRAY};


string say_pos(pos_type);

//!!!!!!!????????!!!!!!!
class Error {
public:
    virtual string what() const = 0;
    virtual ~Error();
};

class SemError: public Error
{
    Sem_err code;
    const_node_ptr exp;
    const_node_ptr op;
    string type;
public:
    SemError(Sem_err, string type = "", const_node_ptr expr = nullptr, const_node_ptr oper = nullptr);
    string what() const;
    ~SemError() {}
};

class SynError: public Error
{
    const_lex_ptr met, exp;
    Syn_err code;
public:
    SynError(Syn_err, const_lex_ptr found = nullptr,const_lex_ptr expected = nullptr);
    string what() const;
    ~SynError() {}
};

class LexError: public Error
{
    static std::map<Lex_err, string> messages;
    const_lex_ptr lex;
    Lex_err code;
public:
    LexError(Lex_err code, const_lex_ptr lex);
    LexError(Lex_err code, int symbol, const pos_type& position);
    string what() const;
    ~LexError() {}
};

#endif // ERRORS_H
