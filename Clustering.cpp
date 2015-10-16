//
// Created by Julian Abbott-Whitley on 9/20/15.
//

#include <iostream>
#include <string>
#include <Vector>
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
                nCursor=nCursor->next;
            }
        }
        return *this;
    }

    // Destructor
    Cluster::~Cluster(){
/*        for (LNodePtr nCursor = this->points; nCursor != nullptr; ){
            delete nCursor->p;
            LNodePtr nPrev = nCursor;
            nCursor = nCursor->next;
            delete nPrev;
        }*/
    }

    int Cluster::getSize(){
        return this->size;
    }

    LNodePtr Cluster::getPoints(){
        return this->points;
    }

    /*
     * Adds the point, referenced by the point pnt to the cluster
     * Point is inserted in lexicographical order based on the point dimensions, (i.e. lowest to highest )
     * If two points have the same dimensions, then the memory address is used to order the points
     */
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

    /*
     * Removes the referenced point from the cluster
     * Iterates through the entire cluster to make sure that
     * all instances of point address 0xABC123 are removed
     */
    const PointPtr & Cluster::remove(const PointPtr & pntPointer){
        LNodePtr nodeCursor;
        LNodePtr previousNode = nullptr;

        for (nodeCursor = this->points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next) {
            if (nodeCursor->p == pntPointer) {
                if(previousNode == nullptr){
                    this->points = this->points->next;
                    --(this->size);
                } else {
                    previousNode->next = nodeCursor->next;
                    --(this->size);
                }
            } else {
                previousNode = nodeCursor;
            }
        }
        return pntPointer;
    }

    /*
     * Adds the referenced point to the cluster
     * Creates a new point in memory on the heap
     * Can't seem to create a pointer that will reference the address location
     * of the rhs point, due to the const conditions
     * Attempted to create const PointPtrs, but the process drew an error
     */
    Cluster & Cluster::operator+=(const Point &rhs){
        PointPtr ptr = new Point(rhs);
        this->add(ptr);
        return *this;
    }

    /*
     * Removes the referenced point from the cluster
     * Iterates through the entire cluster to make sure that
     * all instances of point address 0xABC123 are removed
     */
    Cluster & Cluster::operator-=(const Point &rhs){
        LNodePtr nodeCursor;
        LNodePtr previousNode = nullptr;

        for (nodeCursor = this->points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next) {
            if (nodeCursor->p == &rhs) {
                if(previousNode == nullptr){
                    this->points = this->points->next;
                    --(this->size);
                } else {
                    previousNode->next = nodeCursor->next;
                    --(this->size);
                }
            } else {
                previousNode = nodeCursor;
            }
        }
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
        return *this;
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
        return *this;
    }

    // Cluster comparison operator
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

    // Cluster comparison operator
    bool operator!=(const Cluster &lhs, const Cluster &rhs){
        if (lhs == rhs){
            return false;
        } else {
            return true;
        }
    }

    // Cluster Union
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs){
        Cluster clstrUnion(lhs);
        clstrUnion += rhs;
        return clstrUnion;
    }

    // Asymmetric Difference
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs){
        Cluster clstrUnion(lhs);
        clstrUnion -= rhs;
        return clstrUnion;
    }

    /*
     * Create a copy cluster of the LHS cluster
     * Add the referenced point to the copy cluster
     * return the copied cluster
     */
    const Cluster operator+(const Cluster &lhs, const PointPtr &rhs){
        Cluster clstrCopy(lhs);
        clstrCopy.add(rhs);
        return clstrCopy;
    }

    /*
     * Create a copy cluster of the LHS cluster
     * Remove the referenced point from the copied cluster
     * return the copied cluster
     */
    const Cluster operator-(const Cluster &lhs, const PointPtr &rhs){
        Cluster clstrCopy(lhs);
        clstrCopy.remove(rhs);
        return clstrCopy;
    }

    /*
     * Overloaded Output Stream Operator
     * Print all of the point dimensions in the referenced cluster
     */
    ostream &operator<<(ostream &os, const Cluster &clstr){
        LNodePtr tmp = clstr.points;
        cout << "[";
        int currentPointDim;
        int nextPointDim;
        for (LNodePtr nodeCursor = clstr.points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next){
            currentPointDim = (nodeCursor->p->getDimSize());
            cout << *(nodeCursor->p);
            if (nodeCursor->next == nullptr){
                // do nothing
            } else {
                cout << ", ";
                nextPointDim = nodeCursor->next->p->getDimSize();
            }
        }
        cout << "]" << endl;
        return os;
    }

    /*
     * Overloaded input stream operator
     * Parse the input stream and insert the various points into the cluster
     * Expected file format for input Stream:
     *      Each line refers to a new point instance
     *      Dimensions 'comma delimited'
     *      The following input stream would create a cluster containing (4) 3-dimensional points
     *           1, 2, 3
     *           4, 5, 6
     *           7, 8, 9
     *           9, 9, 9
     */
    istream &operator>>(istream &is, Cluster &clstr){

        string s;
        size_t pos = 0;
        vector <double>dimVector;
        string delimiter = ",";
        while(getline(is, s)) {
            size_t pos = 0;
            string dim;
            while ((pos = s.find(delimiter)) != string::npos) {
                dim = s.substr(0, pos);
                dimVector.push_back(stod(dim));
                s.erase(0, pos + delimiter.length());
            }
            dim = s.substr(0, pos);
            try {
                double d = stod(dim);
                dimVector.push_back(d);
                double * dimValArray = new double[dimVector.size()];
                for (int i = 0; i < dimVector.size(); i++){
                    dimValArray[i] = dimVector[i];
                }
                PointPtr ptr = new Point(dimVector.size());
                ptr->setAllDimValues(dimValArray);
                clstr.add(ptr);
                dimVector.clear();
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument found within the input stream. \"Point Add\" skipped\n\n";
            } catch (const std::out_of_range&) {
                std::cerr << "Argument is out of range for a double\n";
            }

            dimVector.clear();
        }
        return is;
    }

    /*
     * Print the point address for each point in the cluster
     * Used for debugging.
     */
    void Cluster::printPointAddresses(){
        LNodePtr cursor = this->points;
        for (int i = 0; i < this->size; i++){
            cout << "Point " << i << ": " << cursor->p << endl;
            cursor = cursor->next;
        }
    }
}

