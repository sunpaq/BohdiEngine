//
//  MCType.c
//  monkc3
//
//  Created by 孙御礼 on 9/8/30 H.
//  Copyright © 30 Heisei oreisoft. All rights reserved.
//

#include "MCType.h"

mc_generic gen_i(int i) {
    return (mc_generic){.i = i};
}

mc_generic gen_l(long l) {
    return (mc_generic){.l = l};
}

mc_generic gen_f(float f) {
    return (mc_generic){.f = f};
}

mc_generic gen_d(double d) {
    return (mc_generic){.d = d};
}

mc_generic gen_p(void* p) {
    return (mc_generic){.p = p};
}

mc_generic gen_o(struct MCObject* o) {
    return (mc_generic){.mcobject = o};
}

int MCGenericCompare(mc_generic A, mc_generic B) {
    if (A.f > B.f) {
        return 1;
    }
    else if (A.f < B.f) {
        return -1;
    }
    //A == B
    return 0;
}
