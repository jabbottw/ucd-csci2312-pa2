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

            void printCluster();

            // - Members
            // TODO fix input parameters. Need to be const
            //Cluster &operator+=(const Point &rhs); // add point
            //Cluster &operator-=(const Point &rhs); // remove point
            Cluster &operator+=(Point &rhs); // add point
            Cluster &operator-=(Point &rhs); // remove point

            //Cluster &operator+=(const Cluster &rhs); // union
            //Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

            //Cluster &operator+=(const Point &rhs); // add point
            //Cluster &operator-=(const Point &rhs); // remove point

    };
}
#endif //CLUSTERING_CLUSTER_H