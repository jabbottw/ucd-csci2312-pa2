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
#include "Clustering.h"
#include "kmeans.h"

using namespace std;
using namespace Clustering;

int main(void){

    // Open point file and output file
    string inFileName = "/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data_points.txt";
    string outFileName = "/Users/julianabbott-whitley/Google_Drive/School/UCD/Current_Classes/CSCI_2312_Intermediate/ucd-csci2312-pa2/data_clusters.txt";

    int k = 5;
    kmeans km(5);
    km.load_main_cluster_from_file(inFileName);
    km.pick_K_point_arr();
    km.show_K_points();
    km.create_k_clusters();
    km.qc_K_clusters();



    return 0;

}