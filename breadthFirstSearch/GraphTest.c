/***
 * Morgan Stein
 * mkstein
 * 2026 Winter CSE101 PA2
 * GraphTest.c
 * Test of graph functions
 ***/

 #include "List.h"
 #include "Graph.h"

int main(void){
    Graph G = newGraph(5);
   // List L = newList();

    addEdge(G, 1, 2);
    addEdge(G, 1, 5);
    addEdge(G, 2, 3);
    addEdge(G, 3, 4);
    addArc (G, 3, 4);
    printGraph(stdout, G);

    int x = getNumArcs(G);
    int y = getNumEdges(G);
    int z = getOrder(G);
    fprintf(stdout,"Vertices: %d\nEdges: %d\nArcs: %d\n",z,y,x);


    int i = getSource(G);
    int j = getParent(G,5);
    int k = getDist(G,3);
    int l = getDist(G,5);
//    int m = getPath(L,G,5);
    fprintf(stdout,"Source: %d\nParent of 5: %d\nDistance to 3: %d\nDistance to 5: %d\n",i,j,k,l);

    BFS(G,1);
    i = getSource(G);
    j = getParent(G,5);
    k = getDist(G,3);
    l = getDist(G,5);
//     m = getPath(L,G,5);
    fprintf(stdout,"Source: %d\nParent of 5: %d\nDistance to 3: %d\nDistance to 5: %d\n",i,j,k,l);
    
    freeGraph(&G);
//    printGraph(stdout,G);
//    makeNull(G);
    printGraph(stdout,G);

}