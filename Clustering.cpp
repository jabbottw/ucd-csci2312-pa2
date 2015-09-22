//
// Created by Julian Abbott-Whitley on 9/20/15.
//

#include <iostream>
#include "Point.h"
#include "Clustering.h"

using namespace std;

namespace Clustering {

    Cluster::Cluster() {
        size = 0;
        points = nullptr;
    }


    Cluster::Cluster(const Cluster & clstr) {
        size = 0;
        if (clstr.points != nullptr){
            LNodePtr nodeCursor = clstr.points;
            for (int i = 0; i < clstr.size; ++i){
                //PointPtr pnt(nodeCursor->p);
                // This method doesn't create a new point for each LNode copied
                this->add(nodeCursor->p);
                nodeCursor = nodeCursor->next;
            }
        }
    }


    Cluster &Cluster::operator=(const Cluster & clstr){
        if (this == &clstr){
            return *this;
        } else {
            LNodePtr nodeCursorA = this->points;
            for (int i = 0; i < this->size; i ++){
                delete nodeCursorA->p;
                LNodePtr nodeCursorB = nodeCursorA->next;
                delete nodeCursorA;
                nodeCursorA = nodeCursorB;
            }
            delete this->points;
            for (int i = 0; i < clstr.size; i++)
            {
                this->add(clstr.points->p);
            }
        }
        return *this;
    }

    Cluster::~Cluster(){

    }


    void Cluster::add(const PointPtr & pntPointer){
        // Create a pointer to a new LNode structure object, dynamically allocated on the heap
        LNode * node = new (struct LNode);
        // Update the new Node, set the PointPtr to point at the input pntPointer address
        node->p = pntPointer;
        // Since this is being added to the end up the list, update the next point of the new Node to be the null pointer
        node->next = nullptr;
        if (this->points == nullptr) {
            this->points = node;
        } else {
            LNodePtr nodeCursor = this->points;
            while (nodeCursor->next != nullptr) {
                nodeCursor = nodeCursor->next;
            }
            nodeCursor->next = node;
        }
        // Increment the size of the cluster
        this->size += 1;
    }

    const PointPtr & Cluster::remove(const PointPtr & pntPointer){
        LNode * nodeCursor = new (struct LNode);
        LNode * previousNode = new (struct LNode);
        nodeCursor = this->points;
        previousNode = this->points;
        // if the first LNode contains the point that we want to remove
        while (*nodeCursor->p != *pntPointer and nodeCursor->next != nullptr){
            nodeCursor = nodeCursor->next;
        }

        if (*nodeCursor->p == *pntPointer){
            if (*previousNode->p == *nodeCursor->p){
                this->points = this->points->next;
            }  else {
                while(previousNode->next->p != nodeCursor->p){
                    previousNode = previousNode->next;
                }
                previousNode->next = nodeCursor->next;
            }
            --(this->size);
            return pntPointer;
        } else {
            return pntPointer;
        }
    }

    Cluster & Cluster::operator+=(Point &rhs){
        Point * p = &rhs;
        this->add(p);
        return  *this;
    }

    Cluster & Cluster::operator-=(Point &rhs){
        Point * p = &rhs;
        this->remove(p);
        return *this;
    }


    // Iterate through the cluster and print the point from each LNode
    void Cluster::printCluster() {
        LNodePtr nodeCursor = this->points;
        for (int i = 0; i < this->size; ++i) {
            cout << "Point: " << i << ": " << *(*nodeCursor).p << endl;
            nodeCursor = nodeCursor->next;
        }
    }

    bool operator==(const Cluster &lhs, const Cluster &rhs){
        LNodePtr lhsNodeCursor = lhs.points;
        LNodePtr rhsNodeCursor = rhs.points;
        if(lhs.size != rhs.size){
            return false;
        } else {
            bool equal = true;
            while (equal)
                for(int i = 0; i < lhs.size; i++) {
                    for (int j = 0; j < rhs.size; j++) {
                        if (*lhsNodeCursor->p == *rhsNodeCursor->p){

                        }
                    }
                }
        }
        return false;
    }
}