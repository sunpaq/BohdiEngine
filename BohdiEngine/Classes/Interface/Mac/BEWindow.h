/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 OpenGL view subclass.
 */


#import <Cocoa/Cocoa.h>
#import "BEWindowRenderer.h"
#import "BEWindowRunloop.h"

@protocol BEWindowRenderingDelegate
- (void) beforeRenderFrame;
- (void) afterRenderFrame;
@end

@interface BEWindow : NSView

@property (nonatomic) id<BEWindowRenderingDelegate> delegate;

@property (atomic, readonly) NSOpenGLView* glview;
@property (atomic, readonly) BEWindowRunloop* runloop;
@property (atomic, readonly) BEWindowRenderer* renderer;

@end
