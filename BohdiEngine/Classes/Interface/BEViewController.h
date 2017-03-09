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
#import <CoreMotion/CoreMotion.h>

@interface BEViewController : GLKViewController <GLKViewDelegate>

@property (atomic, readwrite) BOOL useDeltaRotationData;
@property (atomic, readwrite) CMRotationMatrix deviceRotateMat3;
@property (nonatomic, strong) UIActivityIndicatorView* indicator;

-(void) addModelNamed:(NSString*)modelName;

//use default if names/name is null
-(void) addSkyboxNamed:(NSArray*)texnames;
-(void) addSkysphNamed:(NSString*)texname;

-(void) setRotateCamera:(BOOL)doesRotate;
-(void) setDrawWireFrame:(BOOL)doesDrawWF;

-(void) setGLViewFrame:(CGRect)frame;
-(void) setTransparentBG;


@end


#endif /* BEViewController_h */
