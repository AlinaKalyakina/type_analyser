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
    virtual string get_look() const = 0;
    virtual LexType get_type() const = 0;
    virtual void set_look(const string&) = 0;
    virtual void set_look(int) = 0;
    pos_type get_pos() const;
    void set_pos(std::pair<int,int>);
    bool empty() const;
    void clear();
    virtual bool operator== (LexType) const = 0;
    virtual bool operator!= (LexType) const = 0;
    virtual ~Lex();
};

typedef std::shared_ptr<const Lex> const_lex_ptr;

typedef std::shared_ptr<Lex> lex_ptr;

lex_ptr create_lex(LexType, pos_type = std::make_pair<int, int>(0,0));
lex_ptr create_lex(string look, pos_type = std::make_pair<int, int>(0,0));


bool operator==(LexType x, const_lex_ptr l);
bool operator!=(LexType x, const_lex_ptr l);

class Id_lex: public Lex
{
    string look;
public:
    Id_lex(string look, pos_type pos);
    Id_lex(int, pos_type pos);
    string get_look() const;
    LexType get_type() const;
    void set_look(const string&);
    void set_look(int);
    bool operator== (LexType) const;
    bool operator!= (LexType) const;
};

class Num_lex: public Lex
{
    int val;
public:
    Num_lex(string look, pos_type pos);
    Num_lex(int, pos_type);
    string get_look() const;
    LexType get_type() const;
    void set_look(const string&);
    void set_look(int);
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
    void set_look(const string&);
    void set_look(int);
    void clear();
    bool empty() const;
    bool operator== (LexType) const;
    bool operator!= (LexType) const;
};

class LexIt: std::iterator<std::input_iterator_tag, const_lex_ptr>
{
    enum class State{H, F, M, M1, N, I, END};
    State curstate;
    int c;
    string curline;
    lex_ptr curlex;
    pos_type curpos, lexpos;
    void gc();
    void select_badlex();
public:
    LexIt(ItPos x = ItPos::END);
    LexIt& operator++();
    const_lex_ptr operator*() const;
    bool operator==(const LexIt&) const;
    bool operator!=(const LexIt&) const;
};

class LexSeq:Iterated<LexIt>
{
public:
    LexIt cbegin() const;
    LexIt cend() const;
    LexIt begin() const;
    LexIt end() const;
};

#endif // LEXSEQ_H
