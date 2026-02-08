/***
 * Morgan Stein
 * mkstein
 * 2026 Winter CSE101 PA2
 * Graph.c
 * Definition of graph functions
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "List.h"
#include "Graph.h"

#define white 0
#define grey 1
#define black 2
#define INF -1
#define NIL -2


 typedef struct GraphObj{
    int vertices;
    int edges;
    int source;
    int* color;
    int* parent;
    int* distance;
    List* adjacency;
 } GraphObj;


// constructors - destructors -------------------------------------------------

// newGraph()
// Returns a Graph having n vertices and no edges.
Graph newGraph(int n){
    Graph G = malloc(sizeof(GraphObj));
    assert(G!=NULL&&"ERROR: newGraph(): NULL Graph");

    G->vertices = n;
    G->edges = 0;
    G->source = NIL;

    G->adjacency = calloc(n+1, sizeof(List));
    assert(G->adjacency!=NULL && "ERROR: newGraph: NULL Graph (adjacency list)");

    for(int i =1; i<=n; i++){
        G->adjacency[i]=newList();
    }

    G->color = calloc(n+1, sizeof(int));
    G->parent = calloc(n+1, sizeof(int));
    G->distance = calloc(n+1, sizeof(int));

    for(int i =1; i<=n;i++){
       G->color[i] = white;
       G->parent[i] = NIL;
       G->distance[i] = INF; 
    }

    return G;
}

// freeGraph()
// Frees all dynamic memory associated with Graph *pG and sets *pG to NULL.
void freeGraph(Graph* pG){
    if (pG!=NULL && *pG!=NULL){
        Graph G = *pG;
        for(int i =1; i<=(*pG)->vertices; i++){
            freeList(&((*pG)->adjacency[i]));
        }
        free(G->adjacency);
        free(G->color);
        free(G->parent);
        free(G->distance);
        free(G);
        *pG=NULL;
    }
}


// access functions -----------------------------------------------------------

// getOrder()
// Returns the number of vertices in G.
int getOrder(Graph G){
    if (G == NULL){
        fprintf(stderr, "Graph ERROR: getOrder(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    return G->vertices;
}

// getNumEdges()
// Returns the number of edges in G.
int getNumEdges(Graph G){
        if (G == NULL){
            fprintf(stderr, "Graph ERROR: getNumEdges(): NULL Graph");
            exit(EXIT_FAILURE);
        }
        return G->edges;
    }


// getNumArcs()
// Returns the number of Arcs in G.
int getNumArcs(Graph G){
    if (G == NULL){
        fprintf(stderr, "Graph ERROR: getNumEdges(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    int x = G->edges;
    return (2*x);
}


// getSource()
// Returns the source vertex in the most recent call to BFS(), or NIL if
// BFS() has not yet been called.
int getSource(Graph G){
    if (G == NULL){
        fprintf(stderr, "Graph ERROR: getSource(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    if(G->source == NIL){
        return NIL;
    }else{
        return G->source;
    }
}

// getParent
// Returns the parent of vertex u in the most recently constructed BFS tree
// or returns NIL if BFS() has not yet been called.
// Pre: 1 <= u <= getOrder(G)
int getParent(Graph G, int u){
    if(u<1){
        fprintf(stderr,"Graph ERROR: getParent(): u is less than 1");
        exit(EXIT_FAILURE);
    }
    if(u>(getOrder(G))){
        fprintf(stderr,"Graph ERROR: getParent(): u is greater than getOrder(G)");
        exit(EXIT_FAILURE);
    }
    if(G->source == NIL){
        return NIL;
    }else{
        return G->parent[u];
    }

}
// getDist()
// Returns the distance from the source vertex to u if BFS() has been called,
// otherwise returns INF.
// Pre: 1 <= u <= getOrder(G)
int getDist(Graph G, int u){
    if(u<1){
        fprintf(stderr,"Graph ERROR: getDist(): u is less than 1");
        exit(EXIT_FAILURE);
    }
    if(u>(getOrder(G))){
        fprintf(stderr,"Graph ERROR: getDist(): u is greater than getOrder(G)");
        exit(EXIT_FAILURE);
    }
    if(G->source == NIL){
        return INF;
    }else{
        return G->distance[u];
    }
}

// getPath()
// If vertex u is reachable from the source, appends the vertices of a shortest
// source-u path to List L. Otherwise, appends NIL to L.
// Pre: 1 <= u <= getOrder(G), getSource(G) != NIL
void getPath(List L, Graph G, int u){
    if(u<1){
        fprintf(stderr,"Graph ERROR: getPath(): u is less than 1");
        exit(EXIT_FAILURE);
    }
    if(u>(getOrder(G))){
        fprintf(stderr,"Graph ERROR: getPath(): u is greater than getOrder(G)");
        exit(EXIT_FAILURE);
    }
    if(getSource(G)==NIL){
        fprintf(stderr,"Graph ERROR: getPath(): getSource(G) is NIL");
    }
    if (u==G->source){
        append(L,u);
    }else if (G->parent[u]==NIL){
        append(L,NIL);
    }else{
        getPath(L,G,G->parent[u]);
        append(L,u);
    }
}


// manipulation procedures ----------------------------------------------------

// makeNull()
// Resets G to its initial state.
void makeNull(Graph G){
    if (G==NULL){
        fprintf(stderr, "GRAPH ERROR: makeNull(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    for(int i =1; i<=G->vertices; i++){
        clear(G->adjacency[i]);
        G->color[i]=white;
        G->parent[i]=NIL;
        G->distance[i]=INF;
    }
    G->edges = 0;
    G->source = NIL;
}

// addEdge()
// Creates an undirected edge joining vertex u to vertex v.
// Pre: 1 <= u <= getOrder(G), 1 <= v <= getOrder(G)
void addEdge(Graph G, int u, int v){
    if (G==NULL){
        fprintf(stderr, "GRAPH ERROR: addEdge(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    if(u<1||u>G->vertices){
        fprintf(stderr, "GRAPH ERROR: addEdge(): u did not meet pre conditions");
        exit(EXIT_FAILURE);
    }
    if(v<1||v>G->vertices){
        fprintf(stderr, "GRAPH ERROR: addEdge(): v did not meet pre conditions");
        exit(EXIT_FAILURE);
    }
    append(G->adjacency[u], v);
    append(G->adjacency[v],u);
    G->edges++;
}

// addArc()
// Creates a directed edge joining vertex u to vertex v.
// Pre: 1 <= u <= getOrder(G), 1 <= v <= getOrder(G)
void addArc(Graph G, int u, int v){
    if (G==NULL){
        fprintf(stderr, "GRAPH ERROR: addArc(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    if(u<1||u>G->vertices){
        fprintf(stderr, "GRAPH ERROR: addArc(): u did not meet pre conditions");
        exit(EXIT_FAILURE);
    }
    if(v<1||v>G->vertices){
        fprintf(stderr, "GRAPH ERROR: addArc(): v did not meet pre conditions");
        exit(EXIT_FAILURE);
    }
    List L = G->adjacency[u];
    moveFront(L);
    while(position(L)>=0 && get(L)<v){
        moveNext(L);
    }
    if(position(L)>=0){
        insertBefore(L,v);
    } else{
        append(L,v);
    }
    
    G->edges++;
}

// BFS()
// Runs the Breadth First Search algorithm on G with source vertex s.
void BFS(Graph G, int s){
    if(s>G->vertices||s<=0){
        fprintf(stderr,"GRAPH ERROR: BFS(): invalid source vertex");
        exit(EXIT_FAILURE);  
    }
    if (G==NULL){
        fprintf(stderr,"GRAPH ERROR: BFS(): NULL Graph");
        exit(EXIT_FAILURE);
    }
    for(int x=1; x<=G->vertices; x++){
        G->color[x]=white;
        G->distance[x] = INF;
        G->parent[x]=NIL;
    }

    G->source = s;
    G->color[s] = grey;
    G->distance[s] = 0;
    G->parent[s]= NIL;

    List L = newList();
    append(L,s);
    while(length(L)>0){
        int x = front(L);
        deleteFront(L);

        for(moveFront(G->adjacency[x]); position(G->adjacency[x])>=0; moveNext(G->adjacency[x])){
            int y = get(G->adjacency[x]);
            if (G->color[y] == white){
                G->color[y] = grey;
                G->distance[y] = G->distance[x]+1;
                G->parent[y] = x;
                append(L,y); 
            }
        }
        G->color[x]=black;
    }
    freeList(&L);
}


// other functions ------------------------------------------------------------

// printGraph()
// Prints the adjacency list representation of G to FILE* out.
void printGraph(FILE* out, Graph G){
    if (G==NULL){
        fprintf(stderr, "GRAPH ERROR: printGraph(): NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (out==NULL){
        fprintf(stderr, "GRAPH ERROR: printGraph(): NULL file\n");
        exit(EXIT_FAILURE);
    }

    for (int i=1; i<=G->vertices; i++){
        fprintf(out, "%d: (", i);
        for(moveFront(G->adjacency[i]); position(G->adjacency[i])>=0; moveNext(G->adjacency[i])){
            int j = get(G->adjacency[i]);
            fprintf(out,"%d",j);
            if(position(G->adjacency[i])<length(G->adjacency[i])-1){ 
                fprintf(out,", ");
            }
        }
        fprintf(out,")\n");
    }
}