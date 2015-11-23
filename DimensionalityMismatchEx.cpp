//
// Created by Julian Abbott-Whitley on 11/16/15.
//
#include <iostream>
#include <string>
#include "DimensionalityMismatchEx.h"

using namespace std;

namespace Clustering {

    // Constructors
    DimensionalityMismatchEx::DimensionalityMismatchEx(string name) {
        this->clearMembers();
        this->name = name;
        this->errMessage = "Dimensionality Mismatch Error";
    }


    DimensionalityMismatchEx::DimensionalityMismatchEx(string name, string str) {
        this->clearMembers();
        this->name = name;
        this->errMessage = str;
    }


    DimensionalityMismatchEx::DimensionalityMismatchEx(string name, string str, int line_num) {
        this->clearMembers();
        this->name = name;
        this->errMessage = str;
        this->line = line_num;
    }

    // Getters
    string DimensionalityMismatchEx::getName() {return this->name;}
    string DimensionalityMismatchEx::getErr() {return this->errMessage;}
    int DimensionalityMismatchEx::getLine() {return this->line;}

    void DimensionalityMismatchEx::clearMembers() {
        this->name = "";
        this->errMessage = "";
        this->line = -1;
    }

    ostream &operator<<(ostream & os, DimensionalityMismatchEx & dimErr) {
        os.width(4);
        os << dimErr.getLine() << ": ";
        os.width(65);
        os.setf(ios::left, ios::adjustfield);
        os << dimErr.getName() << "  ";
        os.setf(ios::right, ios::adjustfield);
        os << dimErr.getErr() << "\n";
        os.flush();
    }
}
#include "DimensionalityMismatchEx.h"
