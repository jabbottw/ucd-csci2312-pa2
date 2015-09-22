/*****
 * Created by Ivo Georgiev on 8/25/15.
 * Edited by Julian Abbott-Whitley 8/30/2015
 * CSCI 2312 Intermediate Programming
 * University of Colorado Denver
 * 1:00 PM T-Th
 * ***/

#include <iostream>
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


    cout << "Create a new cluster and sample some of the functionality" << endl;
    Cluster * clstr = new Cluster;

    cout << "Add points A, B, C, and H to the cluster" << endl;
    clstr->add(&A);
    clstr->add(&B);
    clstr->add(&C);
    clstr->add(&H);

    cout << "Utilize the cluster copy constructor and assignment operators to create two copies of the cluster class" << endl;
    Cluster clstr2(*clstr);
    Cluster clstr3 = clstr2;
    cout << "***** Print points in Cluster 1 *****";
    clstr->printCluster();
    cout << "***** Print points in Cluster 2 *****";
    clstr2.printCluster();
    cout << "***** Print points in Cluster 3 *****";
    PointPtr p = &A;
    cout << "--- Remove point A from cluster 3 and reprint" << endl;
    clstr3.remove(p);
    clstr3.printCluster();
    cout << "Add point A back into cluster 3 using the += operator and reprint" << endl;
    clstr3 += A;
    cout << "***** Cluster Data 3 again *****";
    clstr3.printCluster();
    cout << "Remove point A from cluster 3 using the -= operator and reprint" << endl;
    clstr3 -= A;
    cout << "***** Cluster Data 3 again *****";
    clstr3.printCluster();

    return 0;
}



