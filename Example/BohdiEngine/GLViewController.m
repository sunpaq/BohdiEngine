//
//  GLViewController.m
//  BohdiEngine
//
//  Created by Sun YuLi on 2017/5/16.
//  Copyright © 2017年 Sun YuLi. All rights reserved.
//

#import "GLViewController.h"

@interface GLViewController ()

@end

@implementation GLViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [BEViewController willOpenModelNamed:@"monkey2.obj"];
    
    [self.renderer setCameraRotateMode:BECameraRotateAroundModelByGyroscope];
    [self.motionManager startDeviceMotion];
}

- (IBAction)onClose:(id)sender {
    [self dismissViewControllerAnimated:YES completion:^{
        
    }];
}

@end
