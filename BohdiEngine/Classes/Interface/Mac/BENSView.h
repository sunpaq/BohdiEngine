#if __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_OSX

#import <Cocoa/Cocoa.h>
#import "BERenderer.h"

#define ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 1

/*
 this View originally from Apple's sample code:
 GLEssentials https://developer.apple.com/library/content/samplecode/GLEssentials/Introduction/Intro.html
 class modified from GLEssentialsGLView
 */

@protocol BENSViewRenderingDelegate
- (void) beforeRenderFrame;
- (void) afterRenderFrame;
@end

@interface BENSView : NSOpenGLView {
	CVDisplayLinkRef displayLink;
}

@property (nonatomic) id<BENSViewRenderingDelegate> delegate;
@property (atomic, readonly) BERenderer* renderer;

- (void) loadModelNamed:(NSString*)name;
- (void) loadSkysphNamed:(NSString*)texname;

- (void) startDraw3DContent:(BECameraRotateMode)rmode;
- (void) stopDraw3DContent;

@end

#endif //TARGET_OS_OSX
#endif //__APPLE__
