//
//  BEGameController.h
//  BohdiEngine
//
//  Created by 孙御礼 on 2/19/30 H.
//

#import <Foundation/Foundation.h>
#import <GameController/GameController.h>

@interface BEGameController : NSObject

@property GCController* gameController;
@property BOOL conneted;

@property (readonly) CGPoint leftStick;
@property (readonly) CGPoint rightStick;
@property (readwrite) CGFloat stickFactor;

+(instancetype)shared;

-(void)startUsingGameController;

@end
