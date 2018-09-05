//
//  MC3DNode.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DNode_h
#define MC3DNode_h

#include "monkc_export.h"
#include "MC3DBase.h"
#include "MCTexture.h"
#include "MCMaterial.h"
#include "MCTexture.h"

class(MC3DNode, MCItem,
      MCUInt index;
      MCInt zorder;
      MCBool visible;

      MCBool overrideDraw;
      MCBool receiveEvent;

      MCVector3 center;
      MCMatrix4 transform;
      MCMatrix4 viewtrans;

      MCMaterial* material;
      MCTexture* diffuseTexture;
      MCTexture* specularTexture;
      MCTexture* normalTexture;

      MCLinkedList* meshes;
      MCLinkedList* children;
);

fun(MC3DNode, void, bye, voida);
//0=success
fun(MC3DNode, MC3DErrCode, addChild, MC3DNode* child);
fun(MC3DNode, MC3DErrCode, addChildAtIndex, MC3DNode* child, int index);
fun(MC3DNode, MC3DErrCode, removeChild, MC3DNode* child);

fun(MC3DNode, void, copyChildrenFrom, MC3DNode* node);
fun(MC3DNode, void, cleanUnvisibleChild, voida);
fun(MC3DNode, int,  childCount, voida);
fun(MC3DNode, void, setAllVisible, MCBool visible);

fun(MC3DNode, void, changeMatrial, MCMaterial* material);
fun(MC3DNode, void, changeTexture, MCTexture* texture);
//pass null use identity matrix
fun(MC3DNode, void, resetTransform, MCMatrix4* transform);
fun(MC3DNode, void, translateVec3, MCVector3* position, MCBool incremental);
fun(MC3DNode, void, rotateMat3, float mat3[9], MCBool incremental);
fun(MC3DNode, void, rotateMat4, float mat4[16], MCBool incremental);
fun(MC3DNode, void, scaleVec3, MCVector3* factors, MCBool incremental);

//draw
fun(MC3DNode, void, willDraw, MCMatrix4* projection, MCMatrix4* view, MCMatrix4* model);
fun(MC3DNode, void, didDraw, voida);
fun(MC3DNode, void, draw, voida);
fun(MC3DNode, void, show, voida);
fun(MC3DNode, void, hide, voida);
fun(MC3DNode, void, show, voida);

#endif /* MC3DNode_h */
