#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef int data_t;

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

    data_t* data;
    int* next;
    int* prev;

    int free;

    int capacity;
    int size;

//    unsigned int curentIndex;

    ListError err;

    char endCanary;
} List;


List*     ListCtr        (int capacity, ListError* err = nullptr);
ListError ListDtr        (List* list);

int       ListInsert     (List* list, data_t data, int index);
int       ListAdd        (List* list, data_t data);
int       ListDelete     (List* list, int index);

int       ListSet        (List* list, data_t data, int index);
data_t    ListGet        (List* list, int index);


void      ListErrPrint   (ListError err, FILE* file = nullptr);
bool      ListDump       (List* list);
bool      ListOk         (List* list);

int       _listResize    (List* list);

int       SlowSlowVerySlow_ThereIsNoSenseToCallMe_ThinkHarder_LogicalIndexToPhysicalIndex (List* list, unsigned int logicalIndex);

#endif
