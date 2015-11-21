
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Point.h"
#include "Clustering.h"
#include "kmeans.h"
#include "DimensionalityMismatchEx.h"
#include "OutOfBoundsEx.h"
#include "RemoveFromEmptyEx.h"

using namespace std;
using namespace Clustering;

int main(void){

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
    cout << endl << "Distance: " << p1.distanceTo(p2) << endl;
    p1.setValue(10, 10);
}
