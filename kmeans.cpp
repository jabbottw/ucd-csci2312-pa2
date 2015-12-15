//
// Created by Julian Abbott-Whitley on 10/16/15.
//
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "Cluster.h"
#include "Point.h"
#include "KMeans.h"

using namespace std;

namespace Clustering {

    typedef Cluster * ClusterPtr;
    double KMeans::SCORE_DIFF_THRESHOLD = 0.005;


    KMeans::~KMeans() {


    }


    // Kmeans clustering algorithm member functions



    /*
     * Find the closest centroid for the provided point
     */
    int get_closest_centroid(Cluster ** clstr, int k, const PointPtr pointPtr){
        int n = 0;
        int mDistance, nDistance;
        // Assume closest centroid is cluster zero
        mDistance = pointPtr->distanceTo(clstr[0]->getCentroid());
        // Determine closest centroid
        for (int i = 0; i < k; i++){
            nDistance = pointPtr->distanceTo(clstr[i]->getCentroid());
            if (nDistance < mDistance){
                // Closest centroid is the centroid for point i
                n = i;
                // Set current min distance equal to nDistance
                mDistance = nDistance;
            }
        }
        return n;
    }

//    /*
//     * Debugging method to help qc the processing
//     */
//    void KMeans::qc_K_clusters(bool show_points) {
//        if (this->__clusters != nullptr) {
//            cout << endl << "*** QC k Clusters ***" << endl;
//            for (int i = 0; i < this->__k; i++) {
//                cout << "************************************************************************************" << endl << endl << endl;
//                cout << "\tCluster " << i << " Size: " << this->__clusters[i]->getSize() << endl;
//                cout << "\t\tCentroid: " << this->__clusters[i]->getCentroid() << endl;
//                if (show_points){
//                    cout << "** Points **" << endl;
//                    for (LNodePtr nPtr = this->__clusters[i]->getPoints() ; nPtr != nullptr; nPtr = nPtr->next){
//                        cout << *nPtr->p << " ----- Closest Centroid: " << this->get_closest_centroid(nPtr->p) <<  endl;
//                        for (int j = 0; j < this->__k; j++){
//                            cout << "\t\t\t Distance to centroid " << j << ": " << nPtr->p->distanceTo(
//                                    this->__clusters[j]->getCentroid()) << endl;
//                        }
//                    }
//                }
//            }
//        }
//    }

    void KMeans::computeClusteringScore(){

        double d_in_sum = 0;
        double d_out_sum = 0;
        double p_in_sum = 0;
        double p_out_sum = 0;

        double * d_in = new double[this->__k];
        double * d_out = new double[this->__k];
        double * p_in = new double[this->__k];
        double * p_out = new double[this->__k];

        double betaCV=0;

        for (int i = 0; i < this->__k; i++){
            d_in[i] = this->__clusters[i]->intraClusterDistance();
            p_in[i] = this->__clusters[i]->getClusterEdges();
            for (int j = 0; j < this->__k; j++){
                if (i != j){
                    d_out[i] += interClusterDistance(*this->__clusters[i], *this->__clusters[j]);
                    p_out[i] += interClusterEdges(*this->__clusters[i], *this->__clusters[j]);
                }
            }
        }
        for (int i = 0; i < this->__k; i++){
            d_in_sum += d_in[i];
            d_out_sum += d_out[i];
            p_in_sum += p_in[i];
            p_out_sum += p_out[i];
        }
        d_in_sum /= 2;
        if (d_in_sum == 0){
            betaCV = 0;
        } else if (p_out_sum == 0){
            betaCV = std::numeric_limits<double>::max();
        } else {
            betaCV = (d_in_sum/p_in_sum) / (d_out_sum/p_out_sum);
        }
        this->__score = betaCV;
    }



    void KMeans::run(){

        double score1, score2, scoreDiff;
        int closestCentroid = 0;
//        // loop through all __clusters
//        for (int i = 0; i < this->__k; i++) {
//            this->__clusters[i]->computeCentroid();
//        }
//        this->computeClusteringScore();
        score1 = this->__score;
        scoreDiff = KMeans::SCORE_DIFF_THRESHOLD + 1;
        while (scoreDiff > KMeans::SCORE_DIFF_THRESHOLD) {
            // loop through all __clusters
            for (int i = 0; i < this->__k; i++) {
                // loop through all __points in cluster i
                // Loop through every point in cluster i
                for (int j = 0; j < this->__clusters[i]->getSize(); j++){
                    PointPtr ptr = (*this->__clusters[i])[j];
                    // determine the closest centroid
                    closestCentroid = get_closest_centroid(this->__clusters, this->__k, ptr);
                    // if centroid is not of current cluster
                    // Contains a conditional check to ensure that all __clusters have at least one point
                    if (closestCentroid != i ) { //and this->__clusters[i]->getSize() > 1
                        // perform move(point, current, other)
                        Cluster::Move m(ptr, this->__clusters[i], this->__clusters[closestCentroid]);
                        m.perform();
                    }
                }
            }
            // loop through all __clusters
            for (int i = 0; i < this->__k; i++) {
                if (!this->__clusters[i]->isCentroidValid()) {
                    this->__clusters[i]->computeCentroid();
                }
            }
            this->computeClusteringScore();
            score2 = this->__score;
            scoreDiff = abs(score1 - score2);
//            cout << "Score1: " << score1 << endl;
//            cout << "Score2: " << score2 << endl;
//            cout << setprecision(5);
//            cout << "Score diff: " << scoreDiff << endl;
            score1 = score2;
//            cout << endl <<  "Iteration " << count++ << " *** end *** " << endl << endl;
        }
    }


    Cluster & KMeans::operator[](unsigned int u){
        return *this->__clusters[u];
    }

    const Cluster & KMeans::operator[](unsigned int u) const{
        return *this->__clusters[u];
    }

    ostream &operator<<(ostream &os, const KMeans &kmeans){
        for (int i = 0; i < kmeans.__k; i++){
            os << *(kmeans).__clusters[i];
        }
        return os;
    }
}