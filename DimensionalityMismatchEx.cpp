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
        string output = "Line: " + to_string(dimErr.getLine()) + " -- Exception Name: {" + dimErr.getName() + "} -- " +
                        dimErr.getErr() + "\n";
        os << output;
    }
}
#include "DimensionalityMismatchEx.h"
