//
//  GLView.h
//  BEDemo
//
//  Created by YuliSun on 02/03/2017.
//  Copyright © 2017 SODEC. All rights reserved.
//

#ifndef BEViewController_h
#define BEViewController_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <CoreMotion/CoreMotion.h>

typedef enum {
    BECameraRotateAroundModelManual,
    BECameraRotateAroundModelByGyroscope,
    BECameraRotateAR
} BECameraRotateMode;

@interface BEViewController : GLKViewController <GLKViewDelegate>

@property (atomic, readwrite) BOOL useTransparentBackground;
@property (atomic, readwrite) BOOL useDeltaRotationData;
@property (atomic, readwrite) BOOL doesRotateCamera;
@property (atomic, readwrite) BOOL doesDrawWireFrame;

@property (atomic, readwrite) CMRotationMatrix deviceRotateMat3;
@property (atomic, readwrite) BECameraRotateMode cameraRotateMode;

@property (nonatomic, strong) UIActivityIndicatorView* indicator;

-(void) addModelNamed:(NSString*)modelName;

//use default if names/name is null
-(void) addSkyboxNamed:(NSArray*)texnames;
-(void) addSkysphNamed:(NSString*)texname;

-(void) glviewResize:(CGRect)frame;

-(void) cameraReset:(GLKMatrix4*)mat4;
-(void) cameraRotate:(GLKMatrix3)mat3 Incremental:(BOOL)inc;
-(void) cameraTranslate:(GLKVector3)vec3 Incremental:(BOOL)inc;

//light pos follow camera if pos is null
-(void) lightReset:(GLKVector3*)pos;

-(void) handlePanGesture:(CGPoint)offset;
-(void) handlePinchGesture:(float)scale;
@end


#endif /* BEViewController_h */
