/*****
 * Created by Ivo Georgiev on 8/25/15.
 * Edited by Julian Abbott-Whitley 8/30/2015
 * CSCI 2312 Intermediate Programming
 * University of Colorado Denver
 * 1:00 PM T-Th
 * ***/

#include <iostream>
#include <fstream>
#include <iomanip>
#include "Point.h"
#include "Clustering.h"

using namespace std;
using namespace Clustering;


int main(void)
{
    double aDimensions[3] = {5,5,5};
    double bDimensions[3] = {10, 10, 10};
    Point A(3, aDimensions);
    Point A_copy(A);
    Point B(3, bDimensions);
    Point C(B);

    C = B / 5;
    Point D = C * 20;
    Point E = D + B;
    Point F = E - B;
    Point G(3);
    G += F;
    Point H(3);
    H -= G;
    Point X = D - B;

    cout << "*** Point A *** " << endl;
    cout << A << endl;
    cout << "*** Point B *** " << endl;
    cout << B << endl;
    cout << "*** Point C *** " << endl;
    cout << C << endl;
    cout << "*** Point D *** " << endl;
    cout << D << endl;
    cout << "*** Point E *** " << endl;
    cout << E << endl;
    cout << "*** Point F *** " << endl;
    cout << F << endl;
    cout << "*** Point G *** " << endl;
    cout << G << endl;
    cout << "*** Point H *** " << endl;
    cout << H << endl;
    cout << "*** Point X *** " << endl;
    cout << X << endl;
    cout << "Distance between points A & B: " << A.distanceTo(B) << endl;
    if (A == A_copy)
    {
        cout << "Point A is equal to A_Copy" << endl;
    }

    if (C != D)
    {
        cout << "C is not equal to D" << endl;
    }
    if (C < D)
    {
        cout << "C is less than D" << endl;
    }
    if (E > D)
    {
        cout << "E is greater than D" << endl;
    }
    if (A >= A_copy)
    {
        cout << "A is greater than or equal to A copy" << endl;
    }
    else
    {
        cout << "A is not greater than or equal to A copy" << endl;
    }
    if (A <= A_copy)
    {
        cout << "A is less than or equal to A copy" << endl;
    }
    else
    {
        cout << "A is not less than or equal to A copy" << endl;
    }


    cout << endl << "Create a new cluster and sample some of the functionality" << endl;
    Cluster clstr;// = new Cluster;
    cout << "Add points A, B, C, E, D, F, & X to the cluster" << endl;
    clstr.add(&A);
    clstr.add(&B);
    clstr.add(&C);
    clstr.add(&E);
    clstr.add(&D);
    clstr.add(&F);
    clstr.add(&X);
    cout << endl <<" *** Cluster1 Output *** " << endl;
    cout << clstr;
    cout << " *** Test remove points: remove point X[30, 30, 30] *** " << endl;
    clstr.remove(&X);
    cout << clstr;
    cout << " *** Test add points: add point X[30, 30, 30] *** " << endl;
    clstr.add(&X);
    cout << clstr;

    cout << "*** Utilize the cluster copy constructor and assignment operators to create two copies of the cluster class ***" << endl;
    Cluster clstr2(clstr);
    cout << "*** Cluster 2 *** " << endl;
    cout << clstr2;
    cout << "*** Cluster 3 ***" << endl;
    Cluster clstr3 = clstr;
    cout << clstr3;


    cout << endl << "*** Test difference and sum operators ***" << endl;

    cout << "Create clstr4, add point H to clstr4" << endl;
    Cluster clstr4;
    clstr4.add(&H);
    cout << "*** Cluster 4 ***" << endl;
    cout << clstr4;
    cout << "Test union on clstr4 with clstr3" << endl;
    clstr4 += clstr3;
    cout << "*** Cluster 4 ***" << endl;
    cout << clstr4;
    cout << "Take the asymetric difference of clusters 3 and 4, (i.e. clstr4 - clstr3)" << endl;
    clstr4 -= clstr3;
    cout << "*** Cluster 4 ***" << endl;
    cout << clstr4;

    cout << "***Test overloaded cluster + and - operators ***" << endl;
    Cluster cUnion = clstr4 + clstr3;
    cout << "*** cUnion (clstr4 + clstr3)***" << endl;
    cout << cUnion;
    Cluster clstrAsymetricDif = clstr4 - clstr3;
    cout << "*** clstrAsymetricDif (clstr4 - clstr3) ***" <<  endl;
    cout << clstrAsymetricDif;



    cout << endl << "*** Test comparitors ***" << endl;
    cout << "*** Test overloaded cluster not equal comparison operator for case when clusters are not equal ***" << endl;
    if (clstr4 != clstr3){
        cout << "Cluster 4 is not equal to cluster 3" << endl;
    } else {
        cout << "Cluster 4 is equal to cluster 3" << endl;
    }
    cout << "*** Test overloaded cluster comparison operator for case when clusters are equal ***" << endl;
    Cluster clstr3Copy(clstr3);
    cout << "*** Cluster 3 ***" << endl;
    cout << clstr3;
    cout << "*** Cluster 3 Copy ***" << endl;
    cout << clstr3Copy;
    if (clstr3Copy == clstr3){
        cout << "clstr3Copy is equal to cluster 3" << endl;
    } else {
        cout << "clstr3Copy is not equal to cluster 3" << endl;
    }

    cout << endl << "Test overloaded (clstr + point) and (clstr - point) friend operators" << endl;

    Cluster clstr5;
    clstr5.add(&H);
    clstr5.add(&H);

    cout << "Cluster5_init: " << clstr5;
    clstr5 += X;
    cout << "Cluster5_second: " << clstr5;
    Cluster cluster_sum = clstr5 + &X;
    cout << "cluster_sum: " << cluster_sum;



    cout << endl << "Cluster 5 Point Addresses" << endl;
    cluster_sum.printPointAddresses();

    cout << endl << "Point X address: " << &X << endl;
    cluster_sum -= X;
    cluster_sum -= H;
    cout << endl << "cluster_sum -= X: " << cluster_sum << endl;

    // Test overloaded isOperator
    cout << "*** Test Overloaded isOperator ***" << endl;
    Cluster isCluster;
    std::ifstream data("/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data.txt");

    data >> isCluster;
    cout << isCluster;

    return 0;
}


