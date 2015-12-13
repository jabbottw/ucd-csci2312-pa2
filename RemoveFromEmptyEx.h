//
// Created by Julian Abbott-Whitley on 11/16/15.
//

#ifndef UCD_CSCI2312_PA2_REMOVEFROMEMPTYEX_H
#define UCD_CSCI2312_PA2_REMOVEFROMEMPTYEX_H
#include <iostream>

using namespace std;

namespace Clustering{

    class RemoveFromEmptyEx{
        static const int EX_ON;

    private:
        string name;
        string errMessage;
        int line;

    public:
        RemoveFromEmptyEx(string);
        RemoveFromEmptyEx(string, string);
        RemoveFromEmptyEx(string, string, int);

        string getName();
        string getErr();
        int getLine();


        friend std::ostream &operator<<(ostream &, RemoveFromEmptyEx &);
    };

}

#endif //UCD_CSCI2312_PA2_REMOVEFROMEMPTYEX_H
