#include <iostream>
#include "lexseq.h"
#include <vector>
#include "errors.h"
#include "synanalys.h"

int main()
{
    try{
        LexSeq seq;
        SynAnalyser x(seq);
        auto tree = x.analyse();
        std::cout << std::endl << "Subexpressions have types: " << std::endl;
        write_subexpressions(tree);
    }
    catch(const Error& er) {std::cout << std::endl << er.what() << std::endl;}
}
