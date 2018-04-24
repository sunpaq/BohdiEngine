//
//  BEWindowRunloop.h
//  BohdiEngine
//
//  Created by 孙御礼 on 4/23/30 H.
//

#import <Cocoa/Cocoa.h>

@interface BEWindowRunloop : NSObject

@property (nonatomic) id target;
@property (nonatomic) SEL selector;

-(instancetype) initWithOpenGLView:(NSOpenGLView*)glview;
-(void) startRunloop;
-(void) stopRunloop;

@end
