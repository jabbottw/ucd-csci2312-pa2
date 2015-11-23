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
        os.width(4);
        os << obErr.getLine() << ": ";
        os.width(65);
        os.setf(ios::left, ios::adjustfield);
        os << obErr.getName() << "  ";
        os.setf(ios::right, ios::adjustfield);
        os << obErr.getErr() << "\n";
        os.flush();
    }
}