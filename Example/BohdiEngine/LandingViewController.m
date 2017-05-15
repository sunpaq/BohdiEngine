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
        [bec.renderer setCameraRotateMode:BECameraRotateAroundModelManual];
        [bec.renderer addModelNamed:@"monkey2.obj"];
    }];
}

-(IBAction)showPanoramaAction:(id)sender
{
    [self presentViewController:bec animated:YES completion:^{
        [bec.renderer setCameraRotateMode:BECameraRotateAroundModelByGyroscope];
        [bec.renderer addSkysphNamed:@"panorama360.jpg"];
    }];
}

@end
