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

const std::map<int, LexType> sign {{'+',LexType::PLUS}, {'*',LexType::MUL},
                                         {'(',LexType::LPAREN}, {')',LexType::RPAREN},
                                         {'[',LexType::LSQBRACKET}, {']', LexType::RSQBEACKET}};
const std::set<int> spaces {' ', '\n', '\t', '\0',-1};

enum Sizes{TAB_SIZE = 4};

struct Lex
{
    string look;
    LexType type = LexType::EMPTY;
    pos_type pos = std::make_pair<int, int> (-1,-1);
    Lex(LexType type, pos_type pos = std::make_pair<int, int>(-1, -1));
    Lex() {}
};

class const_Lex_it: std::iterator<std::input_iterator_tag, Lex>
{
    enum class State{H, NUM, ID, END};
    State curstate;
    int c;
    string curline;
    LexType curtype;
    Lex curlex;
    pos_type curpos;
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
    Lex operator*() const;
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
