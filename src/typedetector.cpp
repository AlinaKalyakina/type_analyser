#include "typedetector.h"
#include "errors.h"

string TypeDetector::type_determine(const Lex &lex) {
    switch(lex.type) {
    case(LexType::NUM):
        return "ci";//const int
    case(LexType::ID): {
        enum class State {H, FUNC, ARR, ARR_IN_FUNC, TYPE};
        State curstate = State::H;
        string look = lex.look;
        string type;
        unsigned long i = 0;
        do {
            switch(curstate){
            case(State::H):
                switch (look[i]) {
                case 'f':
                    type += 'f';
                    curstate = State::FUNC;
                    break;
                case 'a':
                    type += 'a';
                    curstate = State::ARR;
                    break;
                case 'i':
                case 'j':
                case 'k':
                    type += 'i';
                    curstate = State::TYPE;
                    break;
                case 's':
                case 't':
                    type += 's';
                    curstate = State::TYPE;
                    break;
                default:
                    throw SemError(Sem_err::UNTYPED_LEX, lex);
                }
                break;

            case(State::ARR):
                switch (look[i]) {
                case 'f':
                    type += 'f';
                    curstate = State::FUNC;
                    break;
                case 'a':
                    type += 'a';
                    break;
                case 'i':
                case 'j':
                case 'k':
                    type += 'i';
                    curstate = State::TYPE;
                    break;
                case 's':
                case 't':
                    type += 's';
                    curstate = State::TYPE;
                    break;
                default:
                    throw SemError(Sem_err::UNTYPED_LEX, lex);
                }
                break;
            case(State::FUNC):
                switch (look[i]) {
                case 'a':
                    type += 'a';
                    curstate = State::ARR_IN_FUNC;
                    break;
                case 'i':
                case 'j':
                case 'k':
                    type += 'i';
                    break;
                case 's':
                case 't':
                    type += 's';
                    break;
                case '\0':
                    return type;
                default:
                    throw SemError(Sem_err::UNTYPED_LEX, lex);
                }
                break;
            case(State::ARR_IN_FUNC):
                switch (look[i]) {
                case 'a':
                    type += 'a';
                    break;
                case 'i':
                case 'j':
                case 'k':
                    type += 'i';
                    curstate = State::FUNC;
                    break;
                case 's':
                case 't':
                    type += 's';
                    curstate = State::FUNC;
                    break;
                default:
                    throw SemError(Sem_err::UNTYPED_LEX, lex);
                }
                break;
            case(State::TYPE):
                if (look[i] != '\0') {
                    throw SemError(Sem_err::UNTYPED_LEX, lex);
                }
            }
            ++i;
        } while (i <= look.size());
        return type;
    }
    default:
        return "";
    }
}


string TypeDetector::say_type(string type){
    enum State{H, ARR, ARR_RETURN, ARR_PARAM, FUNC_INIT, FUNC_RETURN, FUNC_PARAM};
    auto curstate = H;
    string res = "";
    unsigned long i = 0;
    while (true) {
        switch (curstate) {
        case (H):
            switch (type[i]) {
            case ('\0'):
                return "no semantic type";
            case ('c'):
                return "numeral constant";
            case ('i'):
                return "integer";
            case ('s'):
                return "string";
            case ('a'):
                res += "array of ";
                curstate = ARR;
                break;
            case ('f'): res += "function, returning ";
                curstate = FUNC_INIT;
                break;
            }
            break;
        case (ARR):
            switch (type[i]) {
            case ('i'):
                return res += "integer";
            case ('s'):
                return res += "string";
            case ('a'):
                res += "arrays of ";
                break;
            case ('f'):
                res+= "functions, returning ";
                curstate = FUNC_INIT;
                break;
            }
            break;
        case (FUNC_INIT):
            switch (type[i]) {
            case ('\0'):
                return res +="void, with no parameters";
            case ('a'):
                res += "array of ";
                curstate = ARR_RETURN;
                break;
            case ('s'):
                res += "string, ";
                curstate = FUNC_RETURN;
                break;
            case ('i'):
                res += "integer, ";
                curstate = FUNC_RETURN;
                break;
            }
            break;
        case (ARR_RETURN):
            switch (type[i]) {
            case ('a'):
                res += "arrays of ";
                break;
            case ('s'):
                res += "string, ";
                curstate = FUNC_RETURN;
                break;
            case ('i'):
                res += "integer, ";
                curstate = FUNC_RETURN;
                break;
            }
            break;
        case (FUNC_RETURN):
            if (type[i] == '\0') {
                return res += "with no parameters";
            } else {
                res += "with parameters: ";
                curstate = FUNC_PARAM;
                continue;
            }
        case (FUNC_PARAM):
            switch (type[i]) {
            case ('\0'):
                res.erase(res.size() - 2, 2);
                return res;
            case ('a'):
                res += "array of ";
                curstate = ARR_PARAM;
                break;
            case ('i'):
                res += "integer, ";
                break;
            case ('s'):
                res += "string, ";
                break;
            }
            break;
        case (ARR_PARAM):
            switch (type[i]) {
            case ('a'):
                res += "arrays of ";
                break;
            case ('i'):
                res += "integer, ";
                curstate = FUNC_PARAM;
                break;
            case ('s'):
                res += "string, ";
                curstate = FUNC_PARAM;
                break;
            }
        }
        i++;
    }
}
