#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

const char CANARY = 21;

typedef struct
{
    char beginCanary;

    int* data;
    int* next;
    int* prev;

    unsigned int capacity;

    char endCanary;
} List;

typedef struct
{
    unsigned int allocationError : 1; 
    unsigned int listNullPtr     : 1;
    unsigned int dataNullPtr     : 1;
    unsigned int nextNullPtr     : 1;
    unsigned int prevNullPtr     : 1;
    unsigned int listDamaged     : 1;
    unsigned int outOfSize       : 1;
} ListError;

List*     ListCtr    (unsigned int capacity, ListError* err = nullptr);
ListError ListInsert (List* list, unsigned int index, int data);
ListError ListDelete (List* list, unsigned int index);
ListError ListDtr    (List* list);
void      ListErrPrint (ListError err, FILE* file = nullptr);
int*      getItem    (List* list, unsigned int index, ListError* err = nullptr);
bool      ListOk     (List* list, ListError* err = nullptr);
int       _getFreeCell (List* list);
int       _getCellOfItem (List* list, unsigned int index, bool* outOfSize = nullptr);

#endif
