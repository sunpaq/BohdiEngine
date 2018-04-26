//
//  BEView.m
//  Pods
//
//  Created by Sun YuLi on 2017/5/22.
//
//

#include "TargetConditionals.h"
#if TARGET_OS_IOS

#import "BEView.h"

@implementation BEView

@synthesize motion;
@synthesize renderer;
@synthesize runloop;
@synthesize glview;

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [self setupWithFrame:self.bounds];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setupWithFrame:frame];
    }
    return self;
}

- (void)setupWithFrame:(CGRect)frame
{
    //view
    glview = [BERenderer createDefaultGLView:frame];
    if ([self.subviews count] <= 0) {
        [self addSubview:glview];
    } else {
        UIView* view = [self.subviews objectAtIndex:0];
        if (view) {
            [self insertSubview:glview belowSubview:view];
        }
    }
    //render delegate
    _delegate = nil;
    //renderer
    renderer = [[BERenderer alloc] initWithFrame:frame];
    //runloop
    runloop = [[BERunLoop alloc] initWithTarget:self Selector:@selector(drawFrame)];
    //device motion
    motion = nil;
}

- (void)drawFrame
{    
    if (motion) {
        CMAttitude* att = nil;
        if ((att=[motion getDeltaAttitude]) != nil) {
            renderer.deviceRotateMat3 = att.rotationMatrix;
        }
    }
    
    if (_delegate) {
        [_delegate beforeRenderFrame];
    }
    [renderer drawFrameOnGLView:glview];
    if (_delegate) {
        [_delegate afterRenderFrame];
    }
}

- (void) loadModelNamed:(NSString*)name
{
    [renderer addModelNamed:name];
}

- (void) loadSkysphNamed:(NSString*)texname
{
    [renderer addSkysphNamed:texname];
}

- (void) startDraw3DContent:(BECameraRotateMode)rmode
{
    if (rmode == BECameraRotateAroundModelByGyroscope
        || rmode == BECameraRotateAroundModelByGyroscopeReverse) {
        motion = [BEMotionManager shared];
        [motion startDeviceMotion];
    }
    [renderer setCameraRotateMode:rmode];
    [runloop startRunloop];
}

- (void) stopDraw3DContent
{
    [runloop stopRunloop];
}

- (void) resetAttitude
{
    [self.motion resetAttitude];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    glview.frame = self.bounds;
    [renderer resizeAllScene:self.bounds.size];
}

@end

#endif
