//
//  LandingViewController.m
//  BohdiEngine
//
//  Created by YuliSun on 08/03/2017.
//  Copyright © 2017 Sun YuLi. All rights reserved.
//

#import "LandingViewController.h"
#import <BohdiEngine/BEViewController.h>

@interface LandingViewController()
{
    BEViewController* bec;
}
@end

@implementation LandingViewController

-(void)viewDidLoad
{
    [super viewDidLoad];

}

//-(IBAction)showModelAction:(id)sender
//{
//    [self presentViewController:bec animated:YES completion:^{
//        [bec.renderer addModelNamed:@"monkey2.obj"];
//    }];
//}

-(IBAction)showPanoramaAction:(id)sender
{
    bec = [[BEViewController alloc] init];
    [bec.renderer setCameraRotateMode:BECameraRotateAroundModelByGyroscope];
    [self presentViewController:bec animated:YES completion:^{
        [bec.renderer addSkysphNamed:@"panorama360.jpg"];
        [bec startDeviceMotion];
    }];
}

@end
