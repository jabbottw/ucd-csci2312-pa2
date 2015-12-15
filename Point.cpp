// Function implementation file

#include <math.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Point.h"
#include "DimensionalityMismatchEx.h"
#include "OutOfBoundsEx.h"

using namespace std;

namespace Clustering {
    // Default constructor
    // Initializes the point to (0.0, 0.0, 0.0)

    const char Point::POINT_VALUE_DELIM = ',';
    static const int DEBUGG = 0;
    static const int BASE_INDEX = 1;
    

    // Constructor
    // Initializes the point class and sets the number of dimensions()
    Point::Point(int dimSize) {
        try{
            this->dim = dimSize;
            this->values = new double[dimSize + BASE_INDEX];
            for (int i = 0 + BASE_INDEX; i < dimSize + BASE_INDEX; i++){
                this->values[i] = 0;
            }
        } catch (std::bad_alloc& ba)
        {
            //std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        }
    }

    // Constructor
    // Initializes the point class and sets the number of dimensions()
    Point::Point(int dimSize, double *dimValues) {
        this->dim = dimSize;
        this->values = new double[dimSize + BASE_INDEX];
        for (int i = 0 + BASE_INDEX; i < dimSize + BASE_INDEX; ++i) {
            this->values[i] = dimValues[i];
        }

    }

    // Overloaded copy constructor
    Point::Point(const Point &pnt){
        try{
            const Point * p = &pnt;
            this->dim = pnt.getDims();
            double *dimValues = new double[pnt.getDims() + BASE_INDEX];
            for (int i = 0 + BASE_INDEX; i < pnt.getDims() + BASE_INDEX; i++) {
                dimValues[i] = pnt.getValue(i);
            }
            this->values = dimValues;
        } catch (std::bad_alloc& ba) {
            std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        }
    }


    // Overloaded assignment operator
    Point &Point::operator=(const Point &pnt) {
        if (this == &pnt) {
            return *this;
        }
        else {
            try {
                if (this->dim != pnt.getDims()) {
                    throw DimensionalityMismatchEx("PointAssignmentOp",
                            "The two __points have different dimensionality", __LINE__);
                }
                delete this->values;

                double *dimValues = new double[pnt.getDims() + BASE_INDEX];
                for (int i = 0 + BASE_INDEX; i < pnt.getDims() + BASE_INDEX; i++) {
                    dimValues[i] = pnt.getValue(i);
                }
                this->values = dimValues;

            } catch (DimensionalityMismatchEx dimErr) {
                if (DEBUGG){
                    cout << dimErr;
                }
            }
        }
        return *this;
    }

    // Destructor
    // No dynamic allocation, so nothing to do; if omitted, generated automatically
    Point::~Point() {
        // TODO this doesn't make sense
        //delete this->values;
    }


//    // Subscript operator
//    double & Point::operator[](int i){
//        double n = 0;
//        try{
//            if (i < 0 or i >= this->dim + BASE_INDEX){
//                throw OutOfBoundsEx("Subscript[]OpErr", "Out of Bounds Error");
//            } else {
//                return this->values[i];
//            }
//        } catch (OutOfBoundsEx obErr){
//            if (DEBUGG){
//                cout << obErr;
//            }
//            return n;
//        }
//        return n;
//    }


    // Overloaded addition and subtraction operators
    // Computes a dimension wise addition and subtraction
    const Point operator+(const Point &A, const Point &B) {
        int dimSize = A.getDims();
        Point C(dimSize);
        try {
            for (int i = 0 + BASE_INDEX; i < dimSize + BASE_INDEX; i++) {
                C.setValue(i, (A.getValue(i) + B.getValue(i)));
            }
            return C;
        }
        catch (int e) {
            return C;
        }
    }

    Point &operator+=(Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()){
                throw DimensionalityMismatchEx("Point+=OpErr", "Points have different dimensionality", __LINE__);
            }
            for (int i = 0  + BASE_INDEX; i < A.getDims() + BASE_INDEX; i++) {
                A.setValue(i, A.getValue(i) + B.getValue(i));
            }
            return A;
        }
        catch (DimensionalityMismatchEx dimErr) {
            cout << dimErr;
            return A;
        }
    }

    const Point operator-(const Point &A, const Point &B) {
        int dimSize = A.getDims();
        Point C(dimSize);
        try {
            for (int i = 0 + BASE_INDEX; i < dimSize + BASE_INDEX; i++) {
                C.setValue(i, (A.getValue(i) - B.getValue(i)));
            }
            return C;
        }
        catch (int e) {
            return C;
        }
    }

    Point &operator-=(Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()){
                throw DimensionalityMismatchEx("Point-=OpErr", "Points have different dimensionality", __LINE__);
            }
            for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; i++) {
                A.setValue(i, A.getValue(i) - B.getValue(i));
            }
            return A;
        }
        catch (DimensionalityMismatchEx dimErr) {
            cout << dimErr;
            return A;
        }
    }

    // Overloaded multiplication operators
    // Computes a factor wise multiplication on each dimension
    const Point Point::operator*(double x) const {
        Point pnt(*this);
        try {
            for (int i = 0 + BASE_INDEX; i < pnt.getDims() + BASE_INDEX; i++) {
                pnt.values[i] *= x;
            }
            return pnt;
        }
        catch (int e) {
            return *this;
        }
    }

    Point &Point::operator*=(double x) {
        for (int i = 0 + BASE_INDEX; i < this->dim + BASE_INDEX; i++) {
            this->values[i] *= x;
        }
        return *this;
    }

    // Overloaded division operators
    // Computes a factor wise division on each dimension
    const Point Point::operator/(double x) const {
        Point pnt(*this);
        if (x) {
            try {
                for (int i = 0 + BASE_INDEX; i < pnt.getDims() + BASE_INDEX; i++) {
                    pnt.values[i] /= x;
                }
                return pnt;
            }
            catch (int e) {
                return *this;
            }
        }
        return *this;
    }

    Point &Point::operator/=(double x) {
        if (x) {
            try {
                for (int i = 0 + BASE_INDEX; i < this->dim + BASE_INDEX; i++) {
                    this->values[i] /= x;
                }
                return *this;
            }
            catch (int e) {
                return *this;
            }
        }
        return *this;
    }

    // Overloaded comparison operators
    // Check that point A and point B have the same dim __size and dimension values
    bool operator==(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point==OpErr", "Points have different dimensionality", __LINE__);
            }
            for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; ++i) {
                if (A.getValue(i) != B.getValue(i)) {
                    return false;
                }
            }
            return true;
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return false;
        }
    }

    // Check that point A and point B do not have the same dim __size and dimension values
    bool operator!=(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point!=OpErr", "Points have different dimensionality", __LINE__);
            }
            if (A.getDims() != B.getDims()) {
                return true;
            }
            for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; ++i) {
                if (A.getValue(i) != B.getValue(i)) {
                    return true;
                }
            }
            return false;
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return false;
        }
    }

    // Check if the dimensions values of point A are less than that of point B, (assumes lexicographic order)
    bool operator<(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point<OpErr", "Points have different dimensionality", __LINE__);
            }
            if (A.getDims() == B.getDims()) {
                for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; ++i) {
                    if ((A.getValue(i) == B.getValue(i))) {
                        // check next dim
                    }
                    else if (A.getValue(i) < B.getValue(i)) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
            return false;
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return false;
        }
    }

    // Check if the dimensions values of point A are greater than that of point B, (assumes lexicographic order)
    bool operator>(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point>OpErr", "Points have different dimensionality", __LINE__);
            }
            if (A.getDims() == B.getDims()) {
                for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; ++i) {
                    if (A.getValue(i) > B.getValue(i)) {
                        return true;
                    }
                }
                return false;
            }
            return false;
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return false;
        }
    }

    // Check if the dimensions values of point A are greater than or equal to that of point B, (assumes lexicographic order)
    bool operator>=(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point>=OpErr", "Points have different dimensionality");
            }
            if (A.getDims() == B.getDims()) {
                for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; ++i) {
                    if (A.getValue(i) < B.getValue(i)) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return false;
        }
    }

    // Check if the dimensions values of point A are greater than that of point B, (assumes lexicographic order)
    bool operator<=(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point<=OpErr", "Points have different dimensionality");
            }
            if (A.getDims() == B.getDims()) {
                for (int i = 0 + BASE_INDEX; i < A.getDims() + BASE_INDEX; ++i) {
                    if (A.getValue(i) > B.getValue(i)) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return false;
        }
    }


    void Point::setValue(int n, double m) {
        if (this->values != nullptr) {
            try{
                if (n < 1 or n >= this->dim + BASE_INDEX){
                    throw OutOfBoundsEx("PointSet[]OpErr", "Points have different dimensionality", __LINE__);
                }
                this->values[n] = m;
            } catch (OutOfBoundsEx obErr){
                if (DEBUGG){
                    cout << obErr;
                }
            }

        }
    }

    // Accessor's

    /*
     * Return dimension n value
     */
    double Point::getValue(int n) const {
        double dimValue;
        try {
            if (n < 1 or n >= (this->dim + BASE_INDEX)) {
                throw OutOfBoundsEx("PointGet[]OpErr", "Out of Bounds Error");
            } else {
                dimValue = this->values[n];
                return dimValue;
            }
        } catch (OutOfBoundsEx obErr){
            if (DEBUGG){
                cout << obErr;
            }
            return 0;
        }
    }


    ostream &operator<<(ostream &os, const Point &pnt) {
        Point p(pnt);
        int d = p.getDims();
        for (int i = 0 + BASE_INDEX; i < d + BASE_INDEX; ++i) {
            os << setprecision(20) << fixed << p.getValue(i);
            if (i == (d - 1 + BASE_INDEX)) {
                //do nothing
            } else {
                os << Point::POINT_VALUE_DELIM;
            }
        }
        return os;
    }

    istream &operator>>(std::istream & is, Point &pnt) {
        string dimN;
        vector<double> pointDims;
        while(getline(is, dimN, ',')){
            pointDims.push_back(stod(dimN));
        }
        try {
            if (pointDims.size() != pnt.getDims()){
                throw DimensionalityMismatchEx("Point>>OpErr", "Points have different dimensionality");
            }
            double *dimValues = &pointDims[0];
            pnt.dim = pointDims.size();
            //pnt.setAllDimValues(dimValues);
            for (int i = 1; i < pnt.dim + BASE_INDEX; ++i) {
                pnt.setValue(i, dimValues[i - BASE_INDEX]);
            }
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
        }
        return is;
    }


    // Auxiliary methods
    double Point::distanceTo(const Point &B) const {
        try {
            if (this->getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("PointDistanceToOpErr", "Points have different dimensionality", __LINE__);
            }
            double * dimDiff = new double[this->dim];
            double dimSum = 0;
            for (int i = 0 + BASE_INDEX; i < this->dim + BASE_INDEX; ++i) {
                dimDiff[i] = pow((B.getValue(i) - this->getValue(i)), 2);
                dimSum += dimDiff[i];
            }
            return sqrt(dimSum);
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return 0;
        }
    }
}


