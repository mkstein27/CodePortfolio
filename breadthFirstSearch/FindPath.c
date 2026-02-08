/***
 * Morgan Stein
 * mkstein
 * 2026 Winter CSE101 PA2
 * FindPath.c
 * Implementation of graph functions
***/

#include <stdio.h>
#include<stdlib.h>
#include "List.h"
#include "Graph.h"

#define white 0
#define grey 1
#define black 2
#define INF -1
#define NIL -2

int main(int fileCheck, char* fileRead[]){
    if (fileCheck != 3){
        fprintf(stderr, "Incorrect number of command line arguments\n");
        exit(EXIT_FAILURE);
    }

    FILE* openFile = fopen(fileRead[1], "r");
    if (openFile == NULL){
        fprintf(stderr, "Error: NULL file 1");
        exit(EXIT_FAILURE);
    }

    FILE* closeFile = fopen(fileRead[2], "w");
    if (closeFile == NULL){
        fprintf(stderr, "Error: NULL file 2");
        exit(EXIT_FAILURE);
    }

    int n;
    fscanf(openFile,"%d",&n);
    Graph G = newGraph(n);

    int u;
    int v;
    while(fscanf(openFile,"%d%d",&u,&v)==2){
        if(u==0&&v==0){
            break;
        }else{
            addEdge(G,u,v);
        }
    }

    printGraph(closeFile,G);
    fprintf(closeFile,"\n"); 

    while(fscanf(openFile,"%d%d",&u,&v)==2){
        if(u==0&&v==0){
            break;
        }else{
            BFS(G,u);
            int dist = getDist(G,v);
            if(dist==INF){
                fprintf(closeFile, "The distance from %d to %d is infinity\n", u,v);
                fprintf(closeFile, "No %d-%d path exists\n",u,v);
            }else{
                List P = newList();
                getPath(P,G,v);
                fprintf(closeFile, "The distance from %d to %d is %d\n", u,v,dist);
                fprintf(closeFile, "A shortest %d-%d path is:(",u,v);
                for (moveFront(P); position(P)>=0; moveNext(P)){
                    int x = get(P);
                    fprintf(closeFile, "%d", x);
                    if(position(P)<length(P)-1){
                        fprintf(closeFile, ", ");
                    }
                    
                }
                fprintf(closeFile,")\n\n");
                freeList(&P);
            }
        }
    }

    freeGraph(&G);
    fclose(openFile);
    fclose(closeFile);

}