//
//  BEGameController.m
//  BohdiEngine
//
//  Created by 孙御礼 on 2/19/30 H.
//

#import "BEGameController.h"

@implementation BEGameController

@synthesize gameController;
@synthesize conneted;
@synthesize stickFactor;

static BEGameController* _instance = nil;

+(instancetype)shared
{
    if (_instance == nil) {
        _instance = [[BEGameController alloc] init];
        _instance.gameController = NULL;
        _instance.conneted = NO;
        _instance.stickFactor = 1.0;
    }
    return _instance;
}

-(CGPoint)leftStick
{
    GCExtendedGamepad* profile = gameController.extendedGamepad;
    float x = profile.leftThumbstick.xAxis.value * stickFactor;
    float y = profile.leftThumbstick.yAxis.value * stickFactor;
    return CGPointMake(x, y);
}

-(CGPoint)rightStick
{
    GCExtendedGamepad* profile = gameController.extendedGamepad;
    float x = profile.rightThumbstick.xAxis.value * stickFactor;
    float y = profile.rightThumbstick.yAxis.value * stickFactor;
    return CGPointMake(x, y);
}

-(void)startUsingGameController
{
    //listening the controller notification
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(gameControllerFound:)
                                                 name:GCControllerDidConnectNotification object:nil];
}

-(void)gameControllerFound:(NSNotification*)noti
{
    self.gameController = noti.object;
    self.conneted = YES;
}

@end
