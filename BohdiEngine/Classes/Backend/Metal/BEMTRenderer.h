//
//  BEMTRenderer.h
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//



#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

#import "MC3DScene.h"
#import "MC3DNode.h"
#import "MCMesh.h"

@interface BEMTRenderer : NSObject

@property id<MTLDevice> device;
@property id<MTLRenderPipelineState> pipelineState;
@property id<MTLCommandQueue> commandQueue;
@property vector_uint2 viewportSize;

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;
-(void) addModelNamed:(NSString*)modelName;
-(void) addModelNamed:(NSString*)modelName Scale:(double)scale;
-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian;
-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian Tag:(int)tag;
-(void) drawFrame;
-(void) drawScene:(MC3DScene*)scene;
-(void) drawNode:(MC3DNode*)node;
-(void) drawMesh:(MCMesh*)mesh;

@end
