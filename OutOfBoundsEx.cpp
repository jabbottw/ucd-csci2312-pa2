//
// Created by Julian Abbott-Whitley on 11/16/15.
//
#include <iostream>
#include <string>
#include "OutOfBoundsEx.h"

namespace Clustering {

    OutOfBoundsEx::OutOfBoundsEx(string name) {
        this->clearMembers();
        this->name = name;
        this->errMessage = "Out of bounds error";
    }

    OutOfBoundsEx::OutOfBoundsEx(string name, string str) {
        this->clearMembers();
        this->name = name;
        this->errMessage = str;
    }

    OutOfBoundsEx::OutOfBoundsEx(string name, string str, int line_num) {
        this->clearMembers();
        this->name = name;
        this->errMessage = str;
        this->line = line_num;
    }

    string OutOfBoundsEx::getName() {return this->name;}
    string OutOfBoundsEx::getErr() {return this->errMessage;}
    int OutOfBoundsEx::getLine() {return this->line;}

    void OutOfBoundsEx::clearMembers() {
        this->name = "";
        this->errMessage = "";
        this->line = -1;
    }

    ostream &operator<<(ostream & os, OutOfBoundsEx & obErr) {
        string output = "Line: " + to_string(obErr.getLine()) + " -- Exception Name: {" + obErr.getName() + "} -- " +
                        obErr.getErr() + "\n";
        os << output;
    }
}