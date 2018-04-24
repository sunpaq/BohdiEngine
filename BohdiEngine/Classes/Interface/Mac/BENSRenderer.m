//
//  BERenderer.m
//  Pods
//
//  Created by YuliSun on 19/04/2017.
//
//

#import "BENSRenderer.h"
#import "MCDirector.h"

@interface BENSRenderer()
{
    MCDirector* director;
    float pinch_scale;
}
@end

@implementation BENSRenderer

-(BOOL)doesAutoRotateCamera
{
    return director->lastScene->cameraAutoRotate;
}

-(BOOL)doesDrawWireFrame
{
    return computed(director, contextHandler)->drawMode == MCLineStrip? YES : NO;
}

/*
-(CMRotationMatrix) deviceRotateMat3
{
    CMRotationMatrix mat3 = {0};
    if (director) {
        mat3.m11 = director->deviceRotationMat3.m00;
        mat3.m12 = director->deviceRotationMat3.m01;
        mat3.m13 = director->deviceRotationMat3.m02;
        
        mat3.m21 = director->deviceRotationMat3.m10;
        mat3.m22 = director->deviceRotationMat3.m11;
        mat3.m23 = director->deviceRotationMat3.m12;
        
        mat3.m31 = director->deviceRotationMat3.m20;
        mat3.m32 = director->deviceRotationMat3.m21;
        mat3.m33 = director->deviceRotationMat3.m22;
    }
    return mat3;
}
*/

-(void)setDoesAutoRotateCamera:(BOOL)doesAutoRotateCamera
{
    director->lastScene->cameraAutoRotate = doesAutoRotateCamera? true : false;
}

-(void)setDoesDrawWireFrame:(BOOL)doesDrawWireFrame
{
    computed(director, contextHandler)->drawMode = doesDrawWireFrame ? MCLineStrip : MCTriAngles;
}

/*
-(void) setDeviceRotateMat3:(CMRotationMatrix)mat3
{
    if (director) {
        director->deviceRotationMat3.m00 = mat3.m11;
        director->deviceRotationMat3.m01 = mat3.m12;
        director->deviceRotationMat3.m02 = mat3.m13;

        director->deviceRotationMat3.m10 = mat3.m21;
        director->deviceRotationMat3.m11 = mat3.m22;
        director->deviceRotationMat3.m12 = mat3.m23;

        director->deviceRotationMat3.m20 = mat3.m31;
        director->deviceRotationMat3.m21 = mat3.m32;
        director->deviceRotationMat3.m22 = mat3.m33;
    }
}
*/

+(void) createFramebuffersWithContext:(NSOpenGLContext*)ctx AndLayer:(NSOpenGLLayer*)lyr
{
    GLsizei width  = lyr.bounds.size.width;
    GLsizei height = lyr.bounds.size.height;
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    GLuint colorRenderbuffer;
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    
    //[ctx renderbufferStorage:GL_RENDERBUFFER fromDrawable:lyr];
    
    glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    GLuint depthRenderbuffer;
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    GLenum status = glCheckFramebufferStatus(GL_RENDERBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"BEView - failed to make complete framebuffer");
        exit(-1);
    }
}

+(NSOpenGLView*) createDefaultGLView:(CGRect)frame
{    
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
        0
    };
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)
                                                 pixelFormat:pf];
    [view setOpenGLContext:context];
    [view setPixelFormat:pf];
    [view setWantsBestResolutionOpenGLSurface:YES];
    
    // The reshape function may have changed the thread to which our OpenGL
    // context is attached before prepareOpenGL and initGL are called.  So call
    // makeCurrentContext to ensure that our OpenGL context current to this
    // thread (i.e. makeCurrentContext directs all OpenGL calls on this thread
    // to [self openGLContext])
    [context makeCurrentContext];
    
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [context setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    return view;
}

-(instancetype) initWithFrame:(CGRect)frame
{
    if (self = [super init]) {
        pinch_scale = 10.0;
        director = new(MCDirector);
        CGFloat scale = [NSScreen mainScreen].backingScaleFactor;
        MCDirector_setupMainScene(director,
                                  frame.size.width * scale,
                                  frame.size.height * scale);
        
        computed(director, cameraHandler)->rotateMode = MCCameraRotateAroundModelManual;
        
        //NSColor* color1 = [NSColor colorWithCalibratedWhite:0.65 alpha:1.0];
        //NSColor* color2 = [color1 colorUsingColorSpace:NSColorSpace.deviceRGBColorSpace];
        
        //[self setBackgroundColor:NSColor.blueColor];
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if (director) {
        release(director);
        director = null;
    }
}

-(instancetype) setCameraRotateMode:(BECameraRotateMode)rmode
{
    if (director) {
        computed(director, cameraHandler)->rotateMode = (MCCameraRotateMode)rmode;
    }
    return self;
}

-(instancetype) setBackgroundColor:(NSColor*)color
{
    if (director) {
        CGFloat red, green, blue;
        [color getRed:&red green:&green blue:&blue alpha:nil];
        MCDirector_setBackgroudColor(director, red, green, blue, 1.0);
    }
    return self;
}

-(instancetype) resizeAllScene:(CGSize)frameSize
{
    if (director) {
        CGFloat scale = [NSScreen mainScreen].backingScaleFactor;
        MCDirector_resizeAllScene(director, (int)frameSize.width * scale, (int)frameSize.height * scale);
    }
    return self;
}

-(instancetype) scissorAllScene:(CGRect)frame
{
    if (director) {
        MCDirector_scissorAllScene(director, (int)frame.origin.x, (int)frame.origin.y,
                                   (int)frame.size.width, (int)frame.size.height);
    }
    return self;
}

-(void) removeCurrentModel
{
    ff(director, removeCurrentModel, 0);
}

-(void) addModel:(NSString*)modelpath
{
    MCDirector* dir = director;
    double scale = 10.0;
    double ccwRadian = 0;
    int tag = -1;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        const char* path = [modelpath cStringUsingEncoding:NSUTF8StringEncoding];
        MC3DModel* m = MCDirector_addModelPathed(dir, path, MCFloatF(scale));
        m->tag = tag;
        MC3DModel_rotateAroundSelfAxisX(m, ccwRadian);
        MCDirector_cameraFocusOn(dir, MCVector4Make(0, -scale * 0.5, 0, scale * 2.0));
    });
}

-(void) addModelNamed:(NSString*)modelName
{
    [self addModelNamed:modelName Scale:10.0];//max size
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
        MC3DModel_rotateAroundSelfAxisX(m, ccwRadian);
        MCDirector_cameraFocusOn(dir, MCVector4Make(0, -scale * 0.5, 0, scale * 2.0));
    });
}

-(void) addSkyboxNamed:(NSArray*)texnames
{
    if (texnames) {
        int i = 0;
        const char* names[6] = {0};
        for (NSString* str in texnames) {
            const char* name = [str cStringUsingEncoding:NSUTF8StringEncoding];
            names[i++] = name;
        }
        ff(director, addSkyboxNamed, names);
    } else {
        ff(director, addSkyboxNamed, null);
    }
}

-(void) addSkysphNamed:(NSString*)texname
{
    if (texname) {
        const char* name = [texname cStringUsingEncoding:NSUTF8StringEncoding];
        MCDirector_addSkysphereNamed(director, name);
    } else {
        MCDirector_addSkysphereNamed(director, null);
    }
}

-(void) removeCurrentSkybox
{
    MCDirector_removeCurrentSkybox(director, 0);
}

-(void) removeCurrentSkysph
{
    MCDirector_removeCurrentSkysph(director, 0);
}

-(void) cameraReset:(float*)mat4
{
    [self cameraReset:mat4 isRowMajor:NO];
}

-(void) cameraReset:(float*)mat4 isRowMajor:(BOOL)rowm
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        if (mat4) {
            if (rowm) {
                cam->Super.transform = (MCMatrix4){
                    mat4[0], mat4[4], mat4[8], mat4[12],
                    mat4[1], mat4[5], mat4[9], mat4[13],
                    mat4[2], mat4[6], mat4[10], mat4[14],
                    mat4[3], mat4[7], mat4[11], mat4[15]
                };
            } else {
                cam->Super.transform = (MCMatrix4){
                    mat4[0], mat4[1], mat4[2], mat4[3],
                    mat4[4], mat4[5], mat4[6], mat4[7],
                    mat4[8], mat4[9], mat4[10], mat4[11],
                    mat4[12], mat4[13], mat4[14], mat4[15]
                };
            }
            //MCMatrix4Copy(mat4, &cam->Super.transform, 0.00);
        } else {
            cam->Super.transform = MCMatrix4Identity;
        }
    }
}

-(void) cameraRotate:(GLKMatrix3)mat3 Incremental:(BOOL)inc
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        MC3DNode_rotateMat3(&cam->Super, mat3.m, inc?true:false);
    }
}

-(void) cameraTranslate:(GLKVector3)vec3 Incremental:(BOOL)inc
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        MCVector3 eye = MCVector3Make(vec3.x, vec3.y, vec3.z);
        cam->R_value = MCVector3Length(eye);
        cam->eye = eye;
        MCVector3 v3 = {vec3.x, vec3.y, vec3.z};
        MC3DNode_translateVec3(&cam->Super, &v3, inc?true:false);
    }
}

-(void) cameraAspectRatioReset:(float)aspectRatio
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        cam->ratio = (double)aspectRatio;
    }
}

-(void) cameraFOVReset:(float)fov
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        cam->field_of_view = fov;
    }
}

-(void) cameraTransformWorld:(GLKMatrix4)mat4
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        MCMatrix4 m4 = MCMatrix4Make(mat4.m);
        MCCamera_transformWorld(cam, &m4);
    }
}

-(void) cameraTransformSelf:(GLKMatrix4)mat4
{
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        MCMatrix4 m4 = MCMatrix4Make(mat4.m);
        MCCamera_transformSelf(cam, &m4);
    }
}

-(void) lightReset:(GLKVector3*)pos
{
    if (!director) return;
    if (pos) {
        MCLight* light = computed(director, lightHandler);
        if (light) {
            light->lightPosition = MCVector3Make(pos->x, pos->y, pos->z);
        }
    } else {
        director->lightFollowCamera = true;
    }
}

-(void) rotateModelByPanGesture:(CGPoint)offset
{
    float x = offset.x;
    float y = offset.y;
    computed(director, cameraHandler)->tht += (y/16.0);
    computed(director, cameraHandler)->fai += -(x/9.0);
}

-(void) rotateSkysphByPanGesture:(CGPoint)offset
{
    
}

-(void) zoomModelByPinchGesture:(CGFloat)scale
{
    pinch_scale *= scale;
    pinch_scale = MAX(10.0, MIN(pinch_scale, 100.0));
    
    MCCamera* camera = computed(director, cameraHandler);
    MCCamera_distanceScale(camera, MCFloatF(20.0/pinch_scale));
}

-(void) updateModelTag:(int)tag PoseMat4D:(double*)mat4
{
    if (director) {
        MC3DScene* scene = director->lastScene;
        if (scene) {
            MCItem* iter = scene->rootnode->children->headItem;
            while (iter) {
                MC3DModel* model = cast(MC3DModel*, iter);
                if (model->tag == tag) {
                    model->Super.viewtrans = MCMatrix4MakeDouble(mat4);
                }
                iter = iter->nextItem;
            }
        }
    }
}

-(void) updateModelTag:(int)tag PoseMat4F:(float*)mat4
{
    if (director) {
        MC3DScene* scene = director->lastScene;
        if (scene) {
            MCItem* iter = scene->rootnode->children->headItem;
            while (iter) {
                MC3DModel* model = cast(MC3DModel*, iter);
                if (model->tag == tag) {
                    model->Super.viewtrans = MCMatrix4Make(mat4);
                }
                iter = iter->nextItem;
            }
        }
    }
}

-(void) drawFrame
{
    if (director) {
        MCDirector_updateAll(director, 0);
        MCDirector_drawAll(director, 0);
    }
}

-(void) drawFrame:(CGRect)viewport
{
    if (director) {
        int x = viewport.origin.x;
        int y = viewport.origin.y;
        int width = viewport.size.width;
        int height = viewport.size.height;
        MCDirector_scissorAllScene(director, x, y, width, height);
        MCDirector_updateAll(director, 0);
        MCDirector_drawAll(director, 0);
    }
}

-(void) drawFrame:(CGRect)viewport vrHeadTransform:(GLKMatrix4)head vrEyeTransform:(GLKMatrix4)eye
{
    if (director) {
        MCCamera* cam = computed(director, cameraHandler);
        if (cam) {
            GLKMatrix4 mat4 = GLKMatrix4Multiply(eye, head);
            MCMatrix4 m4 = MCMatrix4Make(mat4.m);
            MCCamera_transformWorld(cam, &m4);
        }
        
        int x = viewport.origin.x;
        int y = viewport.origin.y;
        int width = viewport.size.width;
        int height = viewport.size.height;
        MCDirector_scissorAllScene(director, x, y, width, height);
        MCDirector_updateAll(director, 0);
        MCDirector_drawAll(director, 0);
    }
}

-(void) drawFrame:(CGRect)viewport vrHeadTransform:(GLKMatrix4)head vrEyeTransform:(GLKMatrix4)eye vrFOV:(CGFloat)fov
{
    if (director) {
        MCCamera* cam = computed(director, cameraHandler);
        if (cam) {
            cam->field_of_view = (double)fov;
            GLKMatrix4 mat4 = GLKMatrix4Multiply(eye, head);
            MCMatrix4 m4 = MCMatrix4Make(mat4.m);
            MCCamera_transformWorld(cam, &m4);
        }
        
        int x = viewport.origin.x;
        int y = viewport.origin.y;
        int width = viewport.size.width;
        int height = viewport.size.height;
        MCDirector_scissorAllScene(director, x, y, width, height);
        MCDirector_updateAll(director, 0);
        MCDirector_drawAll(director, 0);
    }
}

-(void) drawFrameOnCALayer:(CALayer*)calayer
{
    if (director) {
        if (calayer) {
            [calayer display];
        }
        [self drawFrame];
    }
}

-(void) drawFrameOnGLView:(NSOpenGLView*)glview
{
    if (director) {
        if (glview) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [glview display];
            });
        }
        [self drawFrame];
    }
}

-(void) pauseDraw:(BOOL)pause
{
    if (director) {
        director->pause = pause ? true : false;
    }
}

@end
