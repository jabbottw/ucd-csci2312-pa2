//
// Created by Julian Abbott-Whitley on 11/16/15.
//

#ifndef DIMENSIONALITYMISMATCHEX_H
#define DIMENSIONALITYMISMATCHEX_H

#include <iostream>
#include <string>

using namespace std;

namespace Clustering {

    class DimensionalityMismatchEx {

    private:
        string name;
        string errMessage;
        int line;

    public:
        // Constructors
        DimensionalityMismatchEx(string);

        DimensionalityMismatchEx(string, string);

        DimensionalityMismatchEx(string, string, int);

        // Getters
        string getName();
        string getErr();
        int getLine();

        void clearMembers();

        friend std::ostream &operator<<(ostream &, DimensionalityMismatchEx &);

    };
}



#endif //DIMENSIONALITYMISMATCHEX_H
