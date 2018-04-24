//
//  BEWindowRunloop.m
//  BohdiEngine
//
//  Created by 孙御礼 on 4/23/30 H.
//

#import "BENSRunloop.h"

@interface BEWindowRunloop()
{
    CVDisplayLinkRef displayLink;
}
@end

@implementation BEWindowRunloop

@synthesize target;
@synthesize selector;

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    // There is no autorelease pool when this method is called
    // because it will be called from a background thread.
    // It's important to create one or app can leak objects.
    @autoreleasepool {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (self.target) {
                [self.target performSelector:self.selector];
            }
        });
    }
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,
                                      const CVTimeStamp* now,
                                      const CVTimeStamp* outputTime,
                                      CVOptionFlags flagsIn,
                                      CVOptionFlags* flagsOut,
                                      void* displayLinkContext)
{
    CVReturn result = [(__bridge BEWindowRunloop*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

-(instancetype) initWithOpenGLView:(NSOpenGLView*)glview
{
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, (__bridge void*)self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[glview openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[glview pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    return self;
}

-(void) startRunloop
{
    // Activate the display link
    CVDisplayLinkStart(displayLink);
}

-(void) stopRunloop
{
    CVDisplayLinkStop(displayLink);
}

@end
