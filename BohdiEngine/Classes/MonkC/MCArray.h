#include "MCContext.h"

#ifndef MCARRAY_H_
#define MCARRAY_H_

class(MCArray, MCObject,
	size_t count;
	size_t maxcount;
	size_t indexLast;
    MCGeneric* buff);

fun(MCArray, void, bye, voida);
fun(MCArray, MCArray*, initWithMaxCount, size_t max);
fun(MCArray, MCArray*, addItem, MCGeneric item);
fun(MCArray, MCArray*, addItemToIndex, MCGeneric item, size_t index);
fun(MCArray, MCArray*, removeLastItem, voida);
fun(MCArray, MCArray*, removeItem, MCGeneric* item);
fun(MCArray, MCArray*, removeItemByIndex, size_t index);
fun(MCArray, MCArray*, clear, voida);
fun(MCArray, MCGeneric*, getItemByIndex, size_t index);
fun(MCArray, void, printAll, const char* delimiter);

#endif /* MCARRAY_H_ */
