#include "typedetector.h"
#include "errors.h"

string TypeDetector::type_of_lex(const_lex_ptr x) {
    string type = x->get_look();
    if (x->get_type() == LexType::NUM) {
        return "ci";
    }
    if (delims.count(type[0])) {
        return "";
    }
    for(auto &c : type) {
        switch (c){
        case('j'):
        case('k'):
            c = 'i';
            break;
        case('t'):
            c = 's';
        }
    }
    return type;
}

string TypeDetector::op_check_and_res(Operation op_code, const_node_ptr x1, const_node_ptr op, const_node_ptr x2) {
    switch (op_code) {
    case (Operation::PLUS):
    case (Operation::MUL):
        if (x1->get_type() != "i" && x1->get_type() != "ci") {
            throw SemError(Sem_err::TYPE_MISMATCH, "i", x1,  op);
        }
        if (x2->get_type() != "i" && x2->get_type() != "ci") {
            throw SemError(Sem_err::TYPE_MISMATCH, "i", x2,  op);
        }
        return "i";
    case (Operation::INDEX):
        if (x1->get_type()[0] != 'a') {
            throw SemError(Sem_err::INDEX_NOT_ARRAY, NO_TYPE, x1, op);
        }
        if (op->get_children()[1]->get_type() != "i" && op->get_children()[1]->get_type() != "ci") {
            throw SemError(Sem_err::BAD_TYPE_OF_INDEX, "i", x1, op);
        }
        return x1->get_type().erase(0,1);//delete first 'a'
    }
}

string TypeDetector::say_type(string type){
    enum State{H, A, A1, A2, F1, F2, F3};
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
                curstate = A;
                break;
            case ('f'): res += "function, returning ";
                curstate = F1;
                break;
            }
            break;
        case (A):
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
                curstate = F1;
                break;
            }
            break;
        case (F1):
            switch (type[i]) {
            case ('\0'):
                return res +="void, with no parameters";
            case ('a'):
                res += "array of ";
                curstate = A1;
                break;
            case ('s'):
                res += "string, ";
                curstate = F2;
                break;
            case ('i'):
                res += "integer, ";
                curstate = F2;
                break;
            }
            break;
        case (A1):
            switch (type[i]) {
            case ('a'):
                res += "arrays of ";
                break;
            case ('s'):
                res += "string, ";
                curstate = F2;
                break;
            case ('i'):
                res += "integer, ";
                curstate = F2;
                break;
            }
            break;
        case (F2):
            if (type[i] == '\0') {
                return res += "with no parameters";
            } else {
                res += "parameters: ";
                curstate = F3;
                continue;
            }
        case (F3):
            switch (type[i]) {
            case ('\0'):
                res.erase(res.size() - 2, 2);
                return res;
            case ('a'):
                res += "array of ";
                curstate = A2;
                break;
            case ('i'):
                res += "integer, ";
                break;
            case ('s'):
                res += "string, ";
                break;
            }
            break;
        case (A2):
            switch (type[i]) {
            case ('a'):
                res += "arrays of ";
                break;
            case ('i'):
                res += "integer, ";
                curstate = F3;
                break;
            case ('s'):
                res += "string, ";
                curstate = F3;
                break;
            }
        }
        i++;
    }
}
