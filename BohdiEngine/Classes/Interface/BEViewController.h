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
#import "BEInterface.h"
#import "BERenderer.h"

@interface BEViewController : GLKViewController <GLKViewDelegate, UIGestureRecognizerDelegate>

@property (atomic, readwrite) BOOL useMultisampleAntiAlias;
@property (atomic, readwrite) BOOL useDeltaRotationData;

@property (atomic, readonly) BERenderer* renderer;
@property (atomic, strong) UIActivityIndicatorView* indicator;

-(void) glviewResize:(CGRect)frame;

//motion
-(void)startDeviceMotion;
-(void)stopDeviceMotion;

//Add 'layer' to the end of the receiver's sublayers array. If 'layer'
//already has a superlayer, it will be removed before being added.
-(void)attachGLLayerTo:(CALayer*)superlayer;
-(void)detachGLLayer;
@end


#endif /* BEViewController_h */
