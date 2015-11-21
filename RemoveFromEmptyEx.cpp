//
// Created by Julian Abbott-Whitley on 11/16/15.
//
#include <iostream>
#include <string>
#include "RemoveFromEmptyEx.h"

using namespace std;

namespace Clustering{

    RemoveFromEmptyEx::RemoveFromEmptyEx(string name) {
        this->name = name;
        this->errMessage = "Cluster doesn't contain any points. Can not remove from empty cluster";
    }

    RemoveFromEmptyEx::RemoveFromEmptyEx(string name, string str) {
        this->name = name;
        this->errMessage = str;
    }

    RemoveFromEmptyEx::RemoveFromEmptyEx(string name, string str, int line_num){
        this->name = name;
        this->errMessage = str;
        this->line = line_num;
    }

    string RemoveFromEmptyEx::getName() {return this->name;}
    string RemoveFromEmptyEx::getErr() {return this->errMessage;}
    int RemoveFromEmptyEx::getLine() {return this->line;}

    ostream &operator<<(ostream & os, RemoveFromEmptyEx & rmErr) {
        string output = "Line: " + to_string(rmErr.getLine()) + " -- Exception Name: {" + rmErr.getName() + "} -- " +
                rmErr.getErr() + "\n";
        os << output;
    }

}