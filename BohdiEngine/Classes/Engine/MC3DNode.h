//
//  MC3DNode.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DNode_h
#define MC3DNode_h

//#include "monkc_export.h"
#include "MC3DBase.h"
#include "MCTexture.h"
#include "MCMaterial.h"
#include "MCTexture.h"
#include "MCLinkedList.h"

structure(MC3DNode, MCItem)
    unsigned index;
    int zorder;
    bool visible;
    bool overrideDraw;
    bool receiveEvent;

    MCVector3 center;
    MCMatrix4 transform;
    MCMatrix4 viewtrans;

    struct MCMaterial* material;
    struct MCTexture* diffuseTexture;
    struct MCTexture* specularTexture;
    struct MCTexture* normalTexture;

    struct MCLinkedList* meshes;
    struct MCLinkedList* children;


    fundef(release, void));
    //0=success
    fundef(addChild, MC3DErrCode), struct MC3DNode* child);
    fundef(addChildAtIndex, MC3DErrCode), struct MC3DNode* child, int index);
    fundef(removeChild, MC3DErrCode), struct MC3DNode* child);

    fundef(copyChildrenFrom, void), struct MC3DNode* node);
    fundef(cleanUnvisibleChild, void));
    fundef(childCount, int));
    fundef(setAllVisible, void), bool visible);

    fundef(changeMatrial, void), struct MCMaterial* material);
    fundef(changeTexture, void), struct MCTexture* texture);
    //pass null use identity matrix
    fundef(resetTransform, void), MCMatrix4* transform);
    fundef(translateVec3, void), MCVector3* position, bool incremental);
    fundef(rotateMat3, void), float mat3[9], bool incremental);
    fundef(rotateMat4, void), float mat4[16], bool incremental);
    fundef(scaleVec3, void), MCVector3* factors, bool incremental);

    //draw
    fundef(willDraw, void), MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model);
    fundef(didDraw, void));
    fundef(draw, void));
    fundef(show, void));
    fundef(hide, void));
};

constructor(MC3DNode));

alias(MC3DNode);

#endif /* MC3DNode_h */
