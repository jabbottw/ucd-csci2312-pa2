//
// Created by Julian Abbott-Whitley on 10/16/15.
//
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "Clustering.h"
#include "Point.h"
#include "kmeans.h"

using namespace std;

namespace Clustering {

    double kmeans::SCORE_DIFF_THRESHOLD = 0.005;

    // Constructors
    kmeans::kmeans() {
        this->k = 0;
        this->main_cluster = new Cluster;
        this->pointArray = nullptr;
        this->clusters = nullptr;
    }

    kmeans::kmeans(int k)  {
        this->k = k;
        this->main_cluster = new Cluster;
        this->pointArray = nullptr;
        this->clusters = nullptr;
    }


    // Kmeans clustering algorithm member functions

    // Set the class k value
    void kmeans::set_k_value(int k){
        this->k = k;
    }


    // Get k-value
    int kmeans::get_k_value() {
        return this->k;
    }


    /*
     * Load point file into main cluster
     */
    void kmeans::load_main_cluster_from_file(string fpath){
        fstream inFile;
        inFile.open(fpath);
        inFile >> *(this->main_cluster);
        inFile.close();
    }

    /*
     * write the referenced cluster to a file
     */
    void kmeans::write_cluster_to_file(string outFileName, int n){
        ofstream outFile;
        outFile.open(outFileName);
        outFile << *this->clusters[n];
        outFile.close();
    }

    /*
     * load the class pointArray with the first k points from the main_cluster
     * Used as the initial centroid for each cluster
     */
    void kmeans::pick_K_point_arr(){
        if (this->k){
            this->pointArray = new PointPtr[this->k];
            this->main_cluster->pickPoints(this->k, this->pointArray);
        }
    }

    /*
     * QC method to view the points in the class pointArray
     */
    void kmeans::show_K_points() {
        if (this->k){
            cout << endl << "*** Initial Centroid Points ***" << endl;
            for (int i = 0; i < this->k; i++){
                cout << i << ": " << *this->pointArray[i] << endl;
            }
            cout << endl << endl;
        }

    }

    /*
     * Create k clusters and set the centroid for each cluster using the points in the pointsArray
     * Load each cluster with a point from the class pointArray, since each point within the array is the cluster centroid
     * Utilizes the Cluster::Move class to simultaneously add and remove points from the main cluster
     * into each of the k clusters
     */
    void kmeans::create_k_clusters(){
        if (pointArray != nullptr){
            this->clusters = new ClusterPtr[k];
            for (int i = 0; i < k; i++) {
                clusters[i] = new Cluster;
                const Point p(*this->pointArray[i]);
                this->clusters[i]->set_centroid(p);
                Cluster::Move m(this->pointArray[i], this->main_cluster, this->clusters[i]);
                m.perform();
            }
        }
    }

    /*
     * Cycles through the main cluster and loads each point into the individual clusters
     * Points are allocated based on the minimum centroid distance
     */
    void kmeans::load_initial_points_into_k_clusters(){
        int n=0;
        // Loop through each point in the main cluster
        for (LNodePtr nodePtr = this->main_cluster->getPoints(); nodePtr != nullptr; nodePtr = nodePtr->next){
            // Get the closest centroid
            n = this->get_closest_centroid(nodePtr->p);

/*            cout << "Point Dimensions: " << *nodePtr->p << endl;
            this->show_K_points();
            cout << "Closest Centroid: " << n << endl << endl << "#########################" << endl << endl;*/

            Cluster::Move m(nodePtr->p, this->main_cluster, this->clusters[n]);
            m.perform();
        }
    }

    /*
     * Find the closest centroid for the provided point
     */
    int kmeans::get_closest_centroid(const PointPtr pointPtr){
        int n = 0;
        int mDistance, nDistance;
        // Assume closest centroid is cluster zero
        mDistance = pointPtr->distanceTo(this->clusters[0]->get_centroid());
        // Determine closest centroid
        for (int i = 0; i < this->k; i++){
            nDistance = pointPtr->distanceTo(this->clusters[i]->get_centroid());
            if (nDistance < mDistance){
                // Closest centroid is the centroid for point i
                n = i;
                // Set current min distance equal to nDistance
                mDistance = nDistance;
            }
        }
        return n;
    }

    /*
     * Debugging method to help qc the processing
     */
    void kmeans::qc_K_clusters(bool show_points) {
        if (this->clusters != nullptr) {
            cout << endl << "*** QC k Clusters ***" << endl;
            for (int i = 0; i < this->k; i++) {
                cout << "************************************************************************************" << endl << endl << endl;
                cout << "\tCluster " << i << " Size: " << this->clusters[i]->getSize() << endl;
                cout << "\t\tCentroid: " << this->clusters[i]->get_centroid() << endl;
                if (show_points){
                    cout << "** Points **" << endl;
                    for (LNodePtr nPtr = this->clusters[i]->getPoints() ; nPtr != nullptr; nPtr = nPtr->next){
                        cout << *nPtr->p << " ----- Closest Centroid: " << this->get_closest_centroid(nPtr->p) <<  endl;
                        for (int j = 0; j < this->k; j++){
                            cout << "\t\t\t Distance to centroid " << j << ": " << nPtr->p->distanceTo(this->clusters[j]->get_centroid()) << endl;
                        }
                    }
                }
            }
        }
    }

    double kmeans::computeClusteringScore(){

        double d_in_sum = 0;
        double d_out_sum = 0;
        double p_in_sum = 0;
        double p_out_sum = 0;

        double * d_in = new double[this->k];
        double * d_out = new double[this->k];
        double * p_in = new double[this->k];
        double * p_out = new double[this->k];

        double betaCV=0;

        for (int i = 0; i < this->k; i++){
            d_in[i] = this->clusters[i]->intraClusterDistance();
            p_in[i] = this->clusters[i]->intraClusterEdges();
            for (int j = 0; j < this->k; j++){
                if (i != j){
                    d_out[i] += interClusterDistance(*this->clusters[i], *this->clusters[j]);
                    p_out[i] += interClusterEdges(*this->clusters[i], *this->clusters[j]);
                }
            }
        }
        for (int i = 0; i < this->k; i++){
            d_in_sum += d_in[i];
            d_out_sum += d_out[i];
            p_in_sum += p_in[i];
            p_out_sum += p_out[i];
        }
        d_in_sum /= 2;

        betaCV = (d_in_sum/p_in_sum) / (d_out_sum/p_out_sum);
        return betaCV;
    }

    double kmeans::calculateClusterScoreDiff(double oldScore, double newScore){
        double sDiff = abs(oldScore - newScore);
        return sDiff;
    }


    void kmeans::process_kmeans(){

        double score1, score2, scoreDiff;
        int count = 0;
        // loop through all clusters
        for (int i = 0; i < this->k; i++) {
            if (!this->clusters[i]->check_centroid_validity()) {
                this->clusters[i]->compute_centroid();
                this->clusters[i]->validate_centroid(1);
            }
        }

        score1 = this->computeClusteringScore();
        scoreDiff = kmeans::SCORE_DIFF_THRESHOLD + 1;
        int closestCentroid;
        while (scoreDiff > kmeans::SCORE_DIFF_THRESHOLD) {
            cout << endl <<  "Iteration " << count << " *** start *** " << endl << endl;
            // loop through all clusters
            for (int i = 0; i < this->k; i++) {
                // loop through all points in cluster i
                for (LNodePtr nPtr = this->clusters[i]->getPoints(); nPtr != nullptr; nPtr = nPtr->next) {
                    // determine the closest centroid
                    closestCentroid = this->get_closest_centroid(nPtr->p);
                    // if centroid not of current cluster
                    // Contains a conditional check to ensure that all clusters have atleast one point
                    if (closestCentroid != i and this->clusters[i]->getSize() > 1) {
                        // perform move(point, current, other)
                        Cluster::Move m(nPtr->p, this->clusters[i], this->clusters[closestCentroid]);
                        m.perform();
                    }
                }
            }
            // loop through all clusters
            for (int i = 0; i < this->k; i++) {
                if (!this->clusters[i]->check_centroid_validity()) {
                    this->clusters[i]->compute_centroid();
                    this->clusters[i]->validate_centroid(1);
                }
            }
            score2 = this->computeClusteringScore();
            scoreDiff = abs(score1 - score2);
            cout << "Score1: " << score1 << endl;
            cout << "Score2: " << score2 << endl;
            cout << setprecision(5);
            cout << "Score diff: " << scoreDiff << endl;
            score1 = score2;
            cout << endl <<  "Iteration " << count++ << " *** end *** " << endl << endl;
        }
    }
}