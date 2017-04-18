//
//  BEView.m
//  Pods
//
//  Created by YuliSun on 18/04/2017.
//
//

#import "BEView.h"
#import "MCDirector.h"
#import "MCGLEngine.h"

@interface BEView()
{
    MCDirector* mcdirector;
}
@end

@implementation BEView

-(void) setUseTransparentBackground:(BOOL)useTransparentBackground
{
    if (useTransparentBackground) {
        self.opaque = NO;
        self.backgroundColor = [UIColor clearColor];
        
        //MCGLEngine_enableTransparency(true);
        MCGLEngine_setClearScreenColor((MCColorf){0.0,0.0,0.0,0.0});
    } else {
        self.opaque = YES;
        
        //MCGLEngine_enableTransparency(true);
        MCGLEngine_setClearScreenColor((MCColorf){0.0,0.0,0.0,0.0});
    }
}

-(instancetype)init
{
    if (self = [super init]) {
        [self setupBohdiEngine];
        return self;
    }
    return null;
}

-(instancetype)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder]) {
        [self setupBohdiEngine];
        return self;
    }
    return null;
}

-(instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setDefaultGLContext];
        [self setupBohdiEngine];
        return self;
    }
    return null;
}

-(instancetype)initWithFrame:(CGRect)frame context:(EAGLContext *)context
{
    if (self = [super initWithFrame:frame context:context]) {
        [self setupBohdiEngine];
        return self;
    }
    return null;
}

-(void) setDefaultGLContext
{
    EAGLContext* ctx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (ctx) {
        self.context = ctx;
        self.context.multiThreaded = NO;
        self.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
        self.drawableDepthFormat = GLKViewDrawableDepthFormat16;
        self.drawableStencilFormat = GLKViewDrawableStencilFormat8;
        [EAGLContext setCurrentContext:ctx];
        [self bindDrawable];
    }
}

-(void)setupBohdiEngine
{
    //assume the frame have valid size now
    unsigned width  = self.frame.size.width;
    unsigned height = self.frame.size.height;
    
    if (self.context) {
        mcdirector = new(MCDirector);
        MCDirector_setupMainScene(0, mcdirector, width, height);
    } else {
        NSLog(@"BEView no GL context found when setup BohdiEngine");
    }
}

-(void)dealloc
{
    release(mcdirector);
    mcdirector = null;
}

-(void) removeCurrentModel
{
    ff(mcdirector, removeCurrentModel, 0);
}

-(void) addModelNamed:(NSString*)modelName
{
    //[self startLoadingAnimation];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        const char* name = [modelName cStringUsingEncoding:NSUTF8StringEncoding];
        ff(mcdirector, addModelNamed, name, 20);
        ff(mcdirector, cameraFocusOn, MCVector4Make(0, 0, 0, 50));
        //[self stopLoadingAnimation];
    });
}

-(void) cameraReset:(float*)mat4
{
    if (!mcdirector) return;
    MCCamera* cam = computed(mcdirector, cameraHandler);
    if (cam) {
        if (mat4) {
            MCMatrix4Copy(mat4, &cam->Super.transform);
        } else {
            cam->Super.transform = MCMatrix4Identity;
        }
    }
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    MCDirector_updateAll(0, mcdirector, 0);
    MCDirector_drawAll(0, mcdirector, 0);
}

@end
