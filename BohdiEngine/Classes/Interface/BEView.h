//
//  BEView.h
//  Pods
//
//  Created by Sun YuLi on 2017/5/22.
//
//

#include "TargetConditionals.h"
#if TARGET_OS_IOS

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "BERenderer.h"
#import "BERunLoop.h"
#import "BEMotionManager.h"
#import "BEGameController.h"

@protocol BEViewRenderingDelegate
- (void) beforeRenderFrame;
- (void) afterRenderFrame;
@end

@interface BEView : UIView

@property (nonatomic) id<BEViewRenderingDelegate> delegate;

@property (atomic, readonly) BEMotionManager* motion;
@property (atomic, readonly) BERenderer* renderer;
@property (atomic, readonly) BERunLoop* runloop;
@property (atomic, readonly) GLKView* glview;

- (void) loadModelNamed:(NSString*)name;
- (void) loadSkysphNamed:(NSString*)texname;

- (void) startDraw3DContent:(BECameraRotateMode)rmode;
- (void) stopDraw3DContent;

- (void) resetAttitude;

@end

#endif
