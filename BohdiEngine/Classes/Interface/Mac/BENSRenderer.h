//
//  BERenderer.h
//  Pods
//
//  Created by YuliSun on 19/04/2017.
//
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <GLKit/GLKit.h>
#import "MCGLBase.h"

typedef enum {
    BECameraFixedAtOrigin = 0,
    BECameraRotateAroundModelManual,
    BECameraRotateAroundModelByGyroscope,
    BECameraRotateAroundModelByGyroscopeReverse
} BECameraRotateMode;

@interface BENSRenderer : NSObject

@property (atomic, readwrite) BOOL doesAutoRotateCamera;
@property (atomic, readwrite) BOOL doesDrawWireFrame;
//@property (atomic, readwrite) CMRotationMatrix deviceRotateMat3;

+(NSOpenGLView*) createDefaultGLView:(CGRect)frame;
+(void) createFramebuffersWithContext:(NSOpenGLContext*)ctx AndLayer:(NSOpenGLLayer*)lyr;

-(instancetype) init __unavailable;
-(instancetype) initWithFrame:(CGRect)frame;

-(instancetype) setCameraRotateMode:(BECameraRotateMode)rmode;
-(instancetype) setBackgroundColor:(NSColor*)color;
-(instancetype) resizeAllScene:(CGSize)frameSize;
-(instancetype) scissorAllScene:(CGRect)frame;

-(void) removeCurrentModel;
-(void) addModel:(NSString*)modelpath;
-(void) addModelNamed:(NSString*)modelName;
-(void) addModelNamed:(NSString*)modelName Scale:(double)scale;
-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian;
-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian Tag:(int)tag;

//use default if names/name is null
-(void) addSkyboxNamed:(NSArray*)texnames;
-(void) addSkysphNamed:(NSString*)texname;
-(void) removeCurrentSkybox;
-(void) removeCurrentSkysph;

//need 4x4 column major matrix
-(void) cameraReset:(float*)mat4;
-(void) cameraReset:(float*)mat4 isRowMajor:(BOOL)rowm;

-(void) cameraRotate:(GLKMatrix3)mat3 Incremental:(BOOL)inc;
-(void) cameraTranslate:(GLKVector3)vec3 Incremental:(BOOL)inc;
-(void) cameraAspectRatioReset:(float)aspectRatio;
-(void) cameraFOVReset:(float)fov;

-(void) cameraTransformWorld:(GLKMatrix4)mat4;
-(void) cameraTransformSelf:(GLKMatrix4)mat4;

//light pos follow camera if pos is null
-(void) lightReset:(GLKVector3*)pos;

-(void) rotateModelByPanGesture:(CGPoint)offset;
-(void) rotateSkysphByPanGesture:(CGPoint)offset;
-(void) zoomModelByPinchGesture:(CGFloat)scale;

-(void) updateModelTag:(int)tag PoseMat4D:(double*)mat4;
-(void) updateModelTag:(int)tag PoseMat4F:(float*)mat4;

-(void) drawFrame;
-(void) drawFrame:(CGRect)viewport;
-(void) drawFrame:(CGRect)viewport vrHeadTransform:(GLKMatrix4)head vrEyeTransform:(GLKMatrix4)eye;
-(void) drawFrame:(CGRect)viewport vrHeadTransform:(GLKMatrix4)head vrEyeTransform:(GLKMatrix4)eye vrFOV:(CGFloat)fov;

-(void) drawFrameOnCALayer:(CALayer*)calayer;
-(void) drawFrameOnGLView:(NSOpenGLView*)glview;

-(void) pauseDraw:(BOOL)pause;

@end
