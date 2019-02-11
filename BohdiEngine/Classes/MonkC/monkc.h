//
//  monkc.h
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#ifndef monkc_h
#define monkc_h

#define alias(cls)\
typedef struct cls cls##_t

#define structure(cls, supercls)\
struct cls { struct supercls Super;

#define constructor(cls)\
void* cls(void* any

#define alloc(cls)\
mc_alloc(sizeof(struct cls))

#define as(cls)\
{ struct cls* it = (struct cls*)any;

#define let(any, cls)\
{ struct cls* it = (struct cls*)any;

#endif /* monkc_h */
