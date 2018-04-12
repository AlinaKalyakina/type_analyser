#include "lexseq.h"
#include <iostream>
#include "errors.h"

string Lex::get_look() const {
    return look;
}

LexType Lex::get_type() const {
    return type;
}

std::pair<int,int> Lex::get_pos() const {
    return pos;
}

void Lex::set_look(const std::string &x) {
    look = x;
}

void Lex::set_look(int x) {
    look.clear();
    look.push_back(x);
}

void Lex::set_pos(std::pair<int, int> x) {
    pos = x;
}
void Lex::set_type(LexType x) {
    type = x;
}
void Lex::clear() {
    look = "";
    type = LexType::EMPTY;
}

bool Lex::empty() const {
    return look == "" && type == LexType::EMPTY;
}

bool Lex::operator==(const Lex&x) const{
    return look == x.look && type == x.type && pos == x.pos;
}

bool Lex::operator!=(const Lex&x) const {
    return look != x.look || type != x.type || pos != x.pos;
}

bool Lex::operator==(LexType x) const{
    return type == x;
}

bool Lex::operator!=(LexType x) const{
    return type != x;
}

bool operator==(LexType x, Lex l) {
    return l.get_type() == x;
}

bool operator!=(LexType x, Lex l) {
    return l.get_type() != x;
}

LexIt::LexIt(ItPos x) {
    if (x == ItPos::BEGIN) {
        curstate = State::H;
        pos = std::make_pair(1,0);
        gc();
        ++*this;
    } else {
        curstate = State::END;
    }
}

Lex LexIt::operator* () const {
    return curlex;
}

void LexIt::gc() {
    if (curstate != State::END) {
        c = getchar();
        if (c == '\n') {
            pos.first++;
            pos.second = -1;
        }
        pos.second++;
    }
}
void LexIt::select_badlex() {
    while ((curstate != State::END) && (terminals.count(c)) &&(delims.count(c) + gaps.count(c) == 0) && c!= -1) {
        curline.push_back(c);
        gc();
    }
    curlex.set_look(curline);
    throw LexError(Lex_err::BADLEX, curlex);
}

LexIt& LexIt::operator++ () {
    curline.clear();
    curlex.clear();
    curlex.set_pos(pos);
    LexType type = LexType::EMPTY;
    do {
        if (!terminals.count(c)) {
            throw LexError(Lex_err::BADCHR, c, pos);
        }
        switch (curstate) {
        case State::H:
            if(isalpha(c)) {
                curline.push_back(c);
                switch (c) {
                case 'f':
                    curstate = State::F;
                    break;
                case 'a':
                    curstate = State::M;
                    break;
                case 'i':
                case 'j':
                case 'k':
                case 's':
                case 't':
                    curstate = State::I;
                    break;
                }
            } else {
                if (isdigit(c)) {
                    curline.push_back(c);
                    curstate = State::N;
                } else {
                    if (delims.count(c)) {
                        curline.push_back(c);
                        type = (delims.find(c))->second;
                    } else {
                        if (c == -1) {
                            curstate = State::END;
                        }
                    }
                }
            }
            break;

        case State::N:
            if(isalpha(c)) {
                select_badlex();
            } else {
                if (isdigit(c)) {
                    curline.push_back(c);
                } else {
                    curstate = State::H;
                    type = LexType::NUM;
                    continue;
                }
            }
            break;

        case State::M:
            if(isalpha(c)) {
                curline.push_back(c);
                switch (c) {
                case 'f':
                    curstate = State::F;
                    break;
                case 'i':
                case 'j':
                case 'k':
                case 's':
                case 't':
                    curstate = State::I;
                }
            } else {
                select_badlex();
            }
            break;

        case State::I:
            if(delims.count(c) + gaps.count(c) > 0) {
                curstate = State::H;
                type = LexType::ID;
                continue;
            } else {
                select_badlex();
            }
            break;

        case State::F:
            if(isalpha(c)) {
                switch(c) {
                case 'a':
                    curstate = State::M1;
                    break;
                case 'f': select_badlex();
                } //i, j, k, s, t doesn't change state
                curline.push_back(c);
            } else {
                if (isdigit(c)) {
                    select_badlex();
                } else {
                    curstate = State::H;
                    type = LexType::ID;
                    continue;
                }
            }
            break;

        case State::M1:
            if(isalpha(c)) {
                switch(c) {
                case 'f': select_badlex();
                case 'i':
                case 'j':
                case 'k':
                case 's':
                case 't':
                    curstate = State::F;
                } //'a' doesn't change state
                curline.push_back(c);
            } else {
                select_badlex();
                }
            }
        gc();
    } while (curstate != State::H && curstate != State::END);
    curlex.set_look(curline);
    curlex.set_type(type);
    return *this;
}

bool LexIt::operator==(const LexIt& x) const {
    return (x.curstate == State::END && curstate == State::END) || (x.pos == pos);
}

bool LexIt::operator!=(const LexIt& x) const {
    return (x.curstate != State::END || curstate != State::END) && (x.pos != pos);
}
LexIt LexSeq::cbegin () const
{
    return LexIt(ItPos::BEGIN);
}
LexIt LexSeq::begin () const
{
    return LexIt(ItPos::BEGIN);
}
LexIt LexSeq::end () const
{
    return LexIt(ItPos::END);
}
LexIt LexSeq::cend () const
{
    return LexIt(ItPos::END);
}
