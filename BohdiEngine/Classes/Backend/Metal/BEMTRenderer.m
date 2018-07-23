//
//  BEMTRenderer.m
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#import "BEMTRenderer.h"
#import "BEMTShaderTypes.h"

//#import "MCCube.h"
#import "MCDirector.h"

@interface BEMTRenderer ()
{
    MTKView* _view;
    MCDirector* director;
}
@end

@implementation BEMTRenderer

/// Initialize with the MetalKit view from which we'll obtain our Metal device
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
{
    self = [super init];
    if(self)
    {
        float ScreenScale = 2.0;
        CGFloat w = mtkView.drawableSize.width;
        CGFloat h = mtkView.drawableSize.height;
        
        director = new(MCDirector);
        MCDirector_setupMainScene(director,
                                  w * ScreenScale,
                                  h * ScreenScale);
        
        computed(director, cameraHandler)->rotateMode = MCCameraRotateAroundModelManual;
        
        NSError *error = NULL;
        
        _view = mtkView;
        _device = mtkView.device;
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibraryWithBundle:[NSBundle bundleForClass:[BEMTRenderer class]] error:&error];
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

-(void) addModelNamed:(NSString*)modelName
{
    [self addModelNamed:modelName Scale:100.0];//max size
}

-(void) addModelNamed:(NSString*)modelName Scale:(double)scale
{
    [self addModelNamed:modelName Scale:scale RotateX:0];
}

-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian
{
    [self addModelNamed:modelName Scale:scale RotateX:ccwRadian Tag:-1];
}

-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian Tag:(int)tag
{
    MCDirector* dir = director;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        const char* name = [modelName cStringUsingEncoding:NSUTF8StringEncoding];
        MC3DModel* m = MCDirector_addModelNamed(dir, name, MCFloatF(scale));
        m->tag = tag;
        //MC3DModel_rotateAroundSelfAxisX(m, ccwRadian);
        //MCDirector_cameraFocusOn(dir, MCVector4Make(0, -scale * 0.5, 0, scale * 2.0));
    });
}

- (MTLRenderPassDescriptor*)getCurrentRenderPassDescriptor {
    return _view.currentRenderPassDescriptor;
}

- (id <CAMetalDrawable>)getCurrentDrawable {
    return _view.currentDrawable;
}

-(void) drawFrame
{
    [self drawScene:director->lastScene];
}

-(void) drawScene:(MC3DScene*)scene
{
    [self drawNode:scene->rootnode];
}

-(void) drawNode:(MC3DNode*)node
{
    //draw self meshes
    MCLinkedListForEach(node->meshes,
                        MCMesh* mesh = (MCMesh*)item;
                        if (mesh != null) {
                            [self drawMesh:mesh];
                        })

    //draw children
    MCLinkedListForEach(node->children,
                        MC3DNode* child = (MC3DNode*)item;
                        if (child != null && child->visible != false) {
                            [self drawNode:child];
                        })
}

-(void) drawMesh:(MCMesh*)mesh
{
    if (mesh == NULL) return;
    
//    static const MCVertexData triangleVertices[] =
//    {
//        // 2D positions,    RGBA colors
//        {  250,-250,0, 0,0,0, 1,0,0, 0,0 },
//        { -250,-250,0, 0,0,0, 0,1,0, 0,0 },
//        {    0, 250,0, 0,0,0, 0,0,1, 0,0 }
//    };
    
    // Create a new command buffer for each render pass to the current drawable
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";
    
    // Obtain a renderPassDescriptor generated from the view's drawable textures
    MTLRenderPassDescriptor *renderPassDescriptor = [self getCurrentRenderPassDescriptor];
    
    if(renderPassDescriptor != nil)
    {
        // Create a render command encoder so we can render into something
        id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";
        
        // Set the region of the drawable to which we'll draw.
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, self.viewportSize.x, self.viewportSize.y, -1.0, 1.0 }];
        [renderEncoder setRenderPipelineState:self.pipelineState];
        
        // We call -[MTLRenderCommandEncoder setVertexBytes:length:atIndex:] to send data from our
        //   Application ObjC code here to our Metal 'vertexShader' function
        // This call has 3 arguments
        //   1) A pointer to the memory we want to pass to our shader
        //   2) The memory size of the data we want passed down
        //   3) An integer index which corresponds to the index of the buffer attribute qualifier
        //      of the argument in our 'vertexShader' function
        
        // You send a pointer to the `triangleVertices` array also and indicate its size
        // The `AAPLVertexInputIndexVertices` enum value corresponds to the `vertexArray`
        // argument in the `vertexShader` function because its buffer attribute also uses
        // the `AAPLVertexInputIndexVertices` enum value for its index
        
        [renderEncoder setVertexBytes:mesh->vertexDataPtr
                               length:sizeof(VertexData) * mesh->vertexCount
                              atIndex:AAPLVertexInputIndexVertices];
        
        // You send a pointer to `_viewportSize` and also indicate its size
        // The `AAPLVertexInputIndexViewportSize` enum value corresponds to the
        // `viewportSizePointer` argument in the `vertexShader` function because its
        //  buffer attribute also uses the `AAPLVertexInputIndexViewportSize` enum value
        //  for its index
        vector_uint2 size = [self viewportSize];
        
        [renderEncoder setVertexBytes:&size
                               length:sizeof(size)
                              atIndex:AAPLVertexInputIndexViewportSize];
        
        // Draw the 3 vertices of our triangle
        [renderEncoder drawPrimitives:MTLPrimitiveTypeLineStrip
                          vertexStart:0
                          vertexCount:mesh->vertexCount];
        
        [renderEncoder endEncoding];
        
        // Schedule a present once the framebuffer is complete using the current drawable
        [commandBuffer presentDrawable:[self getCurrentDrawable]];
    }
    
    // Finalize rendering here & push the command buffer to the GPU
    [commandBuffer commit];
}

@end
