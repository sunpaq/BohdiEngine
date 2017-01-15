//
//  MC3DiOSDriver.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MC3DiOSDriver__
#define __monkcGame__MC3DiOSDriver__

#include <stdio.h>
#include "monkc.h"
#include "MC3DBase.h"
#include "MCUIBase.h"
#include "MCBuffer.h"

//UI parts use UIKit
void MCUIRegisterRootUIView(void* rootview);
void MCUIAddLabelButton(const char* bgname, const char* labelname, MCColor color, double x, double y, MCInt tag, MCBool isContinous);
void MCUIAddLabel(const char* labelname, MCColor color, double x, double y, MCInt tag);
void MCUILabelTextUpdate(const char* newtext, MCInt tag);
void MCUIButtonRegisterCallback(mc_message msg);

//Objective-C parts
#ifdef __OBJC__
#import <UIKit/UIKit.h>

@interface UIEventHandler : NSObject <UIGestureRecognizerDelegate>
@property (nonatomic) UIView* targetView;
@property (atomic) UIActivityIndicatorView* indicator;
@property UISwipeGestureRecognizer* swip;
@property UIPinchGestureRecognizer* pinch;
@property UIPanGestureRecognizer* pan;

- (void) onButtonClicked:(id)sender;
- (void) handleMCGLError:(const char*)errmsg;
- (void) handleMCLoading:(BOOL)startOrStop;
@end
#endif

#endif /* defined(__monkcGame__MC3DiOSDriver__) */



