//
//  BEResource.m
//  BohdiEngine
//
//  Created by 孙御礼 on 2/25/30 H.
//

#import "BEResource.h"

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

@end
