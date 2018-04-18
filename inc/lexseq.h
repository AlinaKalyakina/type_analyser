#ifndef LEXSEQ_H
#define LEXSEQ_H
#include "iterated.h"
#include <iterator>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <memory>

using std::string;
typedef std::pair<int, int> pos_type;

enum class LexType{EMPTY, ID, NUM, PLUS, MUL, LPAREN, RPAREN, LSQBRACKET, RSQBEACKET};
enum class ItPos{BEGIN, END};

const std::set<int> terminals {'i','j','k','s','t','a','f','0','1','2','3','4','5','6','7','8','9',
                                '+','*','[',']','(',')',' ','\n','\t', -1};

const std::map<int, LexType> delims {{'+',LexType::PLUS}, {'*',LexType::MUL},
                                         {'(',LexType::LPAREN}, {')',LexType::RPAREN},
                                         {'[',LexType::LSQBRACKET}, {']', LexType::RSQBEACKET}};
const std::set<int> gaps {' ', '\n', '\t', '\0',-1};

const int end = 0;

class Lex
{
    pos_type pos;
    bool empty_lex;
public:
    Lex(pos_type pos);
    pos_type get_pos() const;
    bool empty() const;
    void clear();
    virtual string get_look() const = 0;
    virtual LexType get_type() const = 0;
    virtual bool operator== (LexType) const = 0;
    virtual bool operator!= (LexType) const = 0;
    virtual ~Lex();
};

typedef std::shared_ptr<const Lex> const_lex_ptr;

typedef std::shared_ptr<Lex> lex_ptr;

lex_ptr create_lex(LexType, pos_type = std::make_pair<int, int>(0,0));
lex_ptr create_lex(string look, pos_type = std::make_pair<int, int>(0,0), bool badlex = false);


bool operator==(LexType x, const Lex& l);
bool operator!=(LexType x, const Lex& l);

class Id_lex: public Lex
{
    string look;
public:
    Id_lex(string look, pos_type pos);
    Id_lex(int, pos_type pos);
    string get_look() const;
    LexType get_type() const;
    bool operator== (LexType) const;
    bool operator!= (LexType) const;
};

class Num_lex: public Lex
{
    string val;
public:
    Num_lex(string look, pos_type pos);
    string get_look() const;
    LexType get_type() const;
    bool operator== (LexType) const;
    bool operator!= (LexType) const;
};

class Delim_lex: public Lex
{
    LexType type;
public:
    Delim_lex(string look, pos_type pos);
    Delim_lex(LexType type, pos_type pos);
    string get_look() const;
    LexType get_type() const;
    bool operator== (LexType) const;
    bool operator!= (LexType) const;
};

class const_Lex_it: std::iterator<std::input_iterator_tag, const_lex_ptr>
{
    enum class State{H, F, M, M1, N, I, END};
    State curstate;
    int c;
    string curline;
    lex_ptr curlex;
    pos_type curpos, lexpos;
    void gc();
    [[noreturn]] void select_badlex();
    static bool exist_flag;
public:
    const_Lex_it(ItPos x = ItPos::END);
    const_Lex_it(const const_Lex_it&) = delete;
    const_Lex_it(const_Lex_it&&);
    const_Lex_it& operator=(const const_Lex_it&) = delete;
    const_Lex_it& operator=(const_Lex_it&&);
    const_Lex_it& operator++();
    const_lex_ptr operator*() const;
    bool operator==(const const_Lex_it&) const;
    bool operator!=(const const_Lex_it&) const;
    ~const_Lex_it();
};

class LexSeq:Iterated<const_Lex_it>
{
public:
    const_Lex_it cbegin() const;
    const_Lex_it cend() const;
};

#endif // LEXSEQ_H
