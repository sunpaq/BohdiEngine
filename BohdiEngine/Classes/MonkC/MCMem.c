//
//  MCMem.c
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#include <stdlib.h>

#include "MCMem.h"
#include "MCLock.h"

static struct MCLock global;

void* mc_alloc(size_t size) {
    if (size < sizeof(int)) {
        return null;
    }
    if (!global.initialized) {
        MCLock(&global);
    }
    global.lock(&global);
    void* obj = malloc(size);
    global.unlock(&global);
    return obj;
}

void mc_free(void* any) {
    if (any) {
        if (!global.initialized) {
            MCLock(&global);
        }
        global.lock(&global);
        free(any);
        global.unlock(&global);
    }
}
