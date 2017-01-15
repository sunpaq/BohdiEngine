//
//  MC3DiOSDriver.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MC3DiOSDriver.h"
#import "MC3DiOS.h"
#import <pthread.h>

static UIView* _rootUIView = nil;
static UIEventHandler* _handler = nil;
static mc_message onButtonClickMsg = {nil, nil};

void MCUIRegisterRootUIView(void* rootview)
{
    _rootUIView = (__bridge UIView*)rootview;
    _handler = [UIEventHandler new];
    _handler.targetView = _rootUIView;
    
    [_handler.targetView addGestureRecognizer:_handler.pan];
    [_handler.targetView addGestureRecognizer:_handler.pinch];
    //[_handler.targetView addGestureRecognizer:_handler.swip];
}

void MCUIAddLabelButton(const char* bgname, const char* labelname, MCColor color, double x, double y, MCInt tag, MCBool isContinuous)
{
    if (_rootUIView) {
        UIButton* btn = [UIButton buttonWithType:UIButtonTypeCustom];
        [btn setTitle:[NSString stringWithCString:labelname encoding:NSUTF8StringEncoding] forState:UIControlStateNormal];
        [btn setTitleColor:[UIColor colorWithRed:color.R green:color.G blue:color.B alpha:1.0] forState:UIControlStateNormal];
        [btn sizeToFit];
        if (x > _rootUIView.bounds.size.width / 0.5f) {
            x = _rootUIView.bounds.size.width;
        }
        if (y > _rootUIView.bounds.size.height / 0.5f) {
            y = _rootUIView.bounds.size.height;
        }
        btn.center = CGPointMake(x, y);
        btn.tag = tag;
        if (isContinuous == true) {
            [btn addTarget:_handler action:@selector(onButtonClicked:) forControlEvents:UIControlEventAllTouchEvents];
        }else{
            [btn addTarget:_handler action:@selector(onButtonClicked:) forControlEvents:UIControlEventTouchDown];
        }
        [_rootUIView addSubview:btn];
    }
}

void MCUIAddLabel(const char* labelname, MCColor color, double x, double y, MCInt tag)
{
    if (_rootUIView) {
        UILabel* label = [UILabel new];
        label.text = [NSString stringWithCString:labelname encoding:NSUTF8StringEncoding];
        label.textColor = [UIColor colorWithRed:color.R/255 green:color.G/255 blue:color.B/255 alpha:1.0];
        [label sizeToFit];
        if (x > _rootUIView.bounds.size.width / 0.5f) {
            x = _rootUIView.bounds.size.width;
        }
        if (y > _rootUIView.bounds.size.height / 0.5f) {
            y = _rootUIView.bounds.size.height;
        }
        label.center = CGPointMake(x, y);
        label.tag = tag;
        [_rootUIView addSubview:label];
    }
}

void MCUILabelTextUpdate(const char* newtext, MCInt tag)
{
    UILabel* label = [_rootUIView viewWithTag:tag];
    label.text = [NSString stringWithCString:newtext encoding:NSUTF8StringEncoding];
    [label sizeToFit];
}

void MCUIButtonRegisterCallback(mc_message msg)
{
    onButtonClickMsg = msg;
}

void MCGLError(const char* errmsg)
{
    if (_handler) {
        [_handler handleMCGLError:errmsg];
    }
}

void MCGLStartLoading()
{
    if (_handler) {
        [_handler handleMCLoading:YES];
    }
}

void MCGLStopLoading()
{
    if (_handler) {
        [_handler handleMCLoading:NO];
    }
}

#ifdef __OBJC__
@implementation UIEventHandler

//using ARC no need to release members
-(instancetype)init
{
    if (self=[super init]) {
        //gesture
        self.swip = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwip:)];
        self.swip.direction = UISwipeGestureRecognizerDirectionRight | UISwipeGestureRecognizerDirectionLeft;
        self.swip.delegate = self;
        
        self.pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
        self.pinch.delegate = self;
        
        self.pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
        self.pan.delegate = self;
        
        self.indicator = nil;
        return self;
    }else{
        return nil;
    }
}

- (void) onButtonClicked:(id)sender
{
    UIButton* btn = (UIButton*)sender;
    if (onButtonClickMsg.address) {
        _push_jump(onButtonClickMsg, (MCInt)btn.tag);
    }
}

- (void) handleMCGLError:(const char*)errmsg
{
    [[[UIAlertView alloc] initWithTitle:@"ERROR"
                                message:[NSString stringWithUTF8String:errmsg]
                               delegate:self
                      cancelButtonTitle:@"OK, got it."
                      otherButtonTitles:nil, nil] show];
}

- (void) handleMCLoading:(BOOL)startOrStop
{
    if (startOrStop) {
        if (self.indicator == nil) {
            self.indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
            self.indicator.center = _rootUIView.center;
            [_rootUIView addSubview:self.indicator];
        }
        [self.indicator startAnimating];
    }else{
        if (self.indicator != nil) {
            [self.indicator stopAnimating];
        }
    }
}

- (void)onSwip:(id)sender
{
    if (sender == self.swip) {
        onGestureSwip();
    }
}

- (void)onPinch:(id)sender
{
    if (sender == self.pinch) {
        onGesturePinch(self.pinch.scale);
    }
}

- (void)onPan:(id)sender
{
    if (sender == self.pan) {
        CGPoint trans = [self.pan translationInView:self.targetView];
        onGesturePan(trans.x, trans.y);
    }
}

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
    if (gestureRecognizer == self.swip) {
        return true;
    }
    else if (gestureRecognizer == self.pinch) {
        return true;
    }
    else if (gestureRecognizer == self.pan) {
        return true;
    }
    
    return false;
}

@end
#endif /* __OBJC__ */





