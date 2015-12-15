cmake_minimum_required(VERSION 3.3)
project(ucd_csci2312_pa2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



set(SOURCE_FILES main.cpp
        Point.cpp Point.h
        Cluster.cpp Cluster.h
        KMeans.cpp KMeans.h
        ErrorContext.cpp ErrorContext.h
        ClusteringTests.cpp ClusteringTests.h
        OutOfBoundsEx.cpp OutOfBoundsEx.h
        RemoveFromEmptyEx.cpp RemoveFromEmptyEx.h
        DimensionalityMismatchEx.cpp DimensionalityMismatchEx.h)
add_executable(ucd_csci2312_pa2 ${SOURCE_FILES})