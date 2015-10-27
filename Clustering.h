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
            unsigned int get_cluster_id();

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
            int getClusterEdges();

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