//
//  GLView.m
//  BEDemo
//
//  Created by YuliSun on 02/03/2017.
//  Copyright © 2017 SODEC. All rights reserved.
//

#import "BEViewController.h"

#import <Foundation/Foundation.h>
#import <BohdiEngine/MCDirector.h>
#import <BohdiEngine/MCGLEngine.h>

@interface BEViewController()
{
    float pinch_scale;
    CMAttitude* referenceAtt;
    CMMotionManager* motionManager;
}
@end

@implementation BEViewController

@synthesize useMultisampleAntiAlias;
@synthesize useDeltaRotationData;
@synthesize renderer;

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    renderer = [[BERenderer alloc] initWithFrame:self.view.frame];
    [renderer setCameraRotateMode:BECameraFixedAtOrigin];
    //[renderer setBackgroundColor:[UIColor blueColor]];
    [renderer setDoesAutoRotateCamera:NO];
    
    //set up GLKView
    [EAGLContext setCurrentContext:[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3]];
    self.glView.context = [EAGLContext currentContext];
    self.glView.context.multiThreaded = NO;
    self.glView.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    self.glView.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    self.glView.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    self.preferredFramesPerSecond = 60;
    self.glView.delegate = self;

    pinch_scale = 10.0;
    referenceAtt = nil;
}

-(void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    if (motion == UIEventSubtypeMotionShake) {
        [self onClose:nil];
    }
}

#pragma mark - Properties

-(GLKView *)glView
{
    return (GLKView*)self.view;
}

-(CGRect)glFrame
{
    return self.view.frame;
}

-(void) setUseTransparentBackground:(BOOL)useTransparentBackground
{
    if (useTransparentBackground) {
        self.view.opaque = NO;
        self.view.backgroundColor = [UIColor clearColor];
        self.modalPresentationStyle = UIModalPresentationOverCurrentContext;
        self.navigationController.modalPresentationStyle = UIModalPresentationOverCurrentContext;
    } else {
        self.view.opaque = YES;
    }
}

-(BOOL)useMultisampleAntiAlias
{
    return self.glView.drawableMultisample;
}

-(void)setUseMultisampleAntiAlias:(BOOL)useMultisampleAntiAlias
{
    if (self.glView) {
        if (self.useMultisampleAntiAlias) {
            self.glView.drawableMultisample = GLKViewDrawableMultisample4X;
        } else {
            self.glView.drawableMultisample = GLKViewDrawableMultisampleNone;
        }
    }
}

-(void)startDeviceMotion
{
    if (!motionManager) {
        motionManager = [[CMMotionManager alloc] init];
    }
    if (!motionManager.isDeviceMotionActive) {
        motionManager.deviceMotionUpdateInterval = 1.0/60.0;
        [motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryZVertical];
    }
}

-(void)stopDeviceMotion
{
    [motionManager stopDeviceMotionUpdates];
}

-(void)saveReferenceAtt
{
    CMDeviceMotion* m;
    if ((m=motionManager.deviceMotion)) {
        referenceAtt = m.attitude;
    }
    else {
        referenceAtt = nil;
    }
}

-(CMAttitude*)getDeltaAttitude
{
    if (!referenceAtt) {
        [self saveReferenceAtt];
    }
    
    CMDeviceMotion* m;
    if ((m=motionManager.deviceMotion)) {
        if (m.attitude && referenceAtt) {
            if (self.useDeltaRotationData) {
                [m.attitude multiplyByInverseOfAttitude:referenceAtt];
            }
            return m.attitude;
        }
    }
    return nil;
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    CMAttitude* att;
    if ((att=[self getDeltaAttitude])) {
        renderer.deviceRotateMat3 = att.rotationMatrix;
    }
    [renderer drawFrame];
}

-(void) glviewResize:(CGRect)frame
{
    [renderer resizeAllScene:self.view.frame.size];
}

-(void) startLoadingAnimation
{
    if (!_indicator) {
        _indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    }
    _indicator.center = self.view.center;
    [self.view addSubview:_indicator];
    [_indicator startAnimating];
}

-(void) stopLoadingAnimation
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (_indicator) {
            [_indicator stopAnimating];
            _indicator = nil;
        }
    });
}

-(void)onClose:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:^{
        
    }];
}

-(void)attachGLLayerTo:(CALayer*)superlayer
{
    [superlayer addSublayer:self.glView.layer];
}

-(void)detachGLLayer
{
    [self.glView.layer removeFromSuperlayer];
}

@end
