//
//  BEView.m
//  Pods
//
//  Created by Sun YuLi on 2017/5/22.
//
//

#import "BEView.h"

@implementation BEView

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
    [self addSubview:glview];
    //renderer
    renderer = [[BERenderer alloc] initWithFrame:frame];
    //runloop
    runloop = [[BERunLoop alloc] initWithTarget:self Selector:@selector(drawFrame)];
    
}

- (void)drawFrame
{
    [renderer drawFrameOnGLView:glview];
}

- (void) loadModelNamed:(NSString*)name
{
    [renderer addModelNamed:name];
}

- (void) loadSkysphNamed:(NSString*)texname
{
    [renderer addSkysphNamed:texname];
}

- (void) startDraw3DContent
{
    [runloop startRunloop];
}

- (void) stopDraw3DContent
{
    [runloop stopRunloop];
}

@end
