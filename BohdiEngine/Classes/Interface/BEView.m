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
    EAGLContext* context;
    CAEAGLLayer* calayer;
    
    MCDirector* mcdirector;
    unsigned width;
    unsigned height;
}
@end

@implementation BEView

+(Class)layerClass
{
    return [CAEAGLLayer class];
}

-(CALayer *)layer
{
    return calayer;
}

-(void) createFramebuffersWithContext:(EAGLContext*)ctx AndLayer:(CAEAGLLayer*)lyr
{
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    GLuint colorRenderbuffer;
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, &colorRenderbuffer);
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

//-(void) setUseTransparentBackground:(BOOL)useTransparentBackground
//{
//    if (useTransparentBackground) {
//        self.opaque = NO;
//        self.backgroundColor = [UIColor clearColor];
//        
//        //MCGLEngine_enableTransparency(true);
//        MCGLEngine_setClearScreenColor((MCColorf){0.0,0.0,0.0,0.0});
//    } else {
//        self.opaque = YES;
//        
//        //MCGLEngine_enableTransparency(true);
//        MCGLEngine_setClearScreenColor((MCColorf){0.0,0.0,0.0,0.0});
//    }
//}

-(instancetype)init
{
    if (self = [super init]) {
        [self setup];
        return self;
    }
    return null;
}

-(instancetype)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder]) {
        [self setup];
        return self;
    }
    return null;
}

-(instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setup];
        return self;
    }
    return null;
}

-(void)setup
{
    width  = self.frame.size.width;
    height = self.frame.size.height;
    
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    calayer = [[CAEAGLLayer alloc] init];
    //calayer.opaque = NO;
    //calayer.opacity= 0.0;
    
    [EAGLContext setCurrentContext:context];
    [self createFramebuffersWithContext:context AndLayer:calayer];
    
    mcdirector = new(MCDirector);
    MCDirector_setupMainScene(0, mcdirector, width, height);
    
    [self addModelNamed:@"2.obj"];
    [self setupRunloop];
}

-(void)setupRunloop
{
    CADisplayLink* link = [self.window.screen displayLinkWithTarget:self selector:@selector(drawFrame)];
    [link addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    link.paused = NO;
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

-(void) drawFrame
{
    [self displayLayer:calayer];
    MCDirector_updateAll(0, mcdirector, 0);
    MCDirector_drawAll(0, mcdirector, 0);
}

@end
