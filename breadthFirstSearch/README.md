/***
 * Morgan Stein
 * mkstein
 * 2026 Winter CSE101 PA2
 * README
 * README describing each file 
 ***/

List.h ---------------------------------------------------------------------------------------------

List.h is the header file for List.c. It contains all of the functions defined in List.c which are then used by ListTest.c and Words.c

List.c ---------------------------------------------------------------------------------------------

List.c takes all of the functions outlined in List.h and defines them so they can be used in other files.

Graph.h ---------------------------------------------------------------------------------------------

Graph.h outlines all of the graph functions so they can be defined in Graph.c.

Graph.c ---------------------------------------------------------------------------------------------

Graph.c outlines defines of the graph functions so they can be defined in other files such as GraphTest.c and FindPath.c

GraphTest.c ------------------------------------------------------------------------------------------

GraphTest.c is a test of all of the graph functions. It should give you "GRAPH ERROR: printGraph(): NULL Graph reference" at the end because I wanted to test printGraph()'s response to being asked to print an empty graph.

FindPath.c -------------------------------------------------------------------------------------------

FindPath.c uses the graph functions to take an input file of a graph and output a file containint its adgacency list as well as the distance between the beginning and target vertices defined in the input file.

Makefile ---------------------------------------------------------------------------------------------

Makefile for this project that builds Graph.o, GraphTest.o, FindPath.o, and GraphTest.o