//
// Created by Julian Abbott-Whitley on 9/20/15.
//

#include <iostream>
#include <sstream>
#include <Vector>
#include "Point.h"
#include "Cluster.h"
#include "RemoveFromEmptyEx.h"
#include "OutOfBoundsEx.h"

using namespace std;

namespace Clustering {

    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';


    // Copy Constructor
    Cluster::Cluster(const Cluster & clstr) : __centroid(clstr.getDimensionality()) {
        if (this == &clstr){
            // do nothing
        } else {
            this->__size = 0;
            this->__dimensionality = clstr.__dimensionality;
            this->__points = nullptr;
            this->__release_points = clstr.__release_points;
            this->__greedy = clstr.__greedy;
            this->__id = 0;
            // TODO NEED to fix the idGen
            //__id = Cluster::__idGenerator++;
            for (LNodePtr nCursor = clstr.__points; nCursor != nullptr; nCursor = nCursor->next) {
                this->add(nCursor->p);
            }
        }
    }

    // Overloaded assignment operator
    Cluster &Cluster::operator=(const Cluster & clstr){
        if (this == &clstr){
            return *this;
        } else {
            this->__size = 0;
            __id = 0; //this->__idGenerator++;
            for (LNodePtr nCursor = this->__points; nCursor != nullptr; ) {
                delete nCursor->p;
                LNodePtr nPrev = nCursor;
                nCursor=nCursor->next;
                delete nPrev;
            }
            for (int i = 0; i < clstr.getSize(); i++){
                this->add(clstr[i]);
            }
        }
        return *this;
    }

    // Destructor
    Cluster::~Cluster(){
//        for (LNodePtr nCursor = this->__points; nCursor != nullptr; ){
//            delete nCursor->p;
//            nCursor = nCursor->next;
//        }
    }


    /*
     * Class centroids function implementations
     */
    void Cluster::computeCentroid(){
        PointPtr centroid = new Point(this->__dimensionality, 0.0);
        int count = 0;
        int tLoops = 0;
        for (LNodePtr nPtr = this->__points; nPtr != nullptr; nPtr = nPtr->next) {
            *centroid += *nPtr->p;
        }
        *centroid /= this->getSize();
        this->setCentroid(*centroid);
    }

                /* Kmeans functions */

    /*
     * Picks k __points from the initial cluster which will be used for the initial centroid locations
     * Selects and even distribution across the entire cluster, since the cluster is sorted lexicographically
     * Step __size is the result of the (clusterSize - 1) / k
     * Function selects every nth point from the list where n = [stepSize + (m * StepSize)] and m = point index in the filled array
     */
    void Cluster::pickPoints(unsigned int k, PointPtr * pointArray){
        LNodePtr nCursor = this->__points;
        for (int i = 0; i < k; i++){
            if (nCursor != nullptr){
                pointArray[i] = nCursor->p;
                nCursor = nCursor->next;
            } else {
                Point * p = new Point(this->__dimensionality, std::numeric_limits<double>::max());
                pointArray[i] = p;
            }
        };
    }


    // Kmeans functions
    // This is the sum of the distances between every two __points in the cluster.
    // Hint: This can be done in a double loop through the __points of the cluster.
    // However, this will count every distance twice, so you need to divide the sum by 2 before returning it.
    double Cluster::intraClusterDistance() const{
        double sum = 0;
        for (LNodePtr nPtr0 = this->__points; nPtr0 != nullptr; nPtr0 = nPtr0->next){
            for (LNodePtr nPtr1 = this->__points; nPtr1 != nullptr; nPtr1 = nPtr1->next){
                sum += nPtr0->p->distanceTo(*nPtr1->p);
            }
        }
        return (sum / 2.0);
    }
    // This returns the number of distinct point pairs, or edges, in a cluster.
    // (That is, every two distinct __points have an imaginary edge between them.
    // Its length is the distance between the two __points.) This is simply __size * (__size - 1) / 2,
    // where __size is the __size of the cluster.
    int Cluster::getClusterEdges() const{
        return ((this->__size * (this->__size - 1)) / 2);
    }

    //Same thing as (intraClusterDistance), but between two __clusters.
    double interClusterDistance(const Cluster &c1, const Cluster &c2){
        Cluster clstrA(c1);
        Cluster clstrB(c2);
        double sum = 0;
        for (LNodePtr nPtr0 = clstrA.__points; nPtr0 != nullptr; nPtr0 = nPtr0->next){
            for (LNodePtr nPtr1 = clstrB.__points; nPtr1 != nullptr; nPtr1 = nPtr1->next){
                sum += nPtr0->p->distanceTo(*nPtr1->p);
            }
        }
        if (clstrA.__size > 1){
            sum /= 2;
        }
        return sum;
    }
    // Same thing as (getClusterEdges), but between two __clusters.
    int interClusterEdges(const Cluster &c1, const Cluster &c2){
        Cluster clstrA(c1);
        Cluster clstrB(c2);
        double edges = clstrA.getSize() * clstrB.getSize();
        return edges;
    }

    /*
     * Adds the point, referenced by the point pnt to the cluster
     * Point is inserted in lexicographical order based on the point dimensions, (i.e. lowest to highest )
     * If two __points have the same dimensions, then the memory address is used to order the __points
     */
    void Cluster::add(const PointPtr & pntPointer) {
        // Create a pointer to a new LNode structure object, dynamically allocated on the heap
        LNodePtr nPntr = new LNode(nullptr, nullptr);
        // Update the new Node, set the PointPtr to point at the input pntPointer address
        nPntr->p = pntPointer;
        nPntr->next = nullptr;
        //LNodePtr nPrevious;
        LNodePtr nCursor;
        LNodePtr nPrev;
        // Find the correct spot in the list to insert the __points
        // Inserting __points in lexigraphical order
        switch (this->__size) {
            // The current list is empty, insert the node at the start
            case 0:
                this->__points = nPntr;
                this->__points->next = nullptr;
                break;
            // If there is only one node, just determine the correct spot for the new node, (i.e. 1st or 2nd)
            case 1:
                // if point one is greater than input point, then place new point at slot zero in cluster
                if (*this->__points->p > *nPntr->p){
                    nCursor = this->__points;
                    this->__points = nPntr;
                    this->__points->next = nCursor;
                } else {
                    this->__points->next = nPntr;
                }
                break;
            default:
                nPrev = this->__points;
                // Iterate through the linked list and place point appropriately
                for (nCursor = this->__points->next; nCursor != nullptr; nCursor = nCursor->next){
                    // If input point is smaller than (1st point), place point at beginning of list
                    if (*nPntr->p < *this->__points->p){
                        nPntr->next = nPrev;
                        this->__points = nPntr;
                        break;
                    // If the input point is less than current point, insert point before cursor
                    } else if (*nPntr->p < *nCursor->p ) {
                        nPrev->next = nPntr;
                        nPntr->next = nCursor;
                        break;
                    // If the input point is equal to the current point, insert point before cursor
                    } else if (*nPntr->p == *nCursor->p) {
                        // Make sure that point addresses are ordered in ascending order as well
                        if (nPntr->p < nCursor->p) {
                            // Place input point before cursor
                            nPrev->next = nPntr;
                            nPntr->next = nCursor;
                        } else {
                            // Place input point after cursor
                            nPntr->next = nCursor->next;
                            nCursor->next = nPntr;
                        }
                        break;
                    // Continue looping
                    } else {
                        nPrev = nPrev->next;
                    }

                }
                // If we made it through the whole list, then the input point goes at the end
                if (nCursor == nullptr){
                    nPrev->next = nPntr;
                }
        }
        // Increment the __size of the cluster and set centroid to invalid
        this->__size += 1;
        this->invalidateCentroid();
    }

    /*
     * Removes the referenced point from the cluster
     * Iterates through the entire cluster to make sure that
     * all instances of point address 0xABC123 are removed
     */
    const PointPtr & Cluster::remove(const PointPtr & pntPointer){
        LNodePtr nodeCursor;
        LNodePtr previousNode = nullptr;
        try {
            if (this->__points == nullptr or this->__size == 0){
                throw RemoveFromEmptyEx("Remove_from_EmptyClusterErr", "Nullpointer or empty cluster", __LINE__);
            }
            for (nodeCursor = this->__points; nodeCursor != nullptr; nodeCursor = nodeCursor->next) {
                if (nodeCursor->p == pntPointer) {
                    if (previousNode == nullptr) {
                        this->__points = this->__points->next;
                        --(this->__size);
                        break;
                    } else {
                        previousNode->next = nodeCursor->next;
                        --(this->__size);
                        break;
                    }
                } else {
                    previousNode = nodeCursor;
                }
            }
            this->invalidateCentroid();
            return pntPointer;
        } catch (RemoveFromEmptyEx rmErr){
            cout << rmErr;
        }
        return pntPointer;
    }

    bool Cluster::contains(const PointPtr &ptr) const{
        for (LNodePtr nodeCursor = this->__points; nodeCursor != nullptr; nodeCursor = nodeCursor->next) {
            if (nodeCursor->p == ptr){
                return true;
            }
        }
        return false;
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
        for (LNodePtr nodeCursor = this->__points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next) {
            if (*nodeCursor->p == rhs){
                this->remove(nodeCursor->p);
                break;
            }
        }
        return *this;
    }



    // Union of the two __clusters
    Cluster & Cluster::operator+=(const Cluster &rhs){
        if (this->__size == 0){
            for (LNodePtr rhsNodeCursor = rhs.__points; rhsNodeCursor != nullptr; rhsNodeCursor = rhsNodeCursor->next) {
                this->add(rhsNodeCursor->p);
            }
        } else {
            // iterate through all the nodes within the RHS cluster
            for (LNodePtr rhsNodeCursor = rhs.__points; rhsNodeCursor != nullptr; rhsNodeCursor = rhsNodeCursor->next){
                bool addPoint = true;
                for (LNodePtr thisNodeCursor = this->__points; thisNodeCursor != nullptr; thisNodeCursor = thisNodeCursor->next){
                    if (thisNodeCursor->p == rhsNodeCursor->p){
                        addPoint = false;
                    }
                }
                if (addPoint){
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
        for(LNodePtr rhsNodeCursor = rhs.__points; rhsNodeCursor != nullptr; rhsNodeCursor = rhsNodeCursor->next){
            thisNodeCursor = this->__points;
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


    const PointPtr & Cluster::operator[](unsigned int u) const{
        LNodePtr nCursor = this->__points;
        for (int i = 0; i < u; i++) {
            nCursor = nCursor->next;
        }
        return nCursor->p;
    }

    // Cluster comparison operator
    bool operator==(const Cluster &lhs, const Cluster &rhs){
        if(lhs.__size != rhs.__size) {
            return false;
        } else {
            LNodePtr rhsCursor = rhs.__points;
            for (LNodePtr lhsCursor = lhs.__points; lhsCursor != nullptr;  lhsCursor = lhsCursor->next){
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
        LNodePtr tmp = clstr.__points;
        unsigned int cluster_id = clstr.__id;
        for (LNodePtr nodeCursor = clstr.__points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next){
            os << *(nodeCursor->p);
            os << Cluster::POINT_CLUSTER_ID_DELIM << cluster_id - 1 << endl;
        }
        return os;
    }

    /*
     * Overloaded input stream operator
     * Parse the input stream and insert the various __points into the cluster
     * Expected file format for input Stream:
     *      Each line refers to a new point instance
     *      Dimensions 'comma delimited'
     *      The following input stream would create a cluster containing (4) 3-dimensional __points
     *           1, 2, 3
     *           4, 5, 6
     *           7, 8, 9
     *           9, 9, 9
     */
    istream &operator>>(istream &is, Cluster &clstr){
        int pntCount;
        string inputData;
        string cleanedData;
        while(getline(is, inputData)){
            int s = inputData.find_first_not_of(" \t");
            int e = inputData.find_last_not_of(" \t");

            if (s != -1 && e != -1) {
                pntCount = std::count(inputData.begin(), inputData.end(), ',') + 1;
                cleanedData = inputData.substr(s, e - s + 1);
            } else {
                cleanedData = "";
            }
            if (cleanedData.length() != 0){
                stringstream sData(cleanedData);
                PointPtr ptr = new Point(pntCount);
                sData >> *ptr;
                clstr.add(ptr);
            }
        }
        return is;
    }
}

