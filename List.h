#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

const int MAX_CAPACITY = 1000;

const char CANARY = 21;

typedef struct
{
    unsigned int allocationError    : 1;
    unsigned int dataNullPtr        : 1;
    unsigned int nextNullPtr        : 1;
    unsigned int prevNullPtr        : 1;
    unsigned int listDamaged        : 1;
    unsigned int emptyCell          : 1;
    unsigned int maxSizeReached     : 1;
} ListError;

const ListError NULL_LIST_ERROR = {};

typedef struct
{
    char beginCanary;

    char* data;
    int* next;
    int* prev;

    int free;

    int capacity;
    int itemSize;
    int size;

//    unsigned int curentIndex;

    ListError err;

    char endCanary;
} List;


List*     ListCtr        (int capacity, int itemSize, ListError* err = nullptr);
ListError ListDtr        (List* list);

int       ListInsert     (List* list, const void* data, int index);
int       ListAdd        (List* list, const void* data);
int       ListDelete     (List* list, int index);

int       ListSet        (List* list, const void* data, int index);
void*     ListGet        (List* list, int index);

int       ListFindItem   (List* list, const void* key, int (*cmp) (const void* val, const void* key));


void      ListErrPrint   (ListError err, FILE* file = nullptr);
//bool      ListDump       (List* list, void (*dump) (FILE* file, const void* item));
bool      ListDump       (List* list, void (*dump) (FILE* file, const void* item), const char* fileName = nullptr);
bool      ListOk         (List* list);

int       _listResize    (List* list);

int       SlowSlowVerySlow_ThereIsNoSenseToCallMe_ThinkHarder_LogicalIndexToPhysicalIndex (List* list, unsigned int logicalIndex);

#endif
