//
// Created by Julian Abbott-Whitley on 10/16/15.
//

#ifndef CLUSTERING_KMEANS_H
#define CLUSTERING_KMEANS_H
#include <iostream>
#include "Clustering.h"


using namespace std;

namespace Clustering {

    typedef Cluster * ClusterPtr;

    class kmeans {

    private:
        ClusterPtr main_cluster;
        ClusterPtr * clusters;
        PointPtr * pointArray;
        int k;

    public:
        static double SCORE_DIFF_THRESHOLD;

        // constructor
        kmeans();
        kmeans(int);
        // Member Functions
        void set_k_value(int k);
        void load_main_cluster_from_file(string);
        void pick_K_point_arr();
        void show_K_points();
        void create_k_clusters();
        void load_points_into_k_clusters();
        void qc_K_clusters();

        double computeClusteringScore();


        void write_cluster_to_file(string, Cluster &);

    };


}

#endif //CLUSTERING_KMEANS_H
