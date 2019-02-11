//
//  Object.c
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#include "MCObject.h"
#include "MCMem.h"

fun(retain, void)) as(MCObject)
    if (!it) return;
    it->ref_count++;
}

fun(release, void)) as(MCObject)
    if (!it) return;
    if (it->ref_count > 0) {
        it->ref_count--;
    }
    if (it->ref_count <= 0) {
        //call clean
        printf("free object %p\n", it);
        mc_free(it);
    }
}

fun(info, void), char* buff) as(MCObject)
    if (buff) {
        const char* cname = "";
        if (it->claz) {
            cname = it->claz->name;
        }
        sprintf(buff, "claz=%s\nref_count=%d\n", cname, it->ref_count);
    }
}

fun(responseTo, void*), const char* name) as(MCObject)
    if (name != null) {
        if (it->claz != null) {
            MCFunction f;
            if ((f = it->claz->getFunction(it->claz, name)) != null) {
                return f;
            }
            MCFunctionDouble fd;
            if ((fd = it->claz->getFunctionDouble(it->claz, name)) != null) {
                return fd;
            }
        }
    }
    return null;
}

bool MCObject_class(obj it, const char* name) {
    if (it->claz && strncmp(name, it->claz->name, strlen(name)) == 0) {
        printf("class %s already loaded\n", name);
        return false;
    } else {
        val root = it->claz;
        struct MCClass* c = MCClass_load(name);
        if (c) {
            it->claz = c;
            it->claz->super = root;
            return true;
        }
    }
    return false;
}

constructor(MCObject)) {
    if (any) {
        as(MCObject)
            it->claz = null;
            it->ref_count = 1;
            funadd(info);
            funadd(responseTo);
            funadd(retain);
            funadd(release);
        }
    }
    return any;
}
