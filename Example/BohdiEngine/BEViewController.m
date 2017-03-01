//
//  BEViewController.m
//  BohdiEngine
//
//  Created by Sun YuLi on 01/14/2017.
//  Copyright (c) 2017 Sun YuLi. All rights reserved.
//

#import "BEViewController.h"
#import <BohdiEngine/BohdiEngine-umbrella.h>

static MCDirector* director = null;

@interface BEViewController ()
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation BEViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView* view = (GLKView*)self.view;
    view.context = self.context;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    view.drawableMultisample = GLKViewDrawableMultisampleNone;
    self.preferredFramesPerSecond = 60;
    [EAGLContext setCurrentContext:self.context];
    
    unsigned width  = (unsigned)view.bounds.size.width;
    unsigned height = (unsigned)view.bounds.size.height;
    
    //MCLogTypeSet(MC_VERBOSE);
    if (!director) {
        director = new(MCDirector);
        ff(director, setupMainScene, width, height);
    }
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        //ff(director, addModelNamed, "monkey2.obj");
        ff(director, addModelNamed, "arcticcondor.obj");
    });
}

-(void)viewDidLayoutSubviews
{
    if (director) {
        CGSize size = self.view.bounds.size;
        ff(director, resizeAllScene, (unsigned)size.width, (unsigned)size.height);
    }
}

- (void)update
{
    if (director) {
        MCDirector_updateAll(0, director, 0);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if (director) {
        MCDirector_drawAll(0, director, 0);
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
