# ucd-csci2312-pa3
Cluster Class and Point Class
Kmeans algorithm class development and implementation


/*****
 * KMeans Class Development
 * Cluster Class and n-dimensional point class
 * Created by Julian Abbott-Whitley 8/30/2015
 * CSCI 2312 Intermediate Programming
 * University of Colorado Denver
 * 1:00 PM T-Th
 * ***/

_working with objects_

* * *

Cluster Class
    - Contains one public inner class: Move
    Provides encapsulation of an important two-step operation, removing a point from one cluster and adding it to another 
Point class
Kmeans class
 

Included C++ files:
<tt>Point.h</tt>
<tt>Point.cpp</tt>
<tt>Cluster.h</tt>
<tt>Cluster.cpp</tt>
<tt>kmeans.h</tt>
<tt>kmeans.cpp</tt>
<tt>pa3_main.cpp</tt>

Below is the source code for <tt>Point.h</tt>

```c++
// A n-dimensional point class!
// Coordinates are double-precision floating point.
#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H


using namespace std;

namespace Clustering {

    class Point {

        static const char POINT_VALUE_DELIM;

        private:
            int dimSize;
            double *dimValues;

        public:
            // Constructors
            Point();

            Point(int);

            Point(int, int);

            Point(int, double *);

            Point(const Point &);               // Copy Constructor
            Point &operator=(const Point &);    // Assignment Operator
            ~Point();                           // Destructor

            // Mutator methods
            void setDimSize(double);
            void setDimValueN(int, double);
            void setAllDimValues(double []);


            // Accessor methods
            double getDimSize() const;
            double getDimension(int) const;
            double *getAllDimensions() const;

            double *getDimPointer() const;

            // Functions
            double distanceTo(const Point &) const;

            // Overloaded operators

            // Members
            Point &operator*=(double);

            Point &operator/=(double);

            const Point operator*(double) const;

            const Point operator/(double) const;

            //double &operator[](int index) { return values[index - 1]; } // TODO out-of-bds?

            // Friends
            friend Point &operator+=(Point &, const Point &);

            friend Point &operator-=(Point &, const Point &);

            friend const Point operator+(const Point &, const Point &);

            friend const Point operator-(const Point &, const Point &);

            friend bool operator==(const Point &, const Point &);

            friend bool operator!=(const Point &, const Point &);

            friend bool operator<(const Point &, const Point &);

            friend bool operator>(const Point &, const Point &);

            friend bool operator<=(const Point &, const Point &);

            friend bool operator>=(const Point &, const Point &);

            friend std::ostream &operator<<(ostream &, const Point &);
            friend std::istream &operator>>(std::istream &, Point &);
    };
}
#endif // CLUSTERING_POINT_H
```
Below is the source code for <tt>Cluster.h</tt>

```c++
//
// Created by Julian Abbott-Whitley on 9/20/15.
//
#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H
#include "Point.h"
#include <iostream>

using namespace std;

namespace Clustering {

    typedef Point * PointPtr;
    typedef struct LNode * LNodePtr;
    typedef unsigned int Dimension;

    struct LNode {
        PointPtr p;
        LNodePtr next;
    };

    class Cluster {

        private:
            int size;
            LNodePtr points;
            unsigned int __id;
            Point __centroid;
            bool centroidValidity;

            // Static class variables
            static const char POINT_CLUSTER_ID_DELIM;
            static unsigned int clusterCount;


        public:
            Cluster();
            // The big three: cpy ctor, overloaded operator=, dtor
            Cluster(const Cluster &);
            Cluster &operator=(const Cluster &);
            ~Cluster();

            // Set functions:
            void add(const PointPtr &);
            const PointPtr & remove(const PointPtr &);
            // Member get functions
            int getSize();
            LNodePtr getPoints();
            const unsigned int get_cluster_id();

            // Centroid member functions
            void set_centroid(const Point &);
            const Point get_centroid();
            void compute_centroid(); // Need to watch out for overloading
            void validate_centroid(int);
            bool check_centroid_validity();

            //Kmeans type functions
            void pickPoints(int k, PointPtr *pointArray);


            // Class tester methods
            void printPointAddresses();
            string clean_string(string str);
            void printCluster();

            // Overlaoded member operators
            Cluster &operator+=(const Point &rhs); // add point
            Cluster &operator-=(const Point &rhs); // remove point
            Cluster &operator+=(const Cluster &rhs); // union
            Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

            // Kmeans functions
            // This is the sum of the distances between every two points in the cluster.
            // Hint: This can be done in a double loop through the points of the cluster.
            // However, this will count every distance twice, so you need to divide the sum by 2 before returning it.
            double intraClusterDistance() const;
            // This returns the number of distinct point pairs, or edges, in a cluster.
            // (That is, every two distinct points have an imaginary edge between them.
            // Its length is the distance between the two points.) This is simply size * (size - 1) / 2,
            // where size is the size of the cluster.
            int intraClusterEdges();

            //Same thing as (intraClusterDistance), but between two clusters.
            friend double interClusterDistance(const Cluster &c1, const Cluster &c2);
            // Same thing as (getClusterEdges), but between two clusters.
            friend double interClusterEdges(const Cluster &c1, const Cluster &c2);

            // Friends
            friend bool operator==(const Cluster &lhs, const Cluster &rhs);
            friend bool operator!=(const Cluster &lhs, const Cluster &rhs);

            friend const Cluster operator+(const Cluster &lhs, const Cluster &rhs);
            friend const Cluster operator-(const Cluster &lhs, const Cluster &rhs);

            friend const Cluster operator+(const Cluster &lhs, const PointPtr &rhs);
            friend const Cluster operator-(const Cluster &lhs, const PointPtr &rhs);


            // IO
            friend std::ostream &operator<<(std::ostream &, const Cluster &);
            friend std::istream &operator>>(std::istream &, Cluster &);

            // Static Member Functions
            static unsigned int cluster_id_generator();

        /*
         * Inner nested class to move points from one cluster to another
         * Constructor: Creates a Move object which contains the point to move, the 'from' cluster and the 'to' cluster
         * Two methods used for managing the execution of moving the points
         */
        class Move {
        private:
            PointPtr ptr = nullptr;
            Cluster * fromClstr = nullptr;
            Cluster * toClstr = nullptr;

        public:
            Move(PointPtr & ptr, Cluster * from, Cluster * to){
                this->ptr = ptr;
                this->fromClstr = from;
                this->toClstr = to;
            };

            // Move point from the "From" cluster into the "To" cluster
            void perform(){
                this->toClstr->add(this->fromClstr->remove(this->ptr));
                this->invalidate_centroids();
            }

            // Invalidate the centroids for the "From" and the "To" cluster
            void invalidate_centroids(){
                this->fromClstr->validate_centroid(0);
                this->toClstr->validate_centroid(0);
            }

        };


    };
}
#endif //CLUSTERING_CLUSTER_H
```

Below is the source code for the kmeans.h file

```c++
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "Clustering.h"
#include "Point.h"
#include "kmeans.h"

using namespace std;

namespace Clustering {

    double kmeans::SCORE_DIFF_THRESHOLD = 0.005;

    // Constructors
    kmeans::kmeans() {
        this->k = 0;
        this->main_cluster = new Cluster;
        this->pointArray = nullptr;
        this->clusters = nullptr;
    }

    kmeans::kmeans(int k)  {
        this->k = k;
        this->main_cluster = new Cluster;
        this->pointArray = nullptr;
        this->clusters = nullptr;
    }


    // Kmeans clustering algorithm member functions

    // Set the class k value
    void kmeans::set_k_value(int k){
        this->k = k;
    }


    // Get k-value
    int kmeans::get_k_value() {
        return this->k;
    }


    /*
     * Load point file into main cluster
     */
    void kmeans::load_main_cluster_from_file(string fpath){
        fstream inFile;
        inFile.open(fpath);
        inFile >> *(this->main_cluster);
        inFile.close();
    }

    /*
     * write the referenced cluster to a file
     */
    void kmeans::write_cluster_to_file(string outFileName, int n){
        ofstream outFile;
        outFile.open(outFileName);
        outFile << *this->clusters[n];
        outFile.close();
    }

    /*
     * load the class pointArray with the first k points from the main_cluster
     * Used as the initial centroid for each cluster
     */
    void kmeans::pick_K_point_arr(){
        if (this->k){
            this->pointArray = new PointPtr[this->k];
            this->main_cluster->pickPoints(this->k, this->pointArray);
        }
    }

    /*
     * QC method to view the points in the class pointArray
     */
    void kmeans::show_K_points() {
        if (this->k){
            cout << endl << "*** Initial Centroid Points ***" << endl;
            for (int i = 0; i < this->k; i++){
                cout << i << ": " << *this->pointArray[i] << endl;
            }
            cout << endl << endl;
        }

    }

    /*
     * Create k clusters and set the centroid for each cluster using the points in the pointsArray
     * Load each cluster with a point from the class pointArray, since each point within the array is the cluster centroid
     * Utilizes the Cluster::Move class to simultaneously add and remove points from the main cluster
     * into each of the k clusters
     */
    void kmeans::create_k_clusters(){
        if (pointArray != nullptr){
            this->clusters = new ClusterPtr[k];
            for (int i = 0; i < k; i++) {
                clusters[i] = new Cluster;
                const Point p(*this->pointArray[i]);
                this->clusters[i]->set_centroid(p);
                Cluster::Move m(this->pointArray[i], this->main_cluster, this->clusters[i]);
                m.perform();
            }
        }
    }

    /*
     * Cycles through the main cluster and loads each point into the individual clusters
     * Points are allocated based on the minimum centroid distance
     */
    void kmeans::load_initial_points_into_k_clusters(){
        int n=0;
        // Loop through each point in the main cluster
        for (LNodePtr nodePtr = this->main_cluster->getPoints(); nodePtr != nullptr; nodePtr = nodePtr->next){
            // Get the closest centroid
            n = this->get_closest_centroid(nodePtr->p);

/*            cout << "Point Dimensions: " << *nodePtr->p << endl;
            this->show_K_points();
            cout << "Closest Centroid: " << n << endl << endl << "#########################" << endl << endl;*/

            Cluster::Move m(nodePtr->p, this->main_cluster, this->clusters[n]);
            m.perform();
        }
    }

    /*
     * Find the closest centroid for the provided point
     */
    int kmeans::get_closest_centroid(const PointPtr pointPtr){
        int n = 0;
        int mDistance, nDistance;
        // Assume closest centroid is cluster zero
        mDistance = pointPtr->distanceTo(this->clusters[0]->get_centroid());
        // Determine closest centroid
        for (int i = 0; i < this->k; i++){
            nDistance = pointPtr->distanceTo(this->clusters[i]->get_centroid());
            if (nDistance < mDistance){
                // Closest centroid is the centroid for point i
                n = i;
                // Set current min distance equal to nDistance
                mDistance = nDistance;
            }
        }
        return n;
    }

    /*
     * Debugging method to help qc the processing
     */
    void kmeans::qc_K_clusters(bool show_points) {
        if (this->clusters != nullptr) {
            cout << endl << "*** QC k Clusters ***" << endl;
            for (int i = 0; i < this->k; i++) {
                cout << "************************************************************************************" << endl << endl << endl;
                cout << "\tCluster " << i << " Size: " << this->clusters[i]->getSize() << endl;
                cout << "\t\tCentroid: " << this->clusters[i]->get_centroid() << endl;
                if (show_points){
                    cout << "** Points **" << endl;
                    for (LNodePtr nPtr = this->clusters[i]->getPoints() ; nPtr != nullptr; nPtr = nPtr->next){
                        cout << *nPtr->p << " ----- Closest Centroid: " << this->get_closest_centroid(nPtr->p) <<  endl;
                        for (int j = 0; j < this->k; j++){
                            cout << "\t\t\t Distance to centroid " << j << ": " << nPtr->p->distanceTo(this->clusters[j]->get_centroid()) << endl;
                        }
                    }
                }
            }
        }
    }

    double kmeans::computeClusteringScore(){

        double d_in_sum = 0;
        double d_out_sum = 0;
        double p_in_sum = 0;
        double p_out_sum = 0;

        double * d_in = new double[this->k];
        double * d_out = new double[this->k];
        double * p_in = new double[this->k];
        double * p_out = new double[this->k];

        double betaCV=0;

        for (int i = 0; i < this->k; i++){
            d_in[i] = this->clusters[i]->intraClusterDistance();
            p_in[i] = this->clusters[i]->intraClusterEdges();
            for (int j = 0; j < this->k; j++){
                if (i != j){
                    d_out[i] += interClusterDistance(*this->clusters[i], *this->clusters[j]);
                    p_out[i] += interClusterEdges(*this->clusters[i], *this->clusters[j]);
                }
            }
        }
        for (int i = 0; i < this->k; i++){
            d_in_sum += d_in[i];
            d_out_sum += d_out[i];
            p_in_sum += p_in[i];
            p_out_sum += p_out[i];
        }
        d_in_sum /= 2;

        betaCV = (d_in_sum/p_in_sum) / (d_out_sum/p_out_sum);
        return betaCV;
    }

    double kmeans::calculateClusterScoreDiff(double oldScore, double newScore){
        double sDiff = abs(oldScore - newScore);
        return sDiff;
    }


    void kmeans::process_kmeans(){

        double score1, score2, scoreDiff;
        int count = 0;
        // loop through all clusters
        for (int i = 0; i < this->k; i++) {
            if (!this->clusters[i]->check_centroid_validity()) {
                this->clusters[i]->compute_centroid();
                this->clusters[i]->validate_centroid(1);
            }
        }

        score1 = this->computeClusteringScore();
        scoreDiff = kmeans::SCORE_DIFF_THRESHOLD + 1;
        int closestCentroid;
        while (scoreDiff > kmeans::SCORE_DIFF_THRESHOLD) {
            cout << endl <<  "Iteration " << count << " *** start *** " << endl << endl;
            // loop through all clusters
            for (int i = 0; i < this->k; i++) {
                // loop through all points in cluster i
                for (LNodePtr nPtr = this->clusters[i]->getPoints(); nPtr != nullptr; nPtr = nPtr->next) {
                    // determine the closest centroid
                    closestCentroid = this->get_closest_centroid(nPtr->p);
                    // if centroid not of current cluster
                    // Contains a conditional check to ensure that all clusters have atleast one point
                    if (closestCentroid != i and this->clusters[i]->getSize() > 1) {
                        // perform move(point, current, other)
                        Cluster::Move m(nPtr->p, this->clusters[i], this->clusters[closestCentroid]);
                        m.perform();
                    }
                }
            }
            // loop through all clusters
            for (int i = 0; i < this->k; i++) {
                if (!this->clusters[i]->check_centroid_validity()) {
                    this->clusters[i]->compute_centroid();
                    this->clusters[i]->validate_centroid(1);
                }
            }
            score2 = this->computeClusteringScore();
            scoreDiff = abs(score1 - score2);
            cout << "Score1: " << score1 << endl;
            cout << "Score2: " << score2 << endl;
            cout << setprecision(5);
            cout << "Score diff: " << scoreDiff << endl;
            score1 = score2;
            cout << endl <<  "Iteration " << count++ << " *** end *** " << endl << endl;
        }
    }
}
```

We can _instantiate_, or create an instance of our point class anywhere in the rest of our code by calling any of the constructors we have defined:

```c++ 
Point myPoint;            // Calls Point::Point()
Point myOtherPoint(5, 3, 0); // Calls three-argument constructor Point::Point(double, double, double)
```

See code samples below for cluster class instantiation

```c++
    // Create array of values for each point dimension
    double aDimensions[3] = {5,5,5};
    // Create a new point object with the specified dimensions
    Point A(3, aDimensions);
    // Utilize the point copy constructor
    Point A_copy(A);
    // Create a cluster object
    Cluster clstr;// = new Cluster;
    // Add point A to the cluster
    clstr.add(&A);
```

Working with the kmeans class
    - The kmeans class contains the necessary class members for storing, processing, and evaluating k clusters from n points
```c++
    class kmeans {

    private:
        ClusterPtr main_cluster;                // Super cluster, used to store all of the initial points
        ClusterPtr * clusters;                  // Head pointer for dynamic allocation of k clusters on the heap
        PointPtr * pointArray;                  // Head pointer for dynamic allocation of k centroids on the heap
        int k;                                  // k value (number of clusters to create)
        static double SCORE_DIFF_THRESHOLD;     // Threshhold value to terminate looping evaluation of k clusters
    public:
        ...
    };
    
    
int main(void){

    // Open the point file and the output file
    // All initial points must be located in a comma seperated file where each line indicates a single points
    //      - Dimensions are comma delimeted
    //
    // Example File Format:
    //        1.3, 4.3, 0, 5.6, 7.9
    //        2.3, 5.6, 0, 5.6, 7.9
    //        2.4, 5.6, 0, 6.6, 7.1
    //        ...
    string inFileName = "/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data_points.txt";
    string outFileName = "/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data_clusters";

    // Specify the number of clusters to create
    int k = 4;
    // Create a kmeans class object
    kmeans km(k);
    // load all points from file location into main cluster within kmeans object
    km.load_main_cluster_from_file(inFileName);
    // select k centroids
    km.pick_K_point_arr();
    // Create k cluster objects
    km.create_k_clusters();
    // Load all points from main cluster into each individual cluster based on the closest point to centroid distance
    km.load_initial_points_into_k_clusters();
    // Run kmeans algorithm and determine final clusters
    km.process_kmeans();

    // Output each cluster file
    for (int i = 0; i < k; i++) {
        string ofName = outFileName + "_" + to_string(i) + ".txt";
        km.write_cluster_to_file(ofName, i);
    }
```



Compiler information:

```
g++ -Wall pa1.cpp Point.cpp -o pa1
```

Note:
*   <tt>g++</tt> is the GNU C++ compiler.
*   <tt>-Wall</tt> means "output ALL warnings." (If you see any warnings, be sure to fix them.)
*   Next comes the list of source files. (You don't list header files here. They are included by the source files.) The only new part here is that we have _two_ files: <tt>lab1.cc</tt> and <tt>Point.cc</tt>.
*   <tt>-o</tt> specifies the output file. In this case, we are making a program called <tt>lab1</tt>.

CLion uses CMake to create a makefile for your architecture. It uses the directives in the CMakeLists.txt file which can be found in your project's directory. The file looks like this

```
cmake_minimum_required(VERSION 3.2)
project(pa1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Point.cpp PA1.cpp)
add_executable(pa1 ${SOURCE_FILES})
```

* * *

<font size="-1">ACKNOWLEDGEMENT: Modelled after CS11 Lab 1 at Caltech.</font>

<font size="-1">Some content Copyright (C) 2004-2010, California Institute of Technology.</font>
