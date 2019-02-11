//
//  MCType.h
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#ifndef MCType_h
#define MCType_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#define null NULL
#define NUL '\0'
#define K(key) #key
#define F(value) (void*(*)(void))value

#define def(any, type) struct type* any
#define cast(any, type) ((struct type*)any)

#define cpt(name) it->name(it)
#define computed(any, name) any->name(any)

typedef void* var;
typedef void* val;

typedef union {
    double d;
    float  f;
    void*  p;
    long   l;
    int    i;
    bool mcbool;
    size_t mcsizet;
    struct MCObject* mcobject;
} mc_generic;

mc_generic gen_i(int i);
mc_generic gen_l(long l);
mc_generic gen_f(float f);
mc_generic gen_d(double d);
mc_generic gen_p(void* p);
mc_generic gen_o(struct MCObject* o);

int MCGenericCompare(mc_generic A, mc_generic B);

#endif /* MCType_h */
