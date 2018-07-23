//
//  BEMTView.m
//  BohdiEngine
//
//  Created by 孙御礼 on 5/5/30 H.
//

#import "BEMTView.h"

#include "TargetConditionals.h"
#if TARGET_OS_IOS
#define Rect CGRect
#else
#define Rect NSRect
#endif

@implementation BEMTView

-(void)setupContext
{
    self.device = MTLCreateSystemDefaultDevice();
    self.renderer = [[BEMTRenderer alloc] initWithMetalKitView:self];
    [self mtkView:self drawableSizeWillChange:self.drawableSize];
    self.delegate = self;
}

-(instancetype)initWithCoder:(NSCoder *)coder
{
    if (self = [super initWithCoder:coder]) {
        [self setupContext];
        return self;
    }
    return nil;
}

-(instancetype)initWithFrame:(Rect)frameRect
{
    if (self = [super initWithFrame:frameRect]) {
        [self setupContext];
        return self;
    }
    return nil;
}

-(instancetype)initWithFrame:(CGRect)frameRect device:(id<MTLDevice>)device
{
    if (self = [super initWithFrame:frameRect device:device]) {
        [self setupContext];
        return self;
    }
    return nil;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    // Save the size of the drawable as we'll pass these
    //   values to our vertex shader when we draw
    vector_uint2 vsize;
    vsize.x = size.width;
    vsize.y = size.height;
    self.renderer.viewportSize = vsize;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    [self.renderer drawFrame];
}

@end
