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
            AnalysisTree tree = x.analyse();
            std::cout << std::endl << "Subexpressions:" << std::endl;
            tree.write_subexpressions();
        }
        catch(Error& er) {std::cerr << std::endl <<er.what() << std::endl;}
        catch(...) { puts("\n!!!");}
//        std::cin.clear();
//    }
}
