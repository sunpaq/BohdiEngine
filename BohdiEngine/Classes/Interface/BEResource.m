//
//  BEResource.m
//  BohdiEngine
//
//  Created by 孙御礼 on 2/25/30 H.
//

#import "BEResource.h"
#import "MCTextureCache.h"

@implementation BEResource

@synthesize objModelNames;

static BEResource* _instance = nil;

+(instancetype)shared
{
    if (_instance == nil) {
        _instance = [[BEResource alloc] init];
        _instance.objModelNames = [NSMutableArray arrayWithCapacity:5];
    }
    return _instance;
}

-(void) detectObjModelNames
{
    NSArray<NSURL*>* paths =[[NSBundle mainBundle] URLsForResourcesWithExtension:@"obj" subdirectory:nil];
    for (NSURL* path in paths) {
        NSString* name = [path lastPathComponent];
        [objModelNames addObject:name];
    }
}

-(void) preloadJPGTextures
{
    [self preloadTextures:@"jpg"];
}

-(void) preloadTGATextures
{
    [self preloadTextures:@"tga"];
}

-(void) preloadTextures:(NSString*)extension
{
    //Monk-C
    MCTextureCache* tcache = MCTextureCache_shared(0);
    //Obj-C
    NSArray<NSURL*>* paths =[[NSBundle mainBundle] URLsForResourcesWithExtension:extension subdirectory:nil];
    for (NSURL* path in paths) {
        NSString* name = [path lastPathComponent];
        const char* cname = [name cStringUsingEncoding:NSUTF8StringEncoding];
        MCTexture* tex = MCTexture_initWithFileName(new(MCTexture), cname);
        MCTextureCache_cacheTextureNamed(tcache, tex, cname);
        release(tex);
    }
}

@end
