//
//  SphereViewController.h
//  BEDemo
//
//  Created by YuliSun on 24/03/2017.
//  Copyright © 2017 SODEC. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "BERenderer.h"
#import "BERunLoop.h"

@interface BEPanoramaViewController : UIViewController

@property (atomic, readwrite) NSString* textureFileName;
@property (atomic, readonly) BERenderer* renderer;
@property (atomic, readonly) BERunLoop* runloop;

@end
