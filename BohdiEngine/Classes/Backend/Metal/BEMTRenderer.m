//
//  BEMTRenderer.m
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#import "BEMTRenderer.h"
#import "BEMTShaderTypes.h"

#import "MCCube.h"

@implementation BEMTRenderer

-(instancetype) initWithMTKView:(MTKView*)view
{
    if (self = [super init]) {
        self->context = [[BEMTContext alloc] initWithMetalKitView:view];
        return self;
    }
    return NULL;
}

static MCCube* cube = NULL;

-(void) drawFrame
{
    if (cube == NULL) {
        cube = new(MCCube);
    }
    [self drawNode:((MC3DNode*)cube)];
}

-(void) drawScene:(MC3DScene*)scene
{

}

-(void) drawNode:(MC3DNode*)node
{
    MCLinkedListForEach(node->meshes,
                        MCMesh* mesh = (MCMesh*)item;
                        if (mesh != null) {
                            [self drawMesh:mesh];
                        })

}

-(void) drawMesh:(MCMesh*)mesh
{
    // Create a new command buffer for each render pass to the current drawable
    id<MTLCommandBuffer> commandBuffer = [[context commandQueue] commandBuffer];
    commandBuffer.label = @"MyCommand";
    
    // Obtain a renderPassDescriptor generated from the view's drawable textures
    MTLRenderPassDescriptor *renderPassDescriptor = [context getCurrentRenderPassDescriptor];
    
    if(renderPassDescriptor != nil)
    {
        // Create a render command encoder so we can render into something
        id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";
        
        // Set the region of the drawable to which we'll draw.
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, [context viewportSize].x, [context viewportSize].y, -1.0, 1.0 }];
        [renderEncoder setRenderPipelineState:[context pipelineState]];
        
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
                               length:mesh->vertexDataSize
                              atIndex:AAPLVertexInputIndexVertices];
        
        // You send a pointer to `_viewportSize` and also indicate its size
        // The `AAPLVertexInputIndexViewportSize` enum value corresponds to the
        // `viewportSizePointer` argument in the `vertexShader` function because its
        //  buffer attribute also uses the `AAPLVertexInputIndexViewportSize` enum value
        //  for its index
        vector_uint2 size = [context viewportSize];
        
        [renderEncoder setVertexBytes:&size
                               length:sizeof(size)
                              atIndex:AAPLVertexInputIndexViewportSize];
        
        // Draw the 3 vertices of our triangle
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                          vertexStart:0
                          vertexCount:mesh->vertexCount];
        
        [renderEncoder endEncoding];
        
        // Schedule a present once the framebuffer is complete using the current drawable
        [commandBuffer presentDrawable:[context getCurrentDrawable]];
    }
    
    // Finalize rendering here & push the command buffer to the GPU
    [commandBuffer commit];
}

@end
