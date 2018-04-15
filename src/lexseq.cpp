#include "lexseq.h"
#include <iostream>
#include <memory>
#include "errors.h"

//Lex
Lex::Lex(pos_type p) : pos(p), empty_lex(false) {}

Lex::~Lex() {}

pos_type Lex::get_pos() const {
    return pos;
}

void Lex::set_pos(std::pair<int, int> x) {
    pos = x;
}

void Lex::clear() {
    empty_lex = true;
}

bool Lex::empty() const {
    return empty_lex;
}

bool operator==(LexType x, const_lex_ptr l) {
    return l->get_type() == x;
}

bool operator!=(LexType x, const_lex_ptr l) {
    return l->get_type() != x;
}

//Id_lex
Id_lex::Id_lex(string view, pos_type pos) : Lex(pos), look(view) {}

Id_lex::Id_lex(int c, pos_type pos) : Lex(pos) {
    look.clear();
    look += c;
}

string Id_lex::get_look() const {
    return look;
}

LexType Id_lex::get_type() const {
    return LexType::ID;
}

void Id_lex::set_look(const std::string &x) {
    look = x;
}

void Id_lex::set_look(int x) {
    look.clear();
    look += x;
}

bool Id_lex::operator==(LexType x) const{
    return x == LexType::ID;
}

bool Id_lex::operator!=(LexType x) const{
    return x != LexType::ID;
}

//Num_lex
Num_lex::Num_lex(string look, pos_type pos) : Lex(pos),
    val(std::stoi(look)) {}

Num_lex::Num_lex(int n, pos_type pos) : Lex(pos), val(n) {}

string Num_lex::get_look() const {
    return std::to_string(val);
}

LexType Num_lex::get_type() const {
    return LexType::NUM;
}

void Num_lex::set_look(const std::string &x) {
    val = std::stoi(x);
}

void Num_lex::set_look(int x) {
    val = x;
}

bool Num_lex::operator==(LexType x) const{
    return x == LexType::NUM;
}

bool Num_lex::operator!=(LexType x) const{
    return x != LexType::NUM;
}

//Delim_lex
Delim_lex::Delim_lex(string look, pos_type pos) : Lex(pos),
    type(delims.find(look[0])->second) {}

Delim_lex::Delim_lex(LexType p, pos_type pos) : Lex(pos), type(p) {}

string Delim_lex::get_look() const {
    string res = "";
    for(auto x : delims) {
        if(x.second == type) {
            return res += x.first;
        }
    }
    return "";
}

LexType Delim_lex::get_type() const {
    return type;
}

void Delim_lex::set_look(const std::string &x) {
    type = delims.find(x[0])->second;
}

void Delim_lex::set_look(int x) {
    type = LexType(x);
}

bool Delim_lex::operator==(LexType x) const{
    return x == type;
}

bool Delim_lex::operator!=(LexType x) const{
    return x != type;
}


lex_ptr create_lex(LexType x, pos_type pos) {
    switch (x) {
    case(LexType::ID):
        return std::make_shared<Id_lex>("", pos);
    case(LexType::NUM):
        return std::make_shared<Num_lex>(0, pos);
    default:
        return std::make_shared<Delim_lex>(x, pos);
    }
}

lex_ptr create_lex(string look, pos_type pos) {
    if (isalpha(look[0])) {
        return std::make_shared<Id_lex>(look, pos);
    } else {
        if (isdigit(look[0])) {
            return std::make_shared<Num_lex>(look, pos);
        } else {
            return std::make_shared<Delim_lex>(look,pos);
        }
    }
}

lex_ptr empty_lex(pos_type pos) {
    auto lex = create_lex(LexType::ID, pos);
    lex->clear();
    return lex;
}
//LexIt
LexIt::LexIt(ItPos x) {
    if (x == ItPos::BEGIN) {
        curstate = State::H;
        curpos = std::make_pair(1,0);
        gc();
        ++*this;
    } else {
        curstate = State::END;
    }
}

const_lex_ptr LexIt::operator* () const {
    if (curstate != State::END) {
        return create_lex(curlex->get_look(), curlex->get_pos());
    } else {
        return empty_lex(curpos);
    }
}

void LexIt::gc() {
    if (curstate != State::END) {
        c = getchar();
        if (c == '\n') {
            curpos.first++;
            curpos.second = -1;
        }
        curpos.second++;
    }
}
void LexIt::select_badlex() {
    while ((curstate != State::END) && (terminals.count(c)) &&(delims.count(c) + gaps.count(c) == 0) && c != -1) {
        curline += c;
        gc();
    }
    curlex = create_lex(curline, lexpos);
    throw LexError(Lex_err::BADLEX, curlex);
}

LexIt& LexIt::operator++ () {
    curline.clear();
    lexpos = curpos;
    bool lex_end = false;
    do {
        if (!terminals.count(c)) {
            throw LexError(Lex_err::BADCHR, c, curpos);
        }
        switch (curstate) {
        case State::H:
            if(isalpha(c)) {
                curline += c;
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
                    curline += c;
                    curstate = State::N;
                } else {
                    if (delims.count(c)) {
                        curline += c;
                        lex_end = true;
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
                    curline += c;
                } else {
                    curstate = State::H;
                    lex_end = true;
                    continue;
                }
            }
            break;

        case State::M:
            if(isalpha(c)) {
                curline += c;
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
                lex_end = true;
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
                curline += c;
            } else {
                if (isdigit(c)) {
                    select_badlex();
                } else {
                    curstate = State::H;
                    lex_end = true;
                    continue;
                }
            }
            break;

        case State::M1:
            if(isalpha(c)) {
                switch(c) {
                case 'f': select_badlex();
                    break;
                case 'i':
                case 'j':
                case 'k':
                case 's':
                case 't':
                    curstate = State::F;
                } //'a' doesn't change state
                curline += c;
            } else {
                select_badlex();
                }
            }
        gc();
    } while (!lex_end && curstate != State::END);
    curlex = create_lex(curline, lexpos);
    return *this;
}

bool LexIt::operator==(const LexIt& x) const {
    return curstate == x.curstate;
}

bool LexIt::operator!=(const LexIt& x) const {
    return curstate != x.curstate;
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
