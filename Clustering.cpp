//
// Created by Julian Abbott-Whitley on 9/20/15.
//

#include <iostream>
#include <sstream>
#include <Vector>
#include "Point.h"
#include "Clustering.h"

using namespace std;

namespace Clustering {

    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';
    unsigned int Cluster::clusterCount = 0;

    Cluster::Cluster() {
        this->size = 0;
        this->points = nullptr;
        this->__id = this->cluster_id_generator();
        this->centroidValidity = false;
    }

    // Copy Constructor
    Cluster::Cluster(const Cluster & clstr) {
        if (this == &clstr){
            // do nothing
        } else {
            this->size = 0;
            __id = this->cluster_id_generator();
            bool centroidValidity = false;
            for (LNodePtr nCursor = clstr.points; nCursor != nullptr;) {
                this->add(nCursor->p);
                nCursor = nCursor->next;
            }
        }
    }

    // Overloaded assignment operator
    Cluster &Cluster::operator=(const Cluster & clstr){
        if (this == &clstr){
            return *this;
        } else {
            this->size = 0;
            __id = this->cluster_id_generator();
            bool centroidValidity = false;
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

    unsigned int Cluster::get_cluster_id(){
        return this->__id;
    }


    /*
     * Class centroids function implementations
     */
    void Cluster::set_centroid(const Point & pnt){
        this->__centroid.setDimSize(pnt.getDimSize());
        this->__centroid.setAllDimValues(pnt.getAllDimensions());
    }

    const Point Cluster::get_centroid(){
        return this->__centroid;
    }

    void Cluster::compute_centroid(){
        // Need to watch out for overloading
    }

    void Cluster::validate_centroid(int valid){
        if (valid){
            this->centroidValidity = true;
        } else {
            this->centroidValidity = false;
        }
    }

    bool Cluster::check_centroid_validity(){
        return this->centroidValidity;
    }

                /* Kmeans functions */

    /*
     * Picks k points from the initial cluster which will be used for the initial centroid locations
     * Selects and even distribution across the entire cluster, since the cluster is sorted lexicographically
     * Step size is the result of the (clusterSize - 1) / k
     * Function selects every nth point from the list where n = [stepSize + (m * StepSize)] and m = point index in the filled array
     */
    void Cluster::pickPoints(int k, PointPtr * pointArray){
        LNodePtr nodeCursor = this->points;
        int stepSize = (this->size - 1) / k;
        for (int i = 0; i < k; i++){
            for(int j = 0; j < stepSize; j++){
                nodeCursor = nodeCursor->next;
            }
            pointArray[i] = nodeCursor->p;

        }
    }


    // Kmeans functions
    // This is the sum of the distances between every two points in the cluster.
    // Hint: This can be done in a double loop through the points of the cluster.
    // However, this will count every distance twice, so you need to divide the sum by 2 before returning it.
    double Cluster::intraClusterDistance() const{

        return 0;
    }
    // This returns the number of distinct point pairs, or edges, in a cluster.
    // (That is, every two distinct points have an imaginary edge between them.
    // Its length is the distance between the two points.) This is simply size * (size - 1) / 2,
    // where size is the size of the cluster.
    int Cluster::getClusterEdges(){

        return 0;
    }

    //Same thing as (intraClusterDistance), but between two clusters.
    double interClusterDistance(const Cluster &c1, const Cluster &c2){

        return 0;
    }
    // Same thing as (getClusterEdges), but between two clusters.
    double interClusterEdges(const Cluster &c1, const Cluster &c2){

        return 0;
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
                // if point one is greater than input point, then place new point at slot zero in cluster
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
                    if ((nPrev == nullptr) and (*nPntr->p < *nCursor->p)){
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
        for (LNodePtr nodeCursor = clstr.points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next){
            os << *(nodeCursor->p);
            os << Cluster::POINT_CLUSTER_ID_DELIM << rand() % 5 << endl;
        }
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

        string inputData;
        string cleanedData;
        while(getline(is, inputData)){
            cleanedData = clstr.clean_string(inputData);
            if (cleanedData.length() != 0){
                stringstream sData(cleanedData);
                PointPtr ptr = new Point;
                sData >> *ptr;
                clstr.add(ptr);
            }
        }
        return is;
    }

    /*
     * Overloaded Output Stream Operator
     * Print all of the point dimensions in the referenced cluster
     */
    void Cluster::printCluster(){
        LNodePtr tmp = this->points;
        cout << "[";
        int currentPointDim;
        int nextPointDim;
        for (LNodePtr nodeCursor = this->points; nodeCursor != nullptr;  nodeCursor = nodeCursor->next){
            currentPointDim = (nodeCursor->p->getDimSize());
            cout << *nodeCursor->p;
            if (nodeCursor->next == nullptr){
                // do nothing
            } else {
                cout << ", ";
                nextPointDim = nodeCursor->next->p->getDimSize();
            }
        }
        cout << "]" << endl;
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

    /*
     * Clean string data from input stream
     * Remove all tabs and check for empty lines
     */
    string Cluster::clean_string(string str)
    {
        int s=str.find_first_not_of(" \t");
        int e=str.find_last_not_of(" \t");

        if (s!=-1 && e!=-1)
            return str.substr(s, e-s+1);

        return "";
    }

    // Static id generator method
    // TODO currently only increments the cluster id values
    // TODO Doesn't hand removed clusters
    unsigned int Cluster::cluster_id_generator(){
        return Cluster::clusterCount++;;
    }
}

