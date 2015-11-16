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
        static double SCORE_DIFF_THRESHOLD;

    public:

        // constructor
        kmeans();
        kmeans(int);
        // Member Functions
        // setters
        void set_k_value(int k);
        void load_main_cluster_from_file(string);
        void create_k_clusters();
        void load_initial_points_into_k_clusters();
        double calculateClusterScoreDiff(double, double);
        void process_kmeans();

        // getters
        int get_k_value();

        // member tools
        void pick_K_point_arr();
        void show_K_points();
        int get_closest_centroid(const PointPtr);
        void qc_K_clusters(bool);

        double computeClusteringScore();

        void write_cluster_to_file(string, int);

    };


}

#endif //CLUSTERING_KMEANS_H
