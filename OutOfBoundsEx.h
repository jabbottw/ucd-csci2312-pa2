//
// Created by Julian Abbott-Whitley on 11/16/15.
//

#ifndef OUTOFBOUNDSEX_H
#define OUTOFBOUNDSEX_H

#include <iostream>

using namespace std;

namespace Clustering {

    class OutOfBoundsEx {

    private:
        string name;
        string errMessage;
        int line;

    public:
        OutOfBoundsEx(string);
        OutOfBoundsEx(string, string);
        OutOfBoundsEx(string, string, int);

        string getName();
        string getErr();
        int getLine();
        void clearMembers();

        friend std::ostream &operator<<(ostream &, OutOfBoundsEx &);
    };
}

#endif //OUTOFBOUNDSEX_H
