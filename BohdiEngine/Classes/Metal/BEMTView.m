//
//  BEMTView.m
//  BohdiEngine
//
//  Created by 孙御礼 on 5/5/30 H.
//

#import "BEMTView.h"
#import "BEMTContext.h"

#include "TargetConditionals.h"
#if TARGET_OS_IOS
#define Rect CGRect
#else
#define Rect NSRect
#endif

@implementation BEMTView
{
    BEMTContext* _ctx;
}

-(void)setupContext
{
    self.device = MTLCreateSystemDefaultDevice();
    _ctx = [[BEMTContext alloc] initWithMetalKitView:self];
    [_ctx mtkView:self drawableSizeWillChange:self.drawableSize];
    self.delegate = _ctx;
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

@end
