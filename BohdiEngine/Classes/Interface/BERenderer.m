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

-(instancetype) initWithFrame:(CGRect)frame doesOpaque:(BOOL)opaque
{
    if (self = [super init]) {
        pinch_scale = 10.0;
        director = new(MCDirector);
        MCDirector_setupMainScene(0, director, frame.size.width, frame.size.height);
        if (!opaque) {
            MCDirector_setBackgroudColor(0, director, 0, 0, 0, 0);
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

-(void) removeCurrentModel
{
    ff(director, removeCurrentModel, 0);
}

-(void) addModelNamed:(NSString*)modelName
{
    //[self startLoadingAnimation];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        const char* name = [modelName cStringUsingEncoding:NSUTF8StringEncoding];
        ff(director, addModelNamed, name, 20);
        ff(director, cameraFocusOn, MCVector4Make(0, 0, 0, 50));
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
    if (!director) return;
    MCCamera* cam = computed(director, cameraHandler);
    if (cam) {
        if (mat4) {
            MCMatrix4Copy(mat4, &cam->Super.transform);
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

-(void) drawFrame
{
    if (director) {
        MCDirector_updateAll(0, director, 0);
        MCDirector_drawAll(0, director, 0);
    }
}

@end
