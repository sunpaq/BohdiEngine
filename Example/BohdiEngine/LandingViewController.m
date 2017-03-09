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
    
    [bec setGLViewFrame:self.view.frame];
}

-(IBAction)onEngineStartBtnClicked:(id)sender
{
    
    [self presentViewController:bec animated:YES completion:^{
        
        //bec.useDeltaRotationData = YES;
        //[bec addSkyboxNamed:nil];
        //[bec addSkysphNamed:@"panorama360.jpg"];
        [bec addSkysphNamed:@"wood.jpg"];

        //[bec addModelNamed:@"monkey2.obj"];
    }];
}

@end
