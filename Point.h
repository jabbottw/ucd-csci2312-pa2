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