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


We can _instantiate_, or create an instance of, our class anywhere in the rest of our code by calling any of the constructors we have defined:

```c++ 
Point myPoint;            // Calls Point::Point()
Point myOtherPoint(5, 3, 0); // Calls three-argument constructor Point::Point(double, double, double)
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
