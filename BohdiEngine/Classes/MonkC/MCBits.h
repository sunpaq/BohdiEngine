//
//  MCBits.h
//  Sapindus
//
//  Created by Sun YuLi on 2016/12/4.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCBits_h
#define MCBits_h

#include "monkc.h"
#include "MCType.h"

typedef unsigned MCBits;

MCBits MCBitsMask(unsigned bitIndex);
bool MCBitsIsSetAt(MCBits bits, unsigned bitIndex);
unsigned MCBitsSetBitNum(MCBits bits);
MCBits MCBitsXORFindOddInPairs(const MCBits* pairs, size_t count);

#endif /* MCBits_h */
