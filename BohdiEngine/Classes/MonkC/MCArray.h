//
// Created by 孙御礼 on 2018-12-04.
//

#ifndef MONKC_DEMO_MCARRAY_H
#define MONKC_DEMO_MCARRAY_H

#include "MCObject.h"

structure(MCArray, MCObject)
    size_t count;
    size_t maxcount;
    size_t indexLast;
    mc_generic* buff;

    fundef(addItem, void), mc_generic item);
    fundef(addTo, void), mc_generic item, size_t index);
    fundef(removeLast, void));
    fundef(removeItem, void), mc_generic* item);
    fundef(removeAt, void), size_t index);
    fundef(clear, void));
    fundef(itemAt, mc_generic*), size_t index);
    fundef(printAll, void), const char* delimiter);
    fundef(release, void));
};

constructor(MCArray), size_t maxcount);

#endif //MONKC_DEMO_MCARRAY_H
