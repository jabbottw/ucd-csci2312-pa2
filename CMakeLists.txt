cmake_minimum_required(VERSION 3.3)
project(ucd_csci2312_pa2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        pa4_main.cpp
        Cluster.cpp
        Cluster.h
        Point.cpp
        Point.h
        ErrorContext.cpp ErrorContext.h ClusteringTests.cpp ClusteringTests.h
        kmeans.cpp kmeans.h DimensionalityMismatchEx.h DimensionalityMismatchEx.cpp DimensionalityMismatchEx.h
        OutOfBoundsEx.cpp OutOfBoundsEx.h RemoveFromEmptyEx.cpp RemoveFromEmptyEx.h MyErrorContext.cpp MyErrorContext.h)

add_executable(ucd_csci2312_pa2 ${SOURCE_FILES} DimensionalityMismatchEx.h)

