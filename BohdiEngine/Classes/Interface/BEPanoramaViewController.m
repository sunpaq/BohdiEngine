//
//  SphereViewController.m
//  BEDemo
//
//  Created by YuliSun on 24/03/2017.
//  Copyright © 2017 SODEC. All rights reserved.
//

#import "BEPanoramaViewController.h"
#import <GLKit/GLKit.h>

@interface BEPanoramaViewController ()
{
    GLKView* glview;
}
@end

@implementation BEPanoramaViewController

@synthesize textureFileName;
@synthesize renderer;
@synthesize runloop;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    glview = [BERenderer createDefaultGLView:self.view.frame];
    glview.drawableMultisample = GLKViewDrawableMultisample4X;
    [self.view addSubview:glview];
    
    renderer = [[BERenderer alloc] initWithFrame:self.view.frame];
    [renderer addSkysphNamed:textureFileName];

    runloop = [[BERunLoop alloc] initWithTarget:self Selector:@selector(drawFrame)];
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [runloop startRunloop];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [runloop stopRunloop];
}

-(void)drawFrame
{
    [renderer drawFrameOnGLView:glview];
}

@end
