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
    UIActivityIndicatorView* _indicator;
}
@end

@implementation BEViewController

@synthesize useMultisampleAntiAlias;
@synthesize useGyroscope;
@synthesize renderer;
@synthesize motionManager;

static NSString* filename = nil;
+(void)willOpenModelNamed:(NSString*)name
{
    filename = name;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    //must set up GLKView first
    [EAGLContext setCurrentContext:[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3]];
    self.glView.context = [EAGLContext currentContext];
    self.glView.context.multiThreaded = NO;
    self.glView.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    self.glView.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    self.glView.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    self.preferredFramesPerSecond = 60;
    self.glView.delegate = self;
    
    renderer = [[BERenderer alloc] initWithFrame:self.view.frame];
    [renderer setDoesAutoRotateCamera:NO];
    [renderer setCameraRotateMode:BECameraRotateAroundModelByGyroscope];
    self.useGyroscope = YES;

    motionManager = [BEMotionManager shared];
    pinch_scale = 10.0;
}

-(void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    [renderer resizeAllScene:self.view.frame.size];
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (filename) {
        [renderer addModelNamed:filename];
    }
}

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

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if (useGyroscope) {
        CMAttitude* att = nil;
        if ((att=[motionManager getDeltaAttitude]) != nil) {
            renderer.deviceRotateMat3 = att.rotationMatrix;
        }
    }
    [renderer drawFrame];
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

@end
