
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Point.h"
#include "Cluster.h"
#include "kmeans.h"
#include "DimensionalityMismatchEx.h"
#include "OutOfBoundsEx.h"
#include "RemoveFromEmptyEx.h"

using namespace std;
using namespace Clustering;

void test_point_exceptions();

int main(void){

    cout << endl << "line: ";
    cout.width(65);
    cout.setf(ios::left, ios::adjustfield);
    cout << "description" << " result" << endl;
    cout.width(78);
    cout.fill('~');
    cout << "~" << endl;
    cout.fill(' ');
    cout.setf(ios::right, ios::adjustfield);

    test_point_exceptions();

    return 0;

}


void test_point_exceptions(){
    double pointDims1[3] = {1,2,3};
    double pointDims2[4] = {4,5,6, 7};
    Point p1(3, pointDims1);
    Point p2(4, pointDims2);
    Cluster clstr;
    clstr.compute_centroid();
    clstr.remove(&p1);
    p1 = p2;
    p1 += p2;
    p1 -= p2;
    if (p1 == p2) {
        cout << "No error detected ==" << endl;
    }
    if(p1 != p2){
        cout << "No error detected !=" << endl;
    }
    if (p1 > p2){
        cout << "No error detected >" << endl;
    }
    if (p1 < p2){
        cout << "No error detected <" << endl;
    }
    cout << p1.distanceTo(p2) << endl;
    p1.setValue(10, 10);
}