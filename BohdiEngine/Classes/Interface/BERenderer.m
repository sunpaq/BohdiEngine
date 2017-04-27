//
//  BERenderer.m
//  Pods
//
//  Created by YuliSun on 19/04/2017.
//
//

#import "BERenderer.h"
#import "MCDirector.h"

@interface BERenderer()
{
    MCDirector* director;
    float pinch_scale;
}
@end

@implementation BERenderer

+(GLKView*) createDefaultGLView:(CGRect)frame
{
    EAGLContext* ctx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    ctx.multiThreaded = NO;
    [EAGLContext setCurrentContext:ctx];
    GLKView* glview = [[GLKView alloc] initWithFrame:frame context:[EAGLContext currentContext]];
    
    glview.enableSetNeedsDisplay = YES;
    glview.opaque = NO;
    
    glview.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    glview.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    glview.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    
    return glview;
}

-(instancetype) initWithFrame:(CGRect)frame doesOpaque:(BOOL)opaque
{
    return [self initWithFrame:frame doesOpaque:opaque cameraRotateMode:BECameraRotateAroundModelManual];
}

-(instancetype) initWithFrame:(CGRect)frame doesOpaque:(BOOL)opaque cameraRotateMode:(BECameraRotateMode)rmode
{
    if (self = [super init]) {
        pinch_scale = 10.0;
        director = new(MCDirector);
        CGFloat scale = [UIScreen mainScreen].scale;
        MCDirector_setupMainScene(0, director, frame.size.width * scale,
                                  frame.size.height * scale);
        
        computed(director, cameraHandler)->rotateMode = rmode;
        if (rmode == BECameraRotateAroundModelByGyroscope) {
            director->gyroscopeMode = true;
        } else {
            director->gyroscopeMode = false;
        }
        
        if (!opaque) {
            MCDirector_setBackgroudColor(0, director, 0, 0, 0, 0);
        } else {
            MCDirector_setBackgroudColor(0, director, 0.05, 0.25, 0.35, 1.0);
        }
        
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if (director) {
        release(director);
        director = nil;
    }
}

-(void) resizeAllScene:(CGSize)frameSize
{
    if (director) {
        CGFloat scale = [UIScreen mainScreen].scale;
        MCDirector_resizeAllScene(0, director, (int)frameSize.width * scale, (int)frameSize.height * scale);
    }
}

-(void) removeCurrentModel
{
    ff(director, removeCurrentModel, 0);
}

-(void) addModelNamed:(NSString*)modelName
{
    [self addModelNamed:modelName Scale:5.0];//5cm
}

-(void) addModelNamed:(NSString*)modelName Scale:(double)scale
{
    [self addModelNamed:modelName Scale:scale RotateX:M_PI_2];
}

-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian
{
    [self addModelNamed:modelName Scale:scale RotateX:ccwRadian Index:-1];
}

-(void) addModelNamed:(NSString*)modelName Scale:(double)scale RotateX:(double)ccwRadian Index:(int)index
{
    //[self startLoadingAnimation];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        const char* name = [modelName cStringUsingEncoding:NSUTF8StringEncoding];
        //ff(director, addModelNamed, name, scale);
        //ff(director, cameraFocusOn, MCVector4Make(0, 0, 0, 50));
        MC3DModel* m = MCDirector_addModelNamedAtIndex(0, director, name, MCFloatF(scale), index);
        MC3DModel_rotateAroundSelfAxisX(0, m, ccwRadian);
        MCDirector_cameraFocusOn(0, director, MCVector4Make(0, 0, 0, 50));
        //[self stopLoadingAnimation];
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
        MCDirector_addSkysphereNamed(0, director, name);
    } else {
        MCDirector_addSkysphereNamed(0, director, null);
    }
}

-(void) removeCurrentSkybox
{
    MCDirector_removeCurrentSkybox(0, director, 0);
}

-(void) removeCurrentSkysph
{
    MCDirector_removeCurrentSkysph(0, director, 0);
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
        MC3DNode_rotateMat3(0, &cam->Super, mat3.m, inc?true:false);
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
        MC3DNode_translateVec3(0, &cam->Super, &v3, inc?true:false);
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

-(void) handlePanGesture:(CGPoint)offset
{
    float x = offset.x;
    float y = offset.y;
    computed(director, cameraHandler)->tht += (y/16.0);
    computed(director, cameraHandler)->fai += -(x/9.0);
}

-(void) handlePinchGesture:(float)scale
{
    pinch_scale *= scale;
    pinch_scale = MAX(10.0, MIN(pinch_scale, 100.0));
    
    MCCamera* camera = computed(director, cameraHandler);
    MCCamera_distanceScale(0, camera, MCFloatF(20.0/pinch_scale));
}

-(void) updateModelAt:(int)index PoseMat4D:(double*)mat4
{
    if (director) {
        MC3DScene* scene = director->lastScene;
        if (scene) {
            MCItem* item = MCLinkedList_itemAtIndex(0, scene->rootnode->children, index);
            if (item) {
                MC3DModel* model = cast(MC3DModel*, item);
                model->Super.viewtrans = MCMatrix4MakeDouble(mat4);
            }
        }
    }
}

-(void) updateModelAt:(int)index PoseMat4F:(float*)mat4
{
    if (director) {
        MC3DScene* scene = director->lastScene;
        if (scene) {
            MCItem* item = MCLinkedList_itemAtIndex(0, scene->rootnode->children, index);
            if (item) {
                MC3DModel* model = cast(MC3DModel*, item);
                model->Super.viewtrans = MCMatrix4Make(mat4);
            }
        }
    }
}

-(void) drawFrame
{
    if (director) {
        MCDirector_updateAll(0, director, 0);
        MCDirector_drawAll(0, director, 0);
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

-(void) drawFrameOnGLView:(GLKView*)glview
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
