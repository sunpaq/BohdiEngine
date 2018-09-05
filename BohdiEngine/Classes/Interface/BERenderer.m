//
//  BERenderer.m
//  Pods
//
//  Created by YuliSun on 19/04/2017.
//
//

#import "BERenderer.h"

#include "../Engine/MCDirector.h"
#include "../Backend/OpenGL/MCGLRenderer.h"
#include "TargetConditionals.h"

@interface BERenderer()
{
    MCDirector* director;
    MCObject* renderer;
    float pinch_scale;
}
@end

@implementation BERenderer

-(BOOL)doesAutoRotateCamera
{
    return director->lastScene->cameraAutoRotate;
}

-(BOOL)doesDrawWireFrame
{
    if (renderer) {
        MCDrawMode mode = (MCDrawMode)ff(renderer, getDrawMode, 0);
        return mode == MCLineStrip? YES : NO;
    }
    return NO;
}

#if TARGET_OS_IOS
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
    
+(void) createFramebuffersWithContext:(EAGLContext*)ctx AndLayer:(CAEAGLLayer*)lyr
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
    [ctx renderbufferStorage:GL_RENDERBUFFER fromDrawable:lyr];
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

+(GLKView*) createDefaultGLView:(CGRect)frame
{
    EAGLContext* ctx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    ctx.multiThreaded = YES;
    
    [EAGLContext setCurrentContext:ctx];
    GLKView* glview = [[GLKView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height) context:ctx];
    
    glview.enableSetNeedsDisplay = YES;
    glview.opaque = NO;
    
    glview.drawableColorFormat   = GLKViewDrawableColorFormatRGBA8888;
    glview.drawableDepthFormat   = GLKViewDrawableDepthFormat16;
    glview.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    
    return glview;
}
#endif

-(void)setDoesAutoRotateCamera:(BOOL)doesAutoRotateCamera
{
    director->lastScene->cameraAutoRotate = doesAutoRotateCamera? true : false;
}

-(void)setDoesDrawWireFrame:(BOOL)doesDrawWireFrame
{
    if (renderer) {
        MCDrawMode mode = doesDrawWireFrame ? MCLineStrip : MCTriAngles;
        ff(renderer, setDrawMode, mode);
    }
}

-(instancetype) initWithFrame:(CGRect)frame
{
    if (self = [super init]) {
        pinch_scale = 10.0;
        director = new(MCDirector);
        renderer = (MCObject*)MCGLRenderer_initWithDefaultShader(new(MCGLRenderer), 0);
        MCDirector_setupMainScene(director,
                                  frame.size.width * ScreenScale,
                                  frame.size.height * ScreenScale);
        
        computed(director, cameraHandler)->rotateMode = MCCameraRotateAroundModelManual;
        [self setBackgroundColor:[Color darkGrayColor]];
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

-(instancetype) setBackgroundColor:(Color*)color
{
    if (director) {
        CGFloat red, green, blue, alpha;
#if TARGET_OS_IOS
        [color getRed:&red green:&green blue:&blue alpha:&alpha];
#else
        NSColorSpace* csp = [[NSColorSpace alloc] initWithCGColorSpace:CGColorSpaceCreateDeviceRGB()];
        color = [color colorUsingColorSpace:csp];
        red = [color redComponent];
        green = [color greenComponent];
        blue = [color blueComponent];
        alpha = [color alphaComponent];
#endif
        MCDirector_setBackgroudColor(director, red, green, blue, alpha);
    }
    return self;
}

-(instancetype) resizeAllScene:(CGSize)frameSize
{
    if (director) {
        CGFloat scale = ScreenScale;
        MCDirector_resizeAllScene(director, (int)frameSize.width * scale, (int)frameSize.height * scale);
    }
    return self;
}

-(instancetype) scissorAllScene:(CGRect)frame
{
    int x = (int)frame.origin.x;
    int y = (int)frame.origin.y;
    int w = (int)frame.size.width;
    int h = (int)frame.size.height;
    
    if (renderer) {
        ff(renderer, scissorAllScene, x, y, w, h);
    }
    if (director) {
        MCDirector_resizeAllScene(director, w, h);
    }
    return self;
}

-(void) removeCurrentModel
{
    ff(director, removeCurrentModel, 0);
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

//mix(void, updateScene, MC3DScene* scene);
//mix(void, drawScene, MC3DScene* scene);
-(void) drawFrame
{
    if (director && renderer) {
        MCDirector_updateAll(director, 0);
        ff(renderer, updateScene, director->lastScene);
        ff(renderer, drawScene, director->lastScene);
    }
}

-(void) drawFrame:(CGRect)viewport
{
    if (director && renderer) {
        int x = viewport.origin.x;
        int y = viewport.origin.y;
        int width = viewport.size.width;
        int height = viewport.size.height;
        
        ff(renderer, scissorAllScene, x, y, width, height);
        MCDirector_updateAll(director, 0);
        ff(renderer, updateScene, director->lastScene);
        ff(renderer, drawScene, director->lastScene);
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
        
        ff(renderer, scissorAllScene, x, y, width, height);
        MCDirector_updateAll(director, 0);
        ff(renderer, updateScene, director->lastScene);
        ff(renderer, drawScene, director->lastScene);
    }
}

-(void) drawFrame:(CGRect)viewport vrHeadTransform:(GLKMatrix4)head vrEyeTransform:(GLKMatrix4)eye vrFOV:(CGFloat)fov
{
    if (director && renderer) {
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
        
        ff(renderer, scissorAllScene, x, y, width, height);
        MCDirector_updateAll(director, 0);
        ff(renderer, updateScene, director->lastScene);
        ff(renderer, drawScene, director->lastScene);
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

-(void) drawFrameOnGLView:(GLView*)glview
{
    if (director) {
        if (glview) {
            [glview display];
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
