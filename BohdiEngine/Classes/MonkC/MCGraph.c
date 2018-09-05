//
//  MCGraph.c
//  Sapindus
//
//  Created by YuliSun on 18/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCGraph.h"

//typedef struct {
//    MCGeneric value;
//    MCArray* neighbors;
//    MCArray* edges;
//} MCGraphVertex;
//
//typedef struct {
//    int weight;
//    MCGraphVertex A;
//    MCGraphVertex B;
//} MCGraphEdge;
//
//class(MCGraph, MCObject,
//      MCGraphVertex* vertexSet;
//      MCGraphEdge*   edgeSet);

oninit(MCGraph)
{
    if (init(MCObject)) {
        var(vertexCount) = 0;
        var(edgeCount) = 0;
        var(vertexSet) = null;
        var(edgeSet) = null;
        return obj;
    } else {
        return null;
    }
}

fun(MCGraph, void, bye, voida)
{
    if (var(vertexSet)) {
        free(var(vertexSet));
    }
    if (var(edgeSet)) {
        free(var(edgeSet));
    }
}

fun(MCGraph, MCBool, isAdjacent, MCGraphVertex x, MCGraphVertex y)
{
    MCGraphVertex* iter = x.neighbors;
    while (iter) {
        if (iter->index == y.index) {
            return true;
        }
        iter = iter->next;
    }
    return false;
}

fun(MCGraph, MCArray*, copyNeighborsOf, MCGraphVertex x)
{
    MCArray* array = new(MCArray);
    MCGraphVertex* iter = x.neighbors;
    while (iter) {
        MCArray_addItem(array, MCGenericI(iter->index));
        iter = iter->next;
    }
    return array;
}

fun(MCGraph, MCGraph*, addEdge, MCGraphEdge e)
{
    return obj;
}

fun(MCGraph, MCGraph*, removeEdge, MCGraphEdge e)
{
    return obj;
}

onload(MCGraph)
{
    if (load(MCObject)) {
        bid(MCGraph, void, bye, voida);
        bid(MCGraph, MCBool, isAdjacent, MCGraphVertex x, MCGraphVertex y);
        bid(MCGraph, MCArray*, copyNeighborsOf, MCGraphVertex x);
        bid(MCGraph, MCGraph*, addEdge, MCGraphEdge e);
        bid(MCGraph, MCGraph*, removeEdge, MCGraphEdge e);
        return cla;
    } else {
        return null;
    }
}
