//
//  MCLock.c
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#include "MCLock.h"
#include <pthread.h>

static void lock(struct MCLock* it) {
    while (pthread_mutex_trylock(&it->mutex)) {
        //wait lock
    }
}

static void unlock(struct MCLock* it) {
    pthread_mutex_unlock(&it->mutex);
}

void MCLock(void* any) {
    struct MCLock* it = (struct MCLock*)any;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    it->mutex = mutex;
    it->lock = lock;
    it->unlock = unlock;
    it->initialized = true;
}
