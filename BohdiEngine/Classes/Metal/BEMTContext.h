//
//  BEMTContext.h
//  BohdiEngine
//
//  Created by 孙御礼 on 5/5/30 H.
//

#ifndef BEMTContext_h
#define BEMTContext_h

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

@interface BEMTContext : NSObject<MTKViewDelegate>
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;
@end

//C APIs
void BEMTContextCreate(void);
void BEMTContextRelease(void);

void BEMTContext_loadMesh(void* meth);
void BEMTContext_drawMesh(void* meth);

#endif /* BEMTContext_h */
