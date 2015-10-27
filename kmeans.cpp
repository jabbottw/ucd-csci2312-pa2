//
// Created by Julian Abbott-Whitley on 10/16/15.
//
#include <math.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "Clustering.h"
#include "Point.h"
#include "kmeans.h"

using namespace std;

namespace Clustering {

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

    /*
     * Load point file into main cluster
     */
    void kmeans::load_main_cluster_from_file(string fpath){
        fstream inFile;
        ofstream outFile;
        inFile.open(fpath);
        inFile >> *(this->main_cluster);
        inFile.close();
    }

    /*
     * write the referenced cluster to a file
     */
    void kmeans::write_cluster_to_file(string outFileName, Cluster &clstr){
        ofstream outFile;
        outFile.open(outFileName);
        outFile << clstr;
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
            cout << endl << "*** K - points array ***" << endl;
            for (int i = 0; i < this->k; i++){
                cout << *this->pointArray[i] << endl;
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

    void kmeans::qc_K_clusters() {
        if (this->clusters != nullptr) {
            cout << endl << "*** QC k Clusters ***" << endl;
            cout << "\tMain cluster Size: " << this->main_cluster->getSize() << endl << endl;
            for (int i = 0; i < this->k; i++) {
                cout << "\tCluster " << i << " Size: " << this->clusters[i]->getSize() << endl;
                cout << "\t\tCentroid: " << this->clusters[i]->get_centroid() << endl;
                cout << "\t\tPoints: " << *this->clusters[i] << endl;
            }
        }
    }

    void kmeans::load_points_into_k_clusters(){
        int cSize = this->main_cluster->getSize() / this->k; // Number of points to load into each cluster
        int n;      // current cluster being processed
        for (int i = 0; i < cSize; i++){
            
        }
    }
}