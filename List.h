#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

const char CANARY = 21;

typedef struct
{
    unsigned int allocationError : 1;
    unsigned int dataNullPtr     : 1;
    unsigned int nextNullPtr     : 1;
    unsigned int prevNullPtr     : 1;
    unsigned int listDamaged     : 1;
    unsigned int emptyCell       : 1;
} ListError;

const ListError NULL_LIST_ERROR = {};

typedef struct
{
    char beginCanary;

    int* data;
    int* next;
    int* prev;

    int free;

    unsigned int capacity;

//    unsigned int curentIndex;

    ListError err;

    char endCanary;
} List;


List*     ListCtr        (unsigned int capacity, ListError* err = nullptr);
ListError ListDtr        (List* list);

int       ListInsert     (List* list, int data, unsigned int index);
int       ListAdd        (List* list, int data);
int       ListDelete     (List* list, unsigned int index);

int       ListSet        (List* list, int data, unsigned int index);
int       ListGet        (List* list, unsigned int index);

//int       ListNext       (List* list);
//int       ListPrev       (List* list);

//int       ListToBegin    (List* list);
//int       ListToEnd      (List* list);

void      ListErrPrint   (ListError err, FILE* file = nullptr);
bool      ListDump       (List* list);
bool      ListOk         (List* list);
int       _getFreeCell   (List* list);

int       SlowSlowVerySlow_ThereIsNoSenseToCallMe_ThinkHarder_LogicalIndexToPhysicalIndex (List* list, unsigned int logicalIndex);

#endif
