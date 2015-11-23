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
    const int Point::DEBUGG = 0;
    const int Point::BASE_INDEX = 1;

    Point::Point() {
        this->dimSize = 0;
        this->dimValues = nullptr;
    }

    // Constructor
    // Initializes the point class and sets the number of dimensions()
    Point::Point(int dimSize) {
        this->dimSize = dimSize;
        this->dimValues = new double[dimSize + Point::BASE_INDEX];
    }

    // Constructor
    // Initializes the point class and sets the number of dimensions()
    Point::Point(int dimSize, double *dimValues) {
        this->dimSize = dimSize;
        this->dimValues = new double[dimSize + Point::BASE_INDEX];
        for (int i = 0 + Point::BASE_INDEX; i < dimSize + Point::BASE_INDEX; ++i) {
            this->dimValues[i] = dimValues[i];
        }

    }

    // Constructor
    // Initializes the point class and sets the number of dimensions()
    // Sets each dimension equal to the default value
    Point::Point(int dimSize, int defaultVal) {
        this->dimSize = dimSize;
        this->dimValues = new double[dimSize + Point::BASE_INDEX];
        for (int i = 0 + Point::BASE_INDEX; i < dimSize + Point::BASE_INDEX; ++i) {
            this->dimValues[i] = defaultVal;
        }

    }

    // Overloaded copy constructor
    Point::Point(const Point &pnt) : dimSize(pnt.getDims()), dimValues(pnt.getAllDimensions()) { }


    // Overloaded assignment operator
    Point &Point::operator=(const Point &pnt) {
        if (this == &pnt) {
            return *this;
        }
        else {
            try {
                if (this->dimSize != pnt.getDims()) {
                    throw DimensionalityMismatchEx("PointAssignmentOp",
                            "The two points have different dimensionality", __LINE__);
                }
                delete this->dimValues;
                this->dimValues = pnt.getAllDimensions();
                this->dimValues = pnt.getDimPointer();

            } catch (DimensionalityMismatchEx dimErr) {
                if (Point::DEBUGG){
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
        //delete [] this->dimValues;
    }


    // Subscript operator
    double & Point::operator[](int i){
        double n = 0;
        try{
            if (i < 0 or i >= this->dimSize + Point::BASE_INDEX){
                throw OutOfBoundsEx("Subscript[]OpErr", "Out of Bounds Error");
            } else {
                return this->dimValues[i];
            }
        } catch (OutOfBoundsEx obErr){
            if (Point::DEBUGG){
                cout << obErr;
            }
            return n;
        }
        return n;
    }


    // Overloaded addition and subtraction operators
    // Computes a dimension wise addition and subtraction
    const Point operator+(const Point &A, const Point &B) {
        int dimSize = A.getDims();
        Point C(dimSize);
        try {
            for (int i = 0 + Point::BASE_INDEX; i < dimSize + Point::BASE_INDEX; i++) {
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
            for (int i = 0  + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; i++) {
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
            for (int i = 0 + Point::BASE_INDEX; i < dimSize + Point::BASE_INDEX; i++) {
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
            for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; i++) {
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
            for (int i = 0 + Point::BASE_INDEX; i < pnt.getDims() + Point::BASE_INDEX; i++) {
                pnt.dimValues[i] *= x;
            }
            return pnt;
        }
        catch (int e) {
            return *this;
        }
    }

    Point &Point::operator*=(double x) {
        for (int i = 0 + Point::BASE_INDEX; i < this->dimSize + Point::BASE_INDEX; i++) {
            this->dimValues[i] *= x;
        }
        return *this;
    }

    // Overloaded division operators
    // Computes a factor wise division on each dimension
    const Point Point::operator/(double x) const {
        Point pnt(*this);
        if (x) {
            try {
                for (int i = 0 + Point::BASE_INDEX; i < pnt.getDims() + Point::BASE_INDEX; i++) {
                    pnt.dimValues[i] /= x;
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
                for (int i = 0 + Point::BASE_INDEX; i < this->dimSize + Point::BASE_INDEX; i++) {
                    this->dimValues[i] /= x;
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
    // Check that point A and point B have the same dim size and dimension values
    bool operator==(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point==OpErr", "Points have different dimensionality", __LINE__);
            }
            for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; ++i) {
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

    // Check that point A and point B do not have the same dim size and dimension values
    bool operator!=(const Point &A, const Point &B) {
        try {
            if (A.getDims() != B.getDims()) {
                throw DimensionalityMismatchEx("Point!=OpErr", "Points have different dimensionality", __LINE__);
            }
            if (A.getDims() != B.getDims()) {
                return true;
            }
            for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; ++i) {
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
                for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; ++i) {
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
                for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; ++i) {
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
                for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; ++i) {
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
                for (int i = 0 + Point::BASE_INDEX; i < A.getDims() + Point::BASE_INDEX; ++i) {
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

    // Mutator methods
    // Change the values of private member variables
    void Point::setDimSize(double newDim) {
        this->dimSize = newDim;
    }

    void Point::setValue(int n, double m) {
        if (this->dimValues != nullptr) {
            try{
                if (n < 0 or n >= this->dimSize + Point::BASE_INDEX){
                    throw OutOfBoundsEx("PointSet[]OpErr", "Points have different dimensionality", __LINE__);
                }
                this->dimValues[n] = m;
            } catch (OutOfBoundsEx obErr){
                if (Point::DEBUGG){
                    cout << obErr;
                }
            }

        }
    }

    void Point::setAllDimValues(double *newDimValues) {
        if (this->dimValues == nullptr) {
            this->dimValues = new double[this->dimSize + Point::BASE_INDEX];
        }
        for (int i = 0 + Point::BASE_INDEX; i < this->dimSize + Point::BASE_INDEX; ++i) {
            this->setValue(i, newDimValues[i - Point::BASE_INDEX]);
        }
    }

    // Accessor's
    // Return the current values of private member variables
    double Point::getDims() const {
        return this->dimSize;
    }

    /*
     * Return dimension n value
     */
    double Point::getValue(int n) const {
        double dimValue;
        try {
            if (n < 0 or n >= (this->dimSize + Point::BASE_INDEX)) {
                throw OutOfBoundsEx("PointGet[]OpErr", "Out of Bounds Error");
            } else {
                dimValue = this->dimValues[n];
                return dimValue;
            }
        } catch (OutOfBoundsEx obErr){
            if (Point::DEBUGG){
                cout << obErr;
            }
            return 0;
        }
    }

    /*
     * Return a copy of all the dimension values
     */
    double * Point::getAllDimensions() const {
        double *dimValues = new double[this->dimSize + Point::BASE_INDEX];
        for (int i = 0 + Point::BASE_INDEX; i < this->dimSize + Point::BASE_INDEX; i++) {
            dimValues[i] = this->getValue(i);
        }
        return dimValues;
    }


    double *Point::getDimPointer() const {
        return this->dimValues;
    }

    ostream &operator<<(ostream &os, const Point &pnt) {
        Point p(pnt);
        int d = p.getDims();
        for (int i = 0 + Point::BASE_INDEX; i < d + Point::BASE_INDEX; ++i) {
            os << setprecision(20) << fixed << p.getValue(i);
            if (i == (d - 1 + Point::BASE_INDEX)) {
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
            pnt.setDimSize(pointDims.size());
            pnt.setAllDimValues(dimValues);
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
            double * dimDiff = new double[this->dimSize];
            double dimSum = 0;
            for (int i = 0 + Point::BASE_INDEX; i < this->dimSize + Point::BASE_INDEX; ++i) {
                dimDiff[i] = pow((B.getValue(i) - this->getValue(i)), 2);
                dimSum += dimDiff[i];
            }
            delete[] dimDiff;
            return sqrt(dimSum);
        } catch (DimensionalityMismatchEx dimErr){
            cout << dimErr;
            return 0;
        }
    }
}


