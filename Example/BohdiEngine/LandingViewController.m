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
    bec = [BEViewController new];
    [bec glviewResize:self.view.frame];
}

-(IBAction)showModelAction:(id)sender
{
    [self presentViewController:bec animated:YES completion:^{
        bec.useTransparentBackground = NO;
        bec.cameraRotateMode = BECameraRotateAroundModelManual;
        [bec addModelNamed:@"monkey2.obj" ];
    }];
}

-(IBAction)showPanoramaAction:(id)sender
{
    [self presentViewController:bec animated:YES completion:^{
        bec.cameraRotateMode = BECameraRotateAroundModelByGyroscope;
        [bec addSkysphNamed:@"panorama360.jpg"];
    }];
}

@end
