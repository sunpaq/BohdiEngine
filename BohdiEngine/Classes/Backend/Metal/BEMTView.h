//
//  BEMTView.h
//  BohdiEngine
//
//  Created by 孙御礼 on 5/5/30 H.
//

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>
#import "BEMTRenderer.h"

@interface BEMTView : MTKView <MTKViewDelegate>

@property BEMTRenderer* renderer;

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size;
- (void)drawInMTKView:(nonnull MTKView *)view;

@end
