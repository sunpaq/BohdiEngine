//
//  BEPanoramaView.m
//  Pods
//
//  Created by Sun YuLi on 2017/5/22.
//
//

#import "BEPanoramaView.h"

#if TARGET_OS_IOS

@implementation BEPanoramaView

- (void) loadPanoramaTexture:(NSString*)texname
{
    [super loadSkysphNamed:texname];
}

- (void) startDraw
{
    [super startDraw3DContent:BECameraRotateAroundModelByGyroscope];
}

- (void) stopDraw
{
    [super stopDraw3DContent];
}

@end

#endif
