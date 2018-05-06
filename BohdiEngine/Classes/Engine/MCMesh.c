//
//  MCMesh.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMesh.h"
#include "MC3DBase.h"
#include "MCGLContext.h"

oninit(MCMesh)
{
    if (init(MCItem)) {
        var(isDataLoaded) = false;
        var(calculatedNormal) = false;
        
        var(Frame) = (MC3DFrame){0,0,0,0,0,0};
        var(useage) = GL_STATIC_DRAW;
        var(mode) = MCTriAngles;

        var(vertexDataNeedRelease) = true;
        var(vertexDataPtr) = null;
        var(vertexDataSize)= 0;
        var(vertexIndexes) = null;
        var(vertexCount)   = 0;
        
        memset(var(vertexAttribArray), (int)null, sizeof(var(vertexAttribArray)));
        //debug_log("MCMesh - init finished\n");
        return obj;
    }else{
        return null;
    }
}

method(MCMesh, void, bye, voida)
{
    glDeleteBuffers(1, &obj->VBO);
    glDeleteVertexArrays(1, &obj->VAO);
    if (obj->vertexDataNeedRelease && obj->vertexDataPtr) {
        free(obj->vertexDataPtr);
    }
}

method(MCMesh, void, allocVertexBuffer, GLsizei vertexCount)
{
    obj->vertexCount = vertexCount ;
    obj->vertexDataSize = obj->vertexCount * 11 * sizeof(GLfloat);
    if (obj->vertexDataSize != 0) {
        obj->vertexDataPtr = (GLfloat*)malloc(obj->vertexDataSize);
        memset(obj->vertexDataPtr, 0, obj->vertexDataSize);
    }else{
        obj->vertexDataPtr = null;
    }
}

method(MCMesh, MCMesh*, initWithDefaultVertexAttributes, GLsizei vertexCount)
{
    //debug_log("MCMesh - initWithDefaultVertexAttributes\n");
    obj->vertexAttribArray[0] = (MCVertexAttribute){
        MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(0)};
    obj->vertexAttribArray[1] = (MCVertexAttribute){
        MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(12)};
    obj->vertexAttribArray[2] = (MCVertexAttribute){
        MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(24)};
    obj->vertexAttribArray[3] = (MCVertexAttribute){
        MCVertexAttribTexCoord0,2, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(36)};
    
    //alloc vertex buffer
    MCMesh_allocVertexBuffer(obj, vertexCount);
    //obj->vertexIndexes = (GLuint*)malloc(sizeof(GLuint)*obj->vforertexCount);
    
    return obj;
}

method(MCMesh, void, setVertex, GLuint offset, MCMeshVertexData* data)
{
    obj->vertexDataPtr[offset+0] = data->x;
    obj->vertexDataPtr[offset+1] = data->y;
    obj->vertexDataPtr[offset+2] = data->z;
    
    if (obj->calculatedNormal) {
        obj->vertexDataPtr[offset+3] += data->nx;
        obj->vertexDataPtr[offset+4] += data->ny;
        obj->vertexDataPtr[offset+5] += data->nz;
    } else {
        obj->vertexDataPtr[offset+3] = data->nx;
        obj->vertexDataPtr[offset+4] = data->ny;
        obj->vertexDataPtr[offset+5] = data->nz;
    }
    
    obj->vertexDataPtr[offset+6] = data->r;
    obj->vertexDataPtr[offset+7] = data->g;
    obj->vertexDataPtr[offset+8] = data->b;
    
    obj->vertexDataPtr[offset+9]  = data->u;
    obj->vertexDataPtr[offset+10] = data->v;
}

method(MCMesh, void, normalizeNormals, voida)
{
    if (!obj->calculatedNormal) {
        return;
    }
    for (int i=0; i<obj->vertexCount; i++) {
        size_t offset = i * 11;
        GLfloat x = obj->vertexDataPtr[offset+3];
        GLfloat y = obj->vertexDataPtr[offset+4];
        GLfloat z = obj->vertexDataPtr[offset+5];
        
        MCVector3 n = MCVector3Normalize(MCVector3Make(x, y, z));
        obj->vertexDataPtr[offset+3] = n.x;
        obj->vertexDataPtr[offset+4] = n.y;
        obj->vertexDataPtr[offset+5] = n.z;
    }
}

onload(MCMesh)
{
    if (load(MCItem)) {
        binding(MCMesh, void, bye, voida);
        binding(MCMesh, MCMesh*, initWithDefaultVertexAttributes, GLsizei vertexCount);
        binding(MCMesh, void, setVertex, GLuint offset, MCMeshVertexData* data);
        binding(MCMesh, void, normalizeNormals, voida);
        return cla;
    }else{
        return null;
    }
}


