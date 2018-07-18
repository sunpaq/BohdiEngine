//
//  BEMTRenderer.h
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#ifndef BEMTRenderer_H
#define BEMTRenderer_H

#include "MC3DScene.h"
#include "MC3DNode.h"
#include "MCMesh.h"

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>
#import "BEMTContext.h"

@interface BEMTRenderer : NSObject
{
    BEMTContext* context;
}

-(instancetype) initWithMTKView:(MTKView*)view;
-(void) drawFrame;
-(void) drawScene:(MC3DScene*)scene;
-(void) drawNode:(MC3DNode*)node;
-(void) drawMesh:(MCMesh*)mesh;

@end

#endif
