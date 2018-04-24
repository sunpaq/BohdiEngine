#import <Cocoa/Cocoa.h>
#import "BENSRenderer.h"

#define ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 1

@protocol BENSViewRenderingDelegate
- (void) beforeRenderFrame;
- (void) afterRenderFrame;
@end

@interface BENSView : NSOpenGLView {
	CVDisplayLinkRef displayLink;
}

@property (nonatomic) id<BENSViewRenderingDelegate> delegate;
@property (atomic, readonly) BENSRenderer* renderer;

- (void) loadModelNamed:(NSString*)name;
- (void) loadSkysphNamed:(NSString*)texname;

- (void) startDraw3DContent:(BECameraRotateMode)rmode;
- (void) stopDraw3DContent;

@end
