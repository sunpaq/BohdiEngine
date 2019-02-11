//
//  MC3DNode.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DNode.h"

fun(release, void)) as(MC3DNode)
    Release(it->material);
    Release(it->diffuseTexture);
    Release(it->specularTexture);
    Release(it->normalTexture);
    Release(it->children);
    Release(it->meshes);
}

//0=success
fun(addChild, MC3DErrCode), struct MC3DNode* child) as(MC3DNode)
    child->visible = false;
    it->children->addItem(it->children, (struct MCItem*)child);
    child->visible = true;
    return MC3DSuccess;
}

fun(addChildAtIndex, MC3DErrCode), struct MC3DNode* child, int index) as(MC3DNode)
    child->visible = false;
    if (index < 0) {
        it->children->addItem(it->children, (struct MCItem*)child);
    } else {
        it->children->addItemAtIndex(it->children, index, (struct MCItem*)child);
    }
    child->visible = true;
    return MC3DSuccess;
}

fun(removeChild, MC3DErrCode), struct MC3DNode* child) as(MC3DNode)
    child->visible = false;
    it->children->delItem(it->children, (struct MCItem*)child);
    return MC3DSuccess;
}

fun(copyChildrenFrom, void), struct MC3DNode* node) as(MC3DNode)
    it->children->connectList(it->children, node->children);
}

fun(cleanUnvisibleChild, void)) as(MC3DNode)
    MCLinkedListForEach(it->children,
                        struct MC3DNode* node = (struct MC3DNode*)item;
                        if (node != null && node->visible == false) {
                            it->children->delItem(it->children, (struct MCItem*)node);
                        })
}

fun(childCount, int)) as(MC3DNode)
    return it->children->count(it->children);
}

fun(setAllVisible, void), bool visible) as(MC3DNode)
    if (it->children) {
        MCLinkedListForEach(it->children,
                            struct MC3DNode* node = (struct MC3DNode*)item;
                            if (node != null) {
                                node->visible = visible;
                            })
    }
}

fun(changeMatrial, void), struct MCMaterial* material) as(MC3DNode)
    Release(it->material);
    it->material = material;
}

fun(changeTexture, void), struct MCTexture* texture) as(MC3DNode)
    Release(it->diffuseTexture);
    it->diffuseTexture = texture;
}

//pass null use identity matrix
fun(resetTransform, void), MCMatrix4* transform) as(MC3DNode)
    if (transform) {
        it->transform = *transform;
    } else {
        it->transform = MCMatrix4Identity;
    }
}

fun(translateVec3, void), MCVector3* position, bool incremental) as(MC3DNode)
    if (incremental) {
        it->transform = MCMatrix4Multiply(MCMatrix4MakeTranslation(position->x, position->y, position->z), it->transform);
    } else {
        it->transform = MCMatrix4MakeTranslation(position->x, position->y, position->z);
    }
}

fun(scaleVec3, void), MCVector3* factors, bool incremental) as(MC3DNode)
    if (incremental) {
        it->transform = MCMatrix4Multiply(MCMatrix4MakeScale(factors->x, factors->y, factors->z), it->transform);
    } else {
        it->transform = MCMatrix4MakeScale(factors->x, factors->y, factors->z);
    }
}

fun(rotateMat3, void), float mat3[9], bool incremental) as(MC3DNode)
    if (mat3) {
        MCMatrix3 m3 = {0};
        for (int i=0; i<9; i++) {
            m3.m[i] = mat3[i];
        }
        if (incremental) {
            it->transform = MCMatrix4Multiply(MCMatrix4FromMatrix3(m3), it->transform);
        } else {
            it->transform = MCMatrix4FromMatrix3(m3);
        }
    }
}

fun(rotateMat4, void), float mat4[16], bool incremental) as(MC3DNode)
    if (mat4) {
        MCMatrix4 m4 = {0};
        int i;
        for (i=0; i<16; i++) {
            m4.m[i] = mat4[i];
        }
        if (incremental) {
            it->transform = MCMatrix4Multiply(m4, it->transform);
        } else {
            it->transform = m4;
        }
    }
}

fun(hide, void)) as(MC3DNode)
    it->visible = false;
}

fun(show, void)) as(MC3DNode)
    it->visible = true;
}

//draw
fun(willDraw, void), MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model)
{
    
}

fun(didDraw, void))
{
    
}

fun(draw, void))
{
    
}

constructor(MC3DNode)) {
    MCItem(any, null);
    as(MC3DNode)
        it->visible = true;
        it->receiveEvent = false;
        it->overrideDraw = false;
        it->center = MCVector3Make(0, 0, 0);
    
        it->transform = MCMatrix4Identity;
        it->viewtrans = MCMatrix4Identity;
    
        it->material = null;
        it->diffuseTexture = null;
        it->specularTexture = null;
        it->normalTexture = null;
        it->zorder = -1;
    
        it->children = MCLinkedList(alloc(MCLinkedList));
        it->meshes = MCLinkedList(alloc(MCLinkedList));
    };
    dynamic(MC3DNode)
        funbind(release);
        funbind(addChild);
        funbind(addChildAtIndex);
        funbind(removeChild);
        funbind(copyChildrenFrom);
        funbind(cleanUnvisibleChild);
        funbind(childCount);
        funbind(setAllVisible);
        funbind(changeMatrial);
        funbind(changeTexture);
        funbind(resetTransform);
        funbind(translateVec3);
        funbind(rotateMat3);
        funbind(rotateMat4);
        funbind(scaleVec3);
        funbind(hide);
        funbind(show);
        funbind(willDraw);
        funbind(didDraw);
        funbind(draw);
    };
    return any;
}

