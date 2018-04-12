#ifndef ERRORS_H
#define ERRORS_H
#include <map>
#include <lexseq.h>
#include <string>
#include <analysistree.h>

enum class Lex_err{BADCHR, BADLEX};
enum class Syn_err{UNDEXP, EXTRA_LEX, ID_OR_NUM_EXPECT};
enum class Sem_err{BAD_TYPE_OF_INDEX, TYPE_MISMATCH, INDEX_NOT_ARRAY};


string say_pos(pos_type);

//!!!!!!!????????!!!!!!!
class Error {
public:
    virtual string what() const = 0;
    virtual ~Error() {}
};

class SemError: public Error
{
    Sem_err code;
    node_ptr exp;
    node_ptr op;
    string type;
public:
    SemError(Sem_err, string type = "", node_ptr expr = nullptr, node_ptr oper = nullptr);
    string what() const;
    ~SemError() {}
};

class SynError: public Error
{
    Lex met, exp;
    Syn_err code;
public:
    SynError(Syn_err, const Lex& found = EMPTY_LEX, const Lex& expected = EMPTY_LEX);
    string what() const;
    ~SynError() {}
};

class LexError: public Error
{
    static std::map<Lex_err, string> messages;
    Lex lex;
    Lex_err code;
public:
    LexError(Lex_err, const Lex&);
    LexError(Lex_err, int, const pos_type&);
    string what() const;
    ~LexError() {}
};

#endif // ERRORS_H
