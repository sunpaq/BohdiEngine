//
//  Object.h
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#ifndef MCObject_h
#define MCObject_h

#include "MCClass.h"
#include "MCFunction.h"

struct MCObject {
    struct MCClass* claz;
    int ref_count;
    
    //instance
    fundef(info, void), char* buff);
    fundef(responseTo, void*), const char* name);

    //mem manage
    fundef(retain, void));
    fundef(release, void));
};

constructor(MCObject));

alias(MCObject);

typedef struct MCObject* obj;

bool MCObject_class(obj it, const char* name);

#define dynamic(cls)\
if (MCObject_class(any, #cls)) { struct MCClass* claz = ((obj)any)->claz; struct cls* it = (struct cls*)any;

#define Retain(it) ((obj)it)->retain(it)
#define Release(it) ((obj)it)->release(it)

#endif /* MCObject_h */
