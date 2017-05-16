//
//  GLView.h
//  BEDemo
//
//  Created by YuliSun on 02/03/2017.
//  Copyright © 2017 SODEC. All rights reserved.
//

#ifndef BEViewController_h
#define BEViewController_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "BEInterface.h"
#import "BERenderer.h"
#import "BEMotionManager.h"

@interface BEViewController : GLKViewController <GLKViewDelegate, UIGestureRecognizerDelegate>

@property (atomic, readwrite) BOOL useMultisampleAntiAlias;
@property (atomic, readwrite) BOOL useGyroscope;

@property (atomic, readonly) BERenderer* renderer;
@property (atomic, readonly) BEMotionManager* motionManager;

+(void)willOpenModelNamed:(NSString*)name;


@end

#endif /* BEViewController_h */
