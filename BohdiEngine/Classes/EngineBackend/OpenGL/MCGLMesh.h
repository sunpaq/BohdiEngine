//
//  MCGLMesh.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCGLMesh_h
#define MCGLMesh_h

#include "monkc.h"
#include "MCGLBase.h"
#include "MCLinkedList.h"
#include "MCMath.h"
#include "BAObjParser.h"

typedef union {
    struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat nx;
        GLfloat ny;
        GLfloat nz;
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLfloat u;
        GLfloat v;
    };
    GLfloat data[11];
} MCVertexData;

class(MCGLMesh, MCItem,
      MCBool     isDataLoaded;
      MCBool     calculatedNormal;
      
      MC3DFrame  Frame;
      GLenum     useage;
      
      GLuint     VAO;  //VAO
      GLuint     VBO;  //VBO
      GLuint     EBO;  //EBO
      
      MCBool     vertexDataNeedRelease;
      GLfloat*   vertexDataPtr;   //gCubeVertexData
      GLsizeiptr vertexDataSize;  //sizeof(gCubeVertexData)
      GLuint*    vertexIndexes;
      GLsizei    vertexCount;

      MCVertexAttribute vertexAttribArray[MCVertexAttribIndexMax];
      MCDrawMode mode;
);

method(MCGLMesh, void, bye, voida);
method(MCGLMesh, MCGLMesh*, initWithDefaultVertexAttributes, GLsizei vertexCount);
method(MCGLMesh, void, setVertex, GLuint offset, MCVertexData* data);
method(MCGLMesh, void, normalizeNormals, voida);

method(MCGLMesh, void, dump, voida);

#endif /* MCGLMesh_h */
