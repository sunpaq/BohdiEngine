#ifndef MCArrayLinkedList_h
#define MCArrayLinkedList_h

#include "MCType.h"
#include "monkc.h"

//MC array linked list
typedef struct MCALItemStruct {
    struct MCALItemStruct* next;
    struct MCALItemStruct* prev;
    mc_generic value;
    mc_generic userdata;
} MCALItem;

static MCALItem MCALItemMake(mc_generic val)
{
    return (MCALItem){null, null, val};
}

static void MCALItemLink(MCALItem* A, MCALItem* B)
{
    A->next = B;
    B->prev = A;
}

static bool MCALItemIsHead(MCALItem* item)
{
    return (item->prev == null);
}

static bool MCALItemIsTail(MCALItem* item)
{
    return (item->next == null);
}

#define MCArrayLinkedListMax 1024
typedef struct {
    bool isCircle;
    size_t count;
    MCALItem* head;
    MCALItem array[MCArrayLinkedListMax];
} MCArrayLinkedList;

static void MCALSetHead(MCArrayLinkedList* list, MCALItem* item)
{
    item->prev = null;
    list->head = item;
}

static void MCALSetTail(MCArrayLinkedList* list, MCALItem* item)
{
    item->next = null;
}

static bool MCALIsEmpty(MCArrayLinkedList* list)
{
    return (list->head == null);
}

MCArrayLinkedList* MCArrayLinkedListInit(MCArrayLinkedList* list, mc_generic values[], size_t count);
MCArrayLinkedList* MCArrayLinkedListInitCircle(MCArrayLinkedList* list, mc_generic values[], size_t count);
MCALItem* MCALDeleteItem(MCArrayLinkedList* list, MCALItem* item);

typedef struct {
    mc_generic data[MCArrayLinkedListMax];
    int nexti[MCArrayLinkedListMax];
    int previ[MCArrayLinkedListMax];
    int count;
} MCArrayList;

MCArrayList* MCArrayListInit(MCArrayList* list);
MCArrayList* MCArrayListAdd(MCArrayList* list, mc_generic data);

#endif /* MCArrayLinkedList_h */
