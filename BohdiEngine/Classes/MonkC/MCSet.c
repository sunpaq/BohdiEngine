//
//  MCSet.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/12.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSet.h"

fun(bye, void)) as(MCSet)
    if (it->values) {
        free(it->values);
    }
}

fun(insertValue, struct MCSet*), mc_generic value) as(MCSet)
    if (it->count < it->maxcount) {
        it->values[it->count++] = value;
    }
    return it;
}

constructor(MCSet), size_t maxcount) {
    MCObject(any);
    as(MCSet)
        it->values = (mc_generic*)malloc(sizeof(mc_generic)*maxcount);
        it->maxcount = maxcount;
        it->count = 0;
    }
    dynamic(MCSet)
        funbind(bye);
        funbind(insertValue);
    }
    return any;
}



fun(findAction, void)) {

}

fun(unionAction, void)) {

}

constructor(MCUnionFind)) {
    MCSet(any, 0);
    dynamic(MCUnionFind)
        funbind(findAction);
        funbind(unionAction);
    }
    return any;
}

