//
//  BEWindowRunloop.h
//  BohdiEngine
//
//  Created by 孙御礼 on 4/23/30 H.
//

#import <Foundation/Foundation.h>

@interface BEWindowRunloop : NSObject

-(instancetype) initWithTarget:(id)target Selector:(SEL)selector OpenGLView:(NSOpenGLView*)glview;
-(void) startRunloop;
-(void) stopRunloop;

@end
