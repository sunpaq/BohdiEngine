//
//  MC3DNode.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DNode.h"

oninit(MC3DNode)
{
    if (init(MCItem)) {
        var(visible) = true;
        var(receiveEvent) = false;
        var(overrideDraw) = false;
        var(center) = MCVector3Make(0, 0, 0);
        
        var(transform) = MCMatrix4Identity;
        var(viewtrans) = MCMatrix4Identity;
        
        var(material) = null;
        var(diffuseTexture) = null;
        var(specularTexture)= null;
        var(normalTexture) = null;
        var(zorder) = -1;
        
        var(children) = new(MCLinkedList);
        var(meshes) = new(MCLinkedList);
        
        return obj;
    }else{
        return null;
    }
}

fun(MC3DNode, void, bye, voida)
{
    release(var(material));
    release(var(diffuseTexture));
    release(var(specularTexture));
    release(var(normalTexture));
    release(var(children));
    release(var(meshes));
}

fun(MC3DNode, MC3DErrCode, addChild, MC3DNode* child)
{
    child->visible = false;
    MCLinkedList_addItem(var(children), (MCItem*)child);
    child->visible = true;
    return MC3DSuccess;
}

fun(MC3DNode, MC3DErrCode, addChildAtIndex, MC3DNode* child, int index)
{
    child->visible = false;
    if (index < 0) {
        MCLinkedList_addItem(var(children), (MCItem*)child);
    } else {
        MCLinkedList_addItemAtIndex(var(children), index, (MCItem*)child);
    }
    child->visible = true;
    return MC3DSuccess;
}

fun(MC3DNode, MC3DErrCode, removeChild, MC3DNode* child)
{
    child->visible = false;
    MCLinkedList_delItem(var(children), (MCItem*)child);
    return MC3DSuccess;
}

fun(MC3DNode, void, copyChildrenFrom, MC3DNode* node)
{
    MCLinkedList_connectList(var(children), node->children);
}

fun(MC3DNode, void, cleanUnvisibleChild, voida)
{
    MCLinkedListForEach(var(children),
                        MC3DNode* node = (MC3DNode*)item;
                        if (node != null && node->visible == false) {
                            MCLinkedList_delItem(var(children), (MCItem*)node);
                        })
}

fun(MC3DNode, int, childCount, voida)
{
    return computed(obj->children, count);
}

fun(MC3DNode, void, setAllVisible, MCBool visible)
{
    if (var(children)) {
        MCLinkedListForEach(var(children),
                            MC3DNode* node = (MC3DNode*)item;
                            if (node != null) {
                                node->visible = visible;
                            })
    }
}

fun(MC3DNode, void, changeMatrial, MCMaterial* material)
{
    release(obj->material);
    obj->material = material;
}

fun(MC3DNode, void, changeTexture, MCTexture* texture)
{
    release(obj->diffuseTexture);
    obj->diffuseTexture = texture;
}

fun(MC3DNode, void, resetTransform, MCMatrix4* transform)
{
    if (transform) {
        obj->transform = *transform;
    } else {
        obj->transform = MCMatrix4Identity;
    }
}

fun(MC3DNode, void, translateVec3, MCVector3* position, MCBool incremental)
{
    if (incremental) {
        obj->transform = MCMatrix4Multiply(MCMatrix4MakeTranslation(position->x, position->y, position->z), obj->transform);
    } else {
        obj->transform = MCMatrix4MakeTranslation(position->x, position->y, position->z);
    }
}

fun(MC3DNode, void, scaleVec3, MCVector3* factors, MCBool incremental)
{
    if (incremental) {
        obj->transform = MCMatrix4Multiply(MCMatrix4MakeScale(factors->x, factors->y, factors->z), obj->transform);
    } else {
        obj->transform = MCMatrix4MakeScale(factors->x, factors->y, factors->z);
    }
}

fun(MC3DNode, void, rotateMat3, float mat3[9], MCBool incremental)
{
    if (mat3) {
        MCMatrix3 m3 = {0};
        for (int i=0; i<9; i++) {
            m3.m[i] = mat3[i];
        }
        if (incremental) {
            obj->transform = MCMatrix4Multiply(MCMatrix4FromMatrix3(m3), obj->transform);
        } else {
            obj->transform = MCMatrix4FromMatrix3(m3);
        }
    }
}

fun(MC3DNode, void, rotateMat4, float mat4[16], MCBool incremental)
{
    if (mat4) {
        MCMatrix4 m4 = {0};
        for (int i=0; i<16; i++) {
            m4.m[i] = mat4[i];
        }
        if (incremental) {
            obj->transform = MCMatrix4Multiply(m4, obj->transform);
        } else {
            obj->transform = m4;
        }
    }
}

fun(MC3DNode, void, hide, voida)
{
    var(visible) = false;
}

fun(MC3DNode, void, show, voida)
{
    var(visible) = true;
}

fun(MC3DNode, void, willDraw, MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model)
{
    
}

fun(MC3DNode, void, didDraw, voida)
{
    
}

fun(MC3DNode, void, draw, voida)
{
    
}

onload(MC3DNode)
{
    if (load(MCItem)) {
        bid(MC3DNode, void, bye, voida);
        bid(MC3DNode, void, addChild, MC3DNode* child);
        bid(MC3DNode, MC3DErrCode, addChildAtIndex, MC3DNode* child, int index);
        bid(MC3DNode, void, removeChild, MC3DNode* child);
        bid(MC3DNode, void, copyChildrenFrom, MC3DNode* node);
        bid(MC3DNode, void, cleanUnvisibleChild, voida);
        bid(MC3DNode, int, childCount, voida);
        bid(MC3DNode, void, setAllVisible, MCBool visible);
        bid(MC3DNode, void, changeMatrial, MCMaterial* material);
        bid(MC3DNode, void, changeTexture, MCTexture* texture);
        bid(MC3DNode, void, resetTransform, MCMatrix4* transform);
        bid(MC3DNode, void, translateVec3, MCVector3* position, MCBool incremental);
        bid(MC3DNode, void, rotateMat3, float mat3[9], MCBool incremental);
        bid(MC3DNode, void, rotateMat4, float mat4[16], MCBool incremental);
        bid(MC3DNode, void, scaleVec3, MCVector3* factors, MCBool incremental);
        bid(MC3DNode, void, hide, voida);
        bid(MC3DNode, void, show, voida);
        bid(MC3DNode, void, willDraw, MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model);
        bid(MC3DNode, void, didDraw, voida);
        bid(MC3DNode, void, draw, voida);
        return cla;
    }else{
        return null;
    }
}
