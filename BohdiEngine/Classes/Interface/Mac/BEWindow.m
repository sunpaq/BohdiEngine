/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 OpenGL view subclass.
 */

#import "BEWindow.h"

@implementation BEWindow

@synthesize runloop;
@synthesize renderer;
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
    glview = [BEWindowRenderer createDefaultGLView:frame];
    if ([self.subviews count] <= 0) {
        [self addSubview:glview];
    } else {
        NSOpenGLView* view = [self.subviews objectAtIndex:0];
        if (view) {
            //[self insertSubview:glview belowSubview:view];
            [self addSubview:glview positioned:NSWindowBelow relativeTo:view];
        }
    }
    //render delegate
    _delegate = nil;
    //renderer
    renderer = [[BEWindowRenderer alloc] initWithFrame:frame];
    //runloop
    runloop = [[BEWindowRunloop alloc] initWithTarget:self Selector:@selector(drawFrame) OpenGLView:glview];
}

- (void)drawFrame
{
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
    [renderer setCameraRotateMode:rmode];
    [runloop startRunloop];
}

- (void) stopDraw3DContent
{
    [runloop stopRunloop];
}

- (void)layoutSubviews
{
    [super layoutSubtreeIfNeeded];
    glview.frame = self.bounds;
    [renderer resizeAllScene:self.bounds.size];
}

@end
