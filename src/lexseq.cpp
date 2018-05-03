#include "lexseq.h"
#include <iostream>
#include <memory>
#include "errors.h"

//Lex
Lex::Lex(LexType t, pos_type p, string l) {
    for (auto i : sign) {
        if (i.second == t) {
            look += i.first;
            return;
        }
    }
    pos = p;
    if ((type = t) == LexType::ID || t == LexType::NUM) {
        look = l;
    }
}

//const_Lex_it
bool const_Lex_it::exist_flag = false;

const_Lex_it::const_Lex_it(ItPos x) {
    if (x == ItPos::BEGIN) {
        if (exist_flag) {
            throw "Attempt to create one more lexem iterator!!!";
        }
        exist_flag = true;
        curstate = State::H;
        curpos = std::make_pair(1,0);
        gc();
        ++*this;
    } else {
        curstate = State::END;
    }
}

const_Lex_it::const_Lex_it(const_Lex_it&& x) {
    curstate = x.curstate;
    x.curstate = State::END;
    c = x.c;
    x.c = -1;
    curline = x.curline;
    x.curline.clear();
    curtype = x.curtype;
    curtype = LexType::EMPTY;
    curlex = x.curlex;
    x.curlex = Lex();
    curpos = x.curpos;
    x.curpos = std::make_pair<int, int>(0,0);
    std::cout << "Constructor worked";
}

const_Lex_it& const_Lex_it::operator=(const_Lex_it&& x) {
    if (this != &x) {
        curstate = x.curstate;
        x.curstate = State::END;
        c = x.c;
        x.c = -1;
        curline = x.curline;
        x.curline.clear();
        curtype = x.curtype;
        curtype = LexType::EMPTY;
        curlex = x.curlex;
        x.curlex = Lex();
        curpos = x.curpos;
        x.curpos = std::make_pair<int, int>(0,0);
    }
    return *this;
}

const_Lex_it::~const_Lex_it() {
    if (curstate != State::END) {
        exist_flag = false;
    }
}


//
Lex const_Lex_it::operator* () const {
    return curlex;
}

void const_Lex_it::gc() {
    if (curstate != State::END) {
        c = getchar();
        if (c == '\n') {
            curpos.first++;
            curpos.second = 0;
            return;
        } else {
            if (c == '\t') {
                curpos.second += Sizes::TAB_SIZE;
                return;
            }
        }
        curpos.second++;
    }
}

const_Lex_it& const_Lex_it::operator++ () {
    if (curstate == State::END) {
        return *this;
    }
    curline.clear();
    curtype = LexType::EMPTY;
    do {
        switch (curstate) {
        case State::H:
            if(isalpha(c)) {
                curline += c;
                curstate = State::ID;
            } else {
                if (isdigit(c)) {
                    curline += c;
                    curstate = State::NUM;
                } else {
                    if (sign.count(c)) {
                        curline += c;
                        curtype = sign.find(c)->second;
                    } else {
                        if (c == -1) {
                            curstate = State::END;
                            curlex.type = LexType::EMPTY;
                            exist_flag = false;
                        } else {
                            if (!spaces.count(c)) {
                                throw LexError(c, curpos);
                            }
                        }
                    }
                }
            }
            curlex.pos = curpos;
        break;

        case State::NUM:
            if (isdigit(c)) {
                curline += c;
            } else {
                curtype = LexType::NUM;
                curstate = State::H;
                continue;
            }
        break;

        case State::ID:
            if (isalpha(c) || isdigit(c)) {
                curline += c;
            } else {
                curstate = State::H;
                curtype = LexType::ID;
                continue;
            }
        }
        gc();
    } while (curtype == LexType::EMPTY && curstate != State::END);
    curlex.type = curtype;
    curlex.look = curline;
    return *this;
}

bool const_Lex_it::operator==(const const_Lex_it& x) const {
    return curstate == x.curstate && (curpos == x.curpos || (curstate == State::END && x.curstate == State::END));
}

bool const_Lex_it::operator!=(const const_Lex_it& x) const {
    return curstate != x.curstate || (x.curpos != x.curpos && (curstate != State::END || x.curstate != State::END));
}

const_Lex_it LexSeq::cbegin () const
{
    return const_Lex_it(ItPos::BEGIN);
}

const_Lex_it LexSeq::cend () const
{
    return const_Lex_it(ItPos::END);
}
