#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "List.h"

List* ListCtr (int capacity, ListError* err)
{
    
    if (err)
        *err = NULL_LIST_ERROR;
    

    List* list = (List*) calloc (1, sizeof (List));
    if (list == nullptr)
    {
        if (err) err -> allocationError = 1;
        return nullptr;
    }

    list -> data = (data_t*) calloc (capacity + 1, sizeof (data_t));
    list -> next = (int*) calloc (capacity + 1, sizeof (int)); 
    list -> prev = (int*) calloc (capacity + 1, sizeof (int)); 

    if (list -> data == nullptr ||
        list -> next == nullptr ||
        list -> prev == nullptr)
    {
        if (err) err -> allocationError = 1;
        return nullptr;
    }

    for (int i = 1; i < capacity + 1; i ++)
    {
        list -> next[i] = -(i + 1);
        list -> prev[i] = -1;
    }
    list -> next[capacity] = 0;

    list -> free = 1;
    list -> capacity = capacity;
    list -> size = 0;

    list -> beginCanary = CANARY;
    list -> endCanary = CANARY;

    if (err) list -> err = NULL_LIST_ERROR;

    return list;
}

ListError ListDtr (List* list)
{
    if (!ListOk (list))
        return list  -> err;

    memset (list -> data, 0, list -> capacity * sizeof (data_t));
    free (list -> data);

    memset (list -> next, 0, list -> capacity * sizeof (int));
    free (list -> next);

    memset (list -> prev, 0, list -> capacity * sizeof (int));
    free (list -> prev);

    ListError save = list -> err;

    list -> beginCanary = 0;
    list -> data = nullptr;
    list -> next = nullptr;
    list -> prev = nullptr;
    list -> free = 0;
    list -> capacity = -1;
    list -> err = NULL_LIST_ERROR;
    list -> endCanary = -1;
    free (list);

    return save;
}


int ListInsert (List* list, data_t data, int index)
{
    if (!ListOk (list))
        return 0;

    if (list -> size == list -> capacity && _listResize (list) == 0)
        return 0;


    if (index == 0 || list -> next[index] < 0 || list -> prev[index] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    int free = list -> free;
    list -> free = - list -> next[free];
    

    list -> data [free] = data;

    list -> next[list -> prev[index]] = free;
    list -> prev[free] = list -> prev[index];

    list -> next[free] = index;
    list -> prev[index] = free;

    list -> size ++;

    return free;
}

int ListAdd (List* list, data_t data)
{
    if (!ListOk (list))
        return 0;

    if (list -> size == list -> capacity && _listResize (list) == 0)
        return 0;

    printf ("ListAdd\n");

    int free = list -> free;
    list -> free = - list -> next[list -> free];


    list -> data [free] = data;

    list -> next[list -> prev[0]] = free;
    list -> prev[free] = list -> prev[0];

    list -> next[free] = 0;
    list -> prev[0] = free;

    list -> size ++;


    return free;
}

int ListDelete (List* list, int index)
{
    if (!ListOk (list))
        return 0;

    if (_listResize (list) == 0)
        return 0;

    if (index == 0 || list -> next[index] < 0 || list -> prev[index] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    int newIndex;
    if (list -> next[index] != 0)
        newIndex = list -> next[index];
    else
        newIndex = list -> prev[index];

    list -> next[list -> prev[index]] = list -> next[index]; 
    list -> prev[list -> next[index]] = list -> prev[index];

    list -> next[index] = - list -> free;
    list -> prev[index] = -1;
    list -> data[index] = 0;

    list -> free = index;

    list -> size --;

    return newIndex;
}


int ListSet (List* list, data_t data, int index)
{
    if (!ListOk (list))
        return 0;


    if (index == 0 || list -> next[index] < 0 || list -> prev[index] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    list -> data[index] = data;

    return index;
}

data_t ListGet (List* list, int index)
{
    if (!ListOk (list))
        return 0;


    if (index == 0 || list -> next[index] < 0 || list -> prev[index] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    return list -> data[index];
}


bool ListDump (List* list)
{
    system ("mkdir -p ./dump");
    FILE* file = fopen ("./dump/test.dot", "w");

    fprintf (file, "digraph G{\n");

    fprintf (file, "graph [rankdir = LR];\
                    edge [weight = 100, minlen = 2];\n"); 

    for (int i = 0; i < list -> capacity + 1; i ++)
    {
        fprintf (file, "subgraph cluster%d                          \
                        {                                           \
                            style = \"filled, rounded\";            \
                            fillcolor = \"#b0ffb0\";                \
                            color = \"#b0ffb0\";                    \
                                                                    \
                            label = \"%d\"                          \
                                                                    \
                            item%d                                  \
                            [                                       \
                                style = \"rounded, filled\",        \
                                fillcolor = \"#ffffff\",            \
                                shape = \"record\",                 \
                                label = \"<addr> addr: %d |         \
                                          <next> next: %d |         \
                                          <prev> prev: %d |         \
                                          <data> data: %d\"         \
                            ];                                      \
                        }\n", i, i, i, i, list -> next[i], list -> prev[i], list -> data[i]);
        if (i + 1 < list -> capacity + 1)
            fprintf (file, "item%d -> item%d [style = invis];\n", i, i + 1);
    }

    fprintf (file, "edge [weight = 1, minlen = 0];\n");

    for (int i = 0; i < list -> capacity + 1; i ++)
    {
        if (list -> next[i] >= 0)
            fprintf (file, "item%d : next -> item%d : addr;\n", i, list -> next[i]);
        else
            fprintf (file, "item%d : next -> item%d : addr;\n", i, - list -> next[i]);

        if (list -> prev[i] >= 0)
            fprintf (file, "item%d : prev -> item%d : addr;\n", i, list -> prev[i]);
    }

    fprintf (file, "free -> item%d\n", list -> free);

    fprintf (file, "}\n");

    fclose (file);

    system ("dot ./dump/test.dot -T svg -o ./dump/test.svg");


    return 1;
}

bool ListOk (List* list)
{
    bool ok = 1;

    assert (list);

    if (list -> beginCanary != CANARY ||
        list -> endCanary   != CANARY)
    {
        list -> err.listDamaged = 1;
        return 0;
    }

    if (list -> data == nullptr)
    {
        list -> err.dataNullPtr = 1;
        ok = 0;
    }
    
    if (list -> next == nullptr)
    {
        list -> err.nextNullPtr = 1;
        ok = 0;
    }

    if (list -> prev == nullptr)
    {
        list -> err.prevNullPtr = 1;
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
    if (err.emptyCell)
    {
        ok = 0;
        fprintf (file, "EMPTY SELL\n");
    }
    if (err.maxSizeReached)
    {
        ok = 0;
        fprintf (file, "MAX SIZE REACHED\n");
    }
    
    
    if (ok)
        fprintf (stdout, "no errors\n");
}

int _listResize (List* list)
{
    int newCapacity = list -> capacity;

    printf ("%i ", newCapacity);

    if (list -> size >= list -> capacity)
    {
        if (list -> capacity >= MAX_CAPACITY)
        {
            list -> err.maxSizeReached = 1;
            return 0;
        }

        if (newCapacity == 1)
            newCapacity = 2;
        newCapacity *= 2 * (1 - 0.5 * newCapacity / MAX_CAPACITY);

        if (newCapacity == list -> capacity)
            newCapacity = MAX_CAPACITY;
    }
    else
    {
        while (list -> size < newCapacity / 3)
            newCapacity = newCapacity * 2 / 3;
    }

    printf ("%i ", newCapacity);
    if (list -> capacity != newCapacity)
    {
        list -> data = (data_t*) realloc (list -> data, (newCapacity + 1) * sizeof (data_t));
        list -> next = (int*) realloc (list -> next, (newCapacity + 1) * sizeof (int));
        list -> prev = (int*) realloc (list -> prev, (newCapacity + 1) * sizeof (int));

        if (list -> data == nullptr ||
            list -> next == nullptr ||
            list -> prev == nullptr)
        {
            list -> err.allocationError = 1;
            return 0;
        }

        for (int i = list -> capacity + 1; i < newCapacity + 1; i ++)
        {
            list -> next[i] = -(i + 1);
            list -> prev[i] = -1;
            list -> data[i] = 0;
        }
        list -> next[newCapacity] = 0;

        list -> free = list -> capacity + 1;


        list -> capacity = newCapacity;
    }

    printf ("%i\n", newCapacity);

    return newCapacity;
}

int SlowSlowVerySlow_ThereIsNoSenseToCallMe_ThinkHarder_LogicalIndexToPhysicalIndex (List* list, unsigned int logicalIndex)
{
    if (!ListOk (list))
        return 0;

    int physicalIndex = list -> next[0];

    while (logicalIndex -- > 0 || list -> next[physicalIndex] != 0)
        physicalIndex = list -> next[physicalIndex];

    return physicalIndex;
}
