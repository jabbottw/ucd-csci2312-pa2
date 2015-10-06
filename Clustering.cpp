//
// Created by Julian Abbott-Whitley on 9/20/15.
//

#include <iostream>
#include <String>
#include "Point.h"
#include "Clustering.h"

using namespace std;

namespace Clustering {

    Cluster::Cluster() {
        size = 0;
        points = nullptr;
    }

    // Copy Constructor
    Cluster::Cluster(const Cluster & clstr) {
        if (this == &clstr){
            // do nothing
        } else {
            this->size = 0;
            }
            for (LNodePtr nCursor = clstr.points; nCursor != nullptr; ){
                this->add(nCursor->p);
                nCursor = nCursor->next;
            }
    }

    // Overloaded assignment operator
    Cluster &Cluster::operator=(const Cluster & clstr){
        if (this == &clstr){
            return *this;
        } else {
            this->size = 0;
            for (LNodePtr nCursor = this->points; nCursor != nullptr; ) {
                delete nCursor->p;
                LNodePtr nPrev = nCursor;
                nCursor=nCursor->next;
                delete nPrev;
            }
            for (LNodePtr nCursor = clstr.points; nCursor != nullptr; ){
                this->add(nCursor->p);
            }
        }
        return *this;
    }

    // Destructor
    Cluster::~Cluster(){

    }

    int Cluster::getSize(){
        return this->size;
    }

    LNodePtr Cluster::getPoints(){
        return this->points;
    }

    void Cluster::add(const PointPtr & pntPointer) {
        // Create a pointer to a new LNode structure object, dynamically allocated on the heap
        LNodePtr nPntr = new LNode;
        // Update the new Node, set the PointPtr to point at the input pntPointer address
        nPntr->p = pntPointer;
        nPntr->next = nullptr;
        //LNodePtr nPrevious;
        LNodePtr nCursor;
        LNodePtr nPrev;
        // Find the correct spot in the list to insert the points
        // Inserting points in lexigraphical order
        switch (this->size) {
            // The current list is empty, insert the node at the start
            case 0:
                this->points = nPntr;
                this->points->next = nullptr;
                break;
            // If there is only one node, just determine the correct spot for the new node, (i.e. 1st or 2nd)
            case 1:
                // if point one is greater than input point, then place new point at slot one in cluster
                if (*this->points->p > *nPntr->p){
                    nCursor = this->points;
                    this->points = nPntr;
                    this->points->next = nCursor;
                } else {
                    this->points->next = nPntr;
                }
                break;
            default:
                nPrev = nullptr;
                // Iterate through the linked list and place point appropriately
                for (nCursor = this->points; nCursor != nullptr; nCursor = nCursor->next){
                    // If nPrev is null and the input point is smaller than the cursor, place point at begining of list
                    if ((nPrev == nullptr) && (*nPntr->p < *nCursor->p)){
                        nPntr->next = nCursor;
                        this->points = nPntr;
                        break;
                    // If the points have equal dim values, then sort by address, (descending order)
                    } else if ((*nPntr->p == *nCursor->p) and (nPntr->p > nCursor->p)) {
                        nPrev->next = nPntr;
                        nPntr->next = nCursor;
                    // If the input point is greater than the node cursor, place point in between prevCursor and nodeCursor
                    } else if (*nPntr->p < *nCursor->p ){
                        nPrev->next = nPntr;
                        nPntr->next = nCursor;
                        break;
                    // Continue looping
                    } else {
                        nPrev = nCursor;
                    }

                }
                // If we made it through the whole list, then the input point goes at the end
                if (nCursor == nullptr){
                    nPrev->next = nPntr;
                }
        }
        // Increment the size of the cluster
        this->size += 1;
    }

    const PointPtr & Cluster::remove(const PointPtr & pntPointer){
        LNodePtr nodeCursor;
        LNodePtr previousNode;
        previousNode = nullptr;

        for (nodeCursor = this->points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next) {
            if (nodeCursor->p == pntPointer) {
                if(previousNode == nullptr){
                    this->points = this->points->next;
                    --(this->size);
                    return pntPointer;
                } else {
                    previousNode->next = nodeCursor->next;
                    --(this->size);
                    return pntPointer;
                }
            } else {
                previousNode = nodeCursor;
            }
        }
        return pntPointer;
    }


    Cluster & Cluster::operator+=(const Point &rhs){
        Point rhsCopy = rhs;
        rhsCopy.setDimSize(rhs.getDimSize());
        PointPtr p = &rhsCopy;
        this->add(p);
        return *this;
    }

    Cluster & Cluster::operator-=(const Point &rhs){
        Point rhsCopy = rhs;
        rhsCopy.setDimSize(rhs.getDimSize());
        PointPtr p = &rhsCopy;
        this->remove(p);
        return *this;
    }

    // Union of the two clusters
    Cluster & Cluster::operator+=(const Cluster &rhs){
        if (this->size == 0){
            for (LNodePtr rhsNodeCursor = rhs.points; rhsNodeCursor != nullptr; rhsNodeCursor = rhsNodeCursor->next) {
                this->add(rhsNodeCursor->p);
            }
        } else {
            LNodePtr thisNodeCursor;
            // iterate through all the nodes within the RHS cluster
            for (LNodePtr rhsNodeCursor = rhs.points; rhsNodeCursor != nullptr; rhsNodeCursor = rhsNodeCursor->next) {
                thisNodeCursor = this->points;
                // iterate through the Nodes in THIS cluster
                // While the current node->point for THIS cluster is not equal to the  current rhsCluster node->point
                // Continue looping through THIS cluster nodes
                // Stop at the end of the linked list or once a match is found
                while (thisNodeCursor->p != rhsNodeCursor->p and thisNodeCursor->next != nullptr) {
                    thisNodeCursor = thisNodeCursor->next;
                }
                if (thisNodeCursor != nullptr) {
                    this->add(rhsNodeCursor->p);
                }
            }
        }
    }

    // Asymmetric Difference
    Cluster & Cluster::operator-=(const Cluster &rhs){
        LNodePtr thisNodeCursor;
        // iterate through all the nodes within the RHS cluster
        for(LNodePtr rhsNodeCursor = rhs.points; rhsNodeCursor != nullptr; rhsNodeCursor = rhsNodeCursor->next){
            thisNodeCursor = this->points;
            // iterate through the Nodes in THIS cluster
            // While the current node->point for THIS cluster is not equal to the  current rhsCluster node->point
            // Continue looping through THIS cluster nodes
            // Stop at the end of the linked list or once a match is found
            while (thisNodeCursor->p != rhsNodeCursor->p and thisNodeCursor->next != nullptr) {
                thisNodeCursor = thisNodeCursor->next;
            }
            if (thisNodeCursor->p == rhsNodeCursor->p){
                this->remove(thisNodeCursor->p);
            }
        }
    }

    bool operator==(const Cluster &lhs, const Cluster &rhs){
        if(lhs.size != rhs.size) {
            return false;
        } else {
            LNodePtr rhsCursor = rhs.points;
            for (LNodePtr lhsCursor = lhs.points; lhsCursor != nullptr;  lhsCursor = lhsCursor->next){
                if(lhsCursor->p != rhsCursor->p){
                    return false;
                } else {
                    rhsCursor = rhsCursor->next;
                }
            }
        }
        return true;
    }

    bool operator!=(const Cluster &lhs, const Cluster &rhs){
        if (lhs == rhs){
            return false;
        } else {
            return true;
        }
    }

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs){
        Cluster clstrUnion(lhs);
        clstrUnion += rhs;
        return clstrUnion;
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs){
        Cluster clstrUnion(lhs);
        clstrUnion -= rhs;
        return clstrUnion;
    }

    const Cluster operator+(const Cluster &lhs, const PointPtr &rhs){
        Cluster clstrCopy(lhs);
        clstrCopy.add(rhs);
        return clstrCopy;
    }


    const Cluster operator-(const Cluster &lhs, const PointPtr &rhs){
        Cluster clstrCopy(lhs);
        clstrCopy.remove(rhs);
        return clstrCopy;
    }

    ostream &operator<<(ostream &os, const Cluster &clstr){
        LNodePtr nodeCursor = clstr.points;
        cout << "[";
        for (int i = 0; i < clstr.size; ++i) {
            cout << *(nodeCursor->p);
            if (i == clstr.size-1){
                // do nothing
            } else {
                cout << ", ";
            }
            nodeCursor = nodeCursor->next;
        }
        cout << "]" << endl;
        return os;
    }

    istream &operator>>(istream &is, Cluster &clstr){
        is >> clstr;
        return is;
    }

    void Cluster::printPointAddresses(){
        LNodePtr cursor = this->points;
        for (int i = 0; i < this->size; i++){
            cout << "Point " << i << ": " << cursor->p << endl;
            cursor = cursor->next;
        }
    }
}

