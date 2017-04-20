//
//  BERenderer.h
//  Pods
//
//  Created by YuliSun on 19/04/2017.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "BEInterface.h"

@interface BERenderer : NSObject

+(GLKView*) createDefaultGLView:(CGRect)frame;

-(instancetype) init __unavailable;
-(instancetype) initWithFrame:(CGRect)frame doesOpaque:(BOOL)opaque;
-(instancetype) initWithFrame:(CGRect)frame doesOpaque:(BOOL)opaque cameraRotateMode:(BECameraRotateMode)rmode;

-(void) removeCurrentModel;
-(void) addModelNamed:(NSString*)modelName;

//use default if names/name is null
-(void) addSkyboxNamed:(NSArray*)texnames;
-(void) addSkysphNamed:(NSString*)texname;
-(void) removeCurrentSkybox;
-(void) removeCurrentSkysph;

//need 4x4 column major matrix
-(void) cameraReset:(float*)mat4;
-(void) cameraRotate:(GLKMatrix3)mat3 Incremental:(BOOL)inc;
-(void) cameraTranslate:(GLKVector3)vec3 Incremental:(BOOL)inc;

//light pos follow camera if pos is null
-(void) lightReset:(GLKVector3*)pos;

-(void) handlePanGesture:(CGPoint)offset;
-(void) handlePinchGesture:(float)scale;

-(void) drawFrame;

@end
