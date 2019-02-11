//
//  MCCube.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCCube.h"
#include "MC3DBase.h"
#include "MCMesh.h"

static float gCubeVertexData[11*6*6] = {
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,     colorR, colorG, colorB     texCoordX texCoordY
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           0.0f, 1.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           1.0f, 1.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           0.0f, 1.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           0.0f, 1.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           1.0f, 1.0f,

    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           0.0f, 0.0f,

    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           1.0f, 1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           0.0f, 1.0f,
};

constructor(MCCube)) {
    MC3DNode(any);
    as(MC3DNode)
        struct MCMesh* mesh = MCMesh(alloc(MCMesh), 36);
        mesh->vertexDataNeedRelease = false;
        mesh->vertexDataPtr = gCubeVertexData;
        mesh->vertexDataSize = sizeof(gCubeVertexData);
        it->meshes->addItem(it->meshes, (struct MCItem*)mesh);
    }
    return any;
}


