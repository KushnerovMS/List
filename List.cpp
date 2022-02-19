#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "List.h"

List* ListCtr (unsigned int capacity, ListError* err)
{
    if (err)
    {
        err = {};
    }
    

    List* list = (List*) calloc (1, sizeof (List));
    if (list == nullptr)
    {
        if (err) err -> allocationError = 1;
        return nullptr;
    }

    list -> data = (int*) calloc (capacity, sizeof (int));
    list -> next = (int*) calloc (capacity, sizeof (int)); 
    list -> prev = (int*) calloc (capacity, sizeof (int)); 

    if (list -> data == nullptr ||
        list -> next == nullptr ||
        list -> prev == nullptr)
    {
        if (err) err -> allocationError = 1;
        return nullptr;
    }

    for (unsigned i = 1; i < capacity; i ++)
    {
        list -> next[i] = -1;
        list -> prev[i] = -1;
    }

    list -> capacity = capacity;

    list -> beginCanary = CANARY;
    list -> endCanary = CANARY;

    return list;
}

ListError ListInsert (List* list, unsigned int index, int data)
{
    ListError err = {};
    if (!ListOk (list, &err))
        return err;

    int free = _getFreeCell (list);

    list -> data [free] = data;

    bool outOfSize = 0;
    int cell = _getCellOfItem (list, index, &outOfSize);
    
    if (!outOfSize)
    {
        list -> next[list -> prev[cell]] = free;
        list -> prev[free] = list -> prev[cell];

        list -> next[free] = cell;
        list -> prev[cell] = free;
    }
    else
    {
        list -> next[cell] = free;
        list -> prev[free] = cell;
        list -> next[free] = 0;
    }

    return err;
}

ListError ListDelite (List* list, int index)
{
    ListError err = {};
    if (!ListOk (list, &err))
        return err;

    int cell = _getCellOfItem (list, index);

    list -> next[list -> prev[cell]] = list -> next[cell]; 
    if (list -> next[cell] != 0)
        list -> prev[list -> next[cell]] = list -> prev[cell];

    list -> prev[cell] = -1;
    list -> next[cell] = -1;
    list -> data[cell] = 0;

    return err;
}

int* getItem (List* list, unsigned int index, ListError* err)
{
    if (!ListOk (list, err))
        return nullptr;

    bool outOfSize = 0;
    int cell = _getCellOfItem (list, index, &outOfSize);

    if (outOfSize)
    {
        if (err) err -> outOfSize = 1;
        return nullptr; 
    }

    return list -> data + cell;
}

ListError ListDtr (List* list)
{
    ListError err = {};
    if (!ListOk (list, &err))
    {
        return err;
    }

    memset (list -> data, 0, list -> capacity * sizeof (int));
    free (list -> data);

    memset (list -> next, 0, list -> capacity * sizeof (int));
    free (list -> next);

    memset (list -> prev, 0, list -> capacity * sizeof (int));
    free (list -> prev);

    list = {};
    free (list);

    return err;
}


bool ListOk (List* list, ListError* err)
{
    bool ok = 1;
    if (err) *err = {};

    if (list == nullptr)
    {
        if (err) err -> listNullPtr = 1;
        return 0;
    }

    if (list -> beginCanary != CANARY ||
        list -> endCanary   != CANARY)
    {
        if (err) err -> listDamaged = 1;
        return 0;
    }

    if (list -> data == nullptr)
    {
        if (err) err -> dataNullPtr = 1;
        ok = 0;
    }
    
    if (list -> next == nullptr)
    {
        if (err) err -> nextNullPtr = 1;
        ok = 0;
    }

    if (list -> prev == nullptr)
    {
        if (err) err -> prevNullPtr = 1;
        ok = 0;
    }
            
    return ok;
}

void ListErrPrint (ListError err, FILE* file)
{
    if (file == nullptr)
        file = stdout;

    bool ok = 1;
    
    if (err.allocationError)
    {
        ok = 0;
        fprintf (file, "ALLOCATION ERROR\n");
    }
    if (err.listNullPtr)
    {
        ok = 0;
        fprintf (file, "LIST NULL POINTER\n");
    }
    if (err.dataNullPtr)
    {
        ok = 0;
        fprintf (file, "DATA NULL POINTER\n");
    }
    if (err.nextNullPtr)
    {
        ok = 0;
        fprintf (file, "NEXT NULL POINTER\n");
    }
    if (err.prevNullPtr)
    {
        ok = 0;
        fprintf (file, "DATA NULL POINTER\n");
    }
    if (err.listDamaged)
    {
        ok = 0;
        fprintf (file, "LIST DAMAGED\n");
    }
    if (err.outOfSize)
    {
        ok = 0;
        fprintf (file, "OUT OF SIZE\n");
    }
    
    if (ok)
        fprintf (file, "no errors\n");
}

int _getFreeCell (List* list)
{
    unsigned int index = 0;

    while (index < list -> capacity || list -> prev[index] != -1)
        index ++;

    return index;
}

int _getCellOfItem (List* list, unsigned int index, bool* outOfSize)
{
    int cell = 0;
    *outOfSize = 0;
    for (unsigned int i = 0; i <= index; i ++)
    {
        if (list -> next[cell] == 0)
        {
            *outOfSize = 1;
            break;
        }
        cell = list -> next[cell];
    }

    return cell;
}
