# ucd-csci2312-pa2
Cluster Class and Point Class


/*****
 * Cluster Class and 3D point class
 * Created by Julian Abbott-Whitley 8/30/2015
 * CSCI 2312 Intermediate Programming
 * University of Colorado Denver
 * 1:00 PM T-Th
 * ***/

_working with objects_

* * *

Point class for creating and working with points in three dimensional space

Included C++ files:
<tt>Point.h</tt>
<tt>Point.cpp</tt>
<tt>Cluster.h</tt>
<tt>Cluster.cpp</tt>
<tt>pa1.cpp</tt>

Below is the source code for <tt>Point.h</tt>

```c++
// A 3-dimensional point class!
// Coordinates are double-precision floating point.
#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H
#include <iostream>

using namespace std;

namespace Clustering {

    class Point {
        private:
            int dimSize;
            double *dimValues;

        public:
            // Constructors
            Point();

            Point(int);

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
            //friend std::istream &operator>>(std::istream &, Point &);
    };
}
#endif // CLUSTERING_POINT_H
```
Below is the source code for <tt>Point.h</tt>

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
    typedef struct LNode *LNodePtr;

    struct LNode {
        PointPtr p;
        LNodePtr next;
    };

    class Cluster {
        private:
            int size;
            LNodePtr points;


        public:
            Cluster();
            // The big three: cpy ctor, overloaded operator=, dtor
            Cluster(const Cluster &);
            Cluster &operator=(const Cluster &);
            ~Cluster();

            // Set functions: They allow calling c1.add(c2.remove(p));
            void add(const PointPtr &);
            const PointPtr &remove(const PointPtr &);
            int getSize();
            LNodePtr getPoints();

            // Tester methods
            void printPointAddresses();

            // - Members
            Cluster &operator+=(const Point &rhs); // add point
            Cluster &operator-=(const Point &rhs); // remove point

            Cluster &operator+=(const Cluster &rhs); // union
            Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

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
