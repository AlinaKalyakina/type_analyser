#include <iostream>
#include "lexseq.h"
#include <vector>
#include "errors.h"
#include "synanalys.h"

int main()
{
//    while (true) {
        try{
            LexSeq seq;
            SynAnalyser x(seq);
            const_tree tree = x.analyse();
            std::cout << std::endl << "Subexpressions in " << tree->get_look() << " have types: " << std::endl;
            write_subexpressions(tree);
        }
        catch(const Error& er) {std::cerr << std::endl <<er.what() << std::endl;}
        catch(...) { puts("\n!!!");}
//        std::cin.clear();
//    }
}
