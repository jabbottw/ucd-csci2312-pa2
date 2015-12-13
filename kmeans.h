//
// Created by Ivo Georgiev on 11/22/15.
//

#ifndef CLUSTERING_KMEANS_H
#define CLUSTERING_KMEANS_H

#include <string>
#include <fstream>

#include "Cluster.h"

namespace Clustering {

    class KMeans {
        unsigned int __dimensionality;
        unsigned int __k;
        Cluster **__clusters;
        Point **__initCentroids;
        std::string __iFileName;
        double __score;

    public:
        // TODO this should be a constructor parameter instead of a static
        static double SCORE_DIFF_THRESHOLD;
        static void setThreshold(double t) { KMeans::SCORE_DIFF_THRESHOLD = t; }
        static double getThreshold() { return KMeans::SCORE_DIFF_THRESHOLD; }

        // TODO this should be a constructor parameter instead of a static
        static int MAX_ITER;
        static void setMaxIter(int i) { KMeans::MAX_ITER = i; }
        static int getMaxIter() { return KMeans::MAX_ITER; }

        KMeans(unsigned int dim, unsigned int k, std::string filename) :
                __dimensionality(dim),
                __k(k), // TODO exception on k=0 (undefined behavior)
                __clusters(new Cluster *[__k]),
                __initCentroids(new Point *[__k]),
                __iFileName(filename),
                __score(std::numeric_limits<double>::max())
        {
            // create k clusters
            for (int i = 0; i < __k; i++)
                __clusters[i] = new Cluster(__dimensionality);

            // load the datafile
            if (__iFileName != "") {
                std::ifstream csv(__iFileName);
                if (csv.is_open()) {    // TODO exception on failure
                    csv >> *__clusters[0];
                    csv.close();
                }
            }

            // pick k points as initial centroids
            __clusters[0]->pickPoints(__k, __initCentroids);

            // initialize k centroids
            for (int i = 0; i < __k; i++)
                __clusters[i]->setCentroid(*__initCentroids[i]);
        }

        // no copying
        KMeans(const KMeans &) = delete;
        KMeans &operator=(const KMeans &) = delete;

        ~KMeans();

        // element access, mostly for testing
        Cluster &operator[](unsigned int u);
        const Cluster &operator[](unsigned int u) const;
        int get_closest_centroid(const PointPtr);

        // write out the results
        friend std::ostream &operator<<(std::ostream &os, const KMeans &kmeans);

        // clustering functions
        void run();
        void computeClusteringScore(); // Beta-CV
        double getScore() const { return __score; }
    };

}


#endif //CLUSTERING_KMEANS_H
