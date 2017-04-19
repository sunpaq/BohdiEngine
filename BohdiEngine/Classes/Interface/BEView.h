//
//  BEView.h
//  Pods
//
//  Created by YuliSun on 18/04/2017.
//
//

#import <GLKit/GLKit.h>

@interface BEView : UIView

//@property (atomic, readwrite) BOOL useTransparentBackground;

-(void) setDefaultGLContext;

-(void) removeCurrentModel;
-(void) addModelNamed:(NSString*)modelName;

-(void) cameraReset:(float*)mat4;

//please call this manually
-(void) drawFrame;

@end
