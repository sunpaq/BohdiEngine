//
//  MCGraph.h
//  Sapindus
//
//  Created by YuliSun on 18/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

/*
 dense  graph (near the max number of edges)
 sparse graph (few edges)
 
 1.adjacency list (generally preferred)
 2.adjacency matrix
 3.incidence matrix
 
 for the edges have same weight, BFS is better
 */

#ifndef MCGraph_h
#define MCGraph_h

#include <stdio.h>
#include "monkc.h"
#include "MCArray.h"

struct _MCGraphEdge;

typedef struct _MCGraphVertex {
    struct _MCGraphVertex* next;
    struct _MCGraphVertex* neighbors;
    size_t neighborsCount;
    int index;
} MCGraphVertex;

typedef struct _MCGraphEdge {
    int weight;
    MCGraphVertex A;
    MCGraphVertex B;
} MCGraphEdge;

structure(MCGraph, MCObject)
    size_t vertexCount;
    size_t edgeCount;
    MCGraphVertex* vertexSet;
    MCGraphEdge*   edgeSet;

    fundef(bye, void));
    fundef(isAdjacent, bool), MCGraphVertex x, MCGraphVertex y);
    fundef(copyNeighborsOf, struct MCArray*), MCGraphVertex x);
    fundef(addEdge, struct MCGraph*), MCGraphEdge e);
    fundef(removeEdge, struct MCGraph*), MCGraphEdge e);
};

constructor(MCGraph));

#endif /* MCGraph_h */

