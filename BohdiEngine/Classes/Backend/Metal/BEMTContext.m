//
//  BEMTContext.m
//  BohdiEngine
//
//  Created by 孙御礼 on 5/5/30 H.
//

#import "BEMTContext.h"
#import "BEMTShaderTypes.h"

static const AAPLVertex triangleVertices[] =
{
    // 2D positions,    RGBA colors
    { { -300,   500, 100 }, { 0, 0, 1, 1 } },
    { {  250,  -250, 200 }, { 1, 0, 0, 1 } },
    { { -250,  -250, 400 }, { 0, 1, 0, 1 } },
    { {    0,   250, -600 }, { 0, 0, 1, 1 } },
};

static const vcount = 4;

@implementation BEMTContext

- (MTLRenderPassDescriptor*)getCurrentRenderPassDescriptor {
    return _view.currentRenderPassDescriptor;
}

- (id <CAMetalDrawable>)getCurrentDrawable {
    return _view.currentDrawable;
}

/// Initialize with the MetalKit view from which we'll obtain our Metal device
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
{
    self = [super init];
    if(self)
    {
        NSError *error = NULL;
        
        _view = mtkView;
        _device = mtkView.device;
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibraryWithBundle:[NSBundle bundleForClass:[BEMTContext class]] error:&error];
        if (error) {
            NSLog(@"load shader error %@", error.description);
        }
        
        // Load the vertex function from the library
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        
        // Load the fragment function from the library
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];
        
        // Configure a pipeline descriptor that is used to create a pipeline state
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        if (!_pipelineState)
        {
            // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
            //  If the Metal API validation is enabled, we can find out more information about what
            //  went wrong.  (Metal API validation is enabled by default when a debug build is run
            //  from Xcode)
            NSLog(@"Failed to created pipeline state, error %@", error);
            return nil;
        }
        
        // Create the command queue
        _commandQueue = [_device newCommandQueue];
    }
    
    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view {

}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    // Save the size of the drawable as we'll pass these
    //   values to our vertex shader when we draw
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

@end

//C APIs
void BEMTContextCreate()
{
    
}

void BEMTContextRelease()
{
    
}

void BEMTContext_loadMesh(void* meth)
{
    
}

void BEMTContext_drawMesh(void* meth)
{
    
}

