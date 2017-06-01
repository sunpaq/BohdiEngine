//
//  BEModelCache.m
//  Pods
//
//  Created by YuliSun on 01/06/2017.
//
//

#import "BEModelCache.h"
#import "MC3DModel.h"

@interface BEModelCache ()
{
    NSMutableDictionary* models;
}
@end

@implementation BEModelCache

static BEModelCache* _instance = NULL;

+(instancetype)shared
{
    if (!_instance) {
        _instance = [[BEModelCache alloc] init];
        _instance->models = [[NSMutableDictionary alloc] init];
    }
    return _instance;
}

-(void) loadModelNamed:(NSString*)name
{
    
}

@end
