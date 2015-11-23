/*****
 * Created by Julian Abbott-Whitley 8/30/2015
 * CSCI 2312 Intermediate Programming
 * University of Colorado Denver
 * 1:00 PM T-Th
 * ***/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Point.h"
#include "Cluster.h"
#include "kmeans.h"

using namespace std;
using namespace Clustering;

int main(void){

    // Open point file and output file
    string inFileName = "/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data_points.txt";
    string outFileName = "/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data_clusters";

    int k = 3;
    kmeans km(k);
    km.load_main_cluster_from_file(inFileName);
    km.pick_K_point_arr();
    km.create_k_clusters();

    km.load_initial_points_into_k_clusters();
    //km.process_kmeans();

    // Output each cluster file
    for (int i = 0; i < k; i++) {
        string ofName = outFileName + "_" + to_string(i) + ".txt";
        km.write_cluster_to_file(ofName, i);
    }


    return 0;

}