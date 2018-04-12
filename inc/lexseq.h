#ifndef LEXSEQ_H
#define LEXSEQ_H

#include "iterated.h"
#include <iterator>
#include <string>
#include <map>
#include <set>
#include <utility>

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
    string look;
    LexType type;
    pos_type pos;
public:
    Lex(string x = "", std::pair<int, int> p = std::make_pair(1,1), LexType t = LexType::EMPTY): look(x), type(t), pos(p){}
    string get_look() const;
    LexType get_type()const;
    std::pair<int, int> get_pos() const;
    void set_look(const string&);
    void set_look(int);
    void set_type(LexType);
    void set_pos(std::pair<int,int>);
    void clear();
    bool empty() const;
    bool operator== (const Lex&) const;
    bool operator!= (const Lex&) const;
    bool operator== (LexType) const;
    bool operator!= (LexType) const;
    friend bool operator== (LexType, Lex);
    friend bool operator!= (LexType, Lex);
};

const Lex EMPTY_LEX = Lex();

class LexIt: std::iterator<std::input_iterator_tag, const Lex>
{
    enum class State{H, F, M, M1, N, I, END};
    State curstate;
    int c;
    string curline;
    Lex curlex;
    std::pair<int, int> pos;
    void gc();
    void select_badlex();
public:
    LexIt(ItPos x = ItPos::END);
    LexIt& operator++();
    Lex operator*() const;
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
