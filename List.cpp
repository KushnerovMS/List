#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "List.h"

List* ListCtr (unsigned int capacity, ListError* err)
{
    
    if (err)
        *err = NULL_LIST_ERROR;
    

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

    for (unsigned int i = 1; i < capacity; i ++)
    {
        list -> next[i] = -(i + 1);
        list -> prev[i] = -1;
    }
    list -> next[capacity - 1] = 0;

    list -> free = 1;
    list -> capacity = capacity;

    list -> beginCanary = CANARY;
    list -> endCanary = CANARY;

    if (err) list -> err = NULL_LIST_ERROR;

    return list;
}

ListError ListDtr (List* list)
{
    if (!ListOk (list))
        return list  -> err;

    memset (list -> data, 0, list -> capacity * sizeof (int));
    free (list -> data);

    memset (list -> next, 0, list -> capacity * sizeof (int));
    free (list -> next);

    memset (list -> prev, 0, list -> capacity * sizeof (int));
    free (list -> prev);


    list -> beginCanary = 0;
    list -> data = nullptr;
    list -> next = nullptr;
    list -> prev = nullptr;
    list -> free = 0;
    list -> capacity = 0;
    *((char*)&list -> err) = 0;
    list -> endCanary = 0;
    free (list);

    return list -> err;
}


int ListInsert (List* list, int data, unsigned int index)
{
    if (!ListOk (list))
        return 0;


    //if (index == 0) index = list -> curentIndex;

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

    return free;
}

int ListAdd (List* list, int data)
{
    if (!ListOk (list))
        return 0;


    int free = list -> free;
    list -> free = - list -> next[list -> free];


    list -> data [free] = data;

    list -> next[list -> prev[0]] = free;
    list -> prev[free] = list -> prev[0];

    list -> next[free] = 0;
    list -> prev[0] = free;

    return free;

}

int ListDelete (List* list, unsigned int index)
{
    if (!ListOk (list))
        return 0;

/*
    if (index == 0) index = list -> curentIndex;
    if (list -> curentIndex == 0)
        return 0;
*/
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
/*
    if (list -> curentIndex == index)
        list -> curentIndex = newIndex;
*/

    list -> next[list -> prev[index]] = list -> next[index]; 
    list -> prev[list -> next[index]] = list -> prev[index];

    list -> next[index] = - list -> free;
    list -> prev[index] = -1;
    list -> data[index] = 0;

    list -> free = index;

    return newIndex;
}


int ListSet (List* list, int data, unsigned int index)
{
    if (!ListOk (list))
        return 0;

    //if (index == 0) index = list -> curentIndex;

    if (index == 0 || list -> next[index] < 0 || list -> prev[index] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    list -> data[index] = data;

    return index;
}

int ListGet (List* list, unsigned int index = 0)
{
    if (!ListOk (list))
        return 0;

    //if (index == 0) index = list -> curentIndex;

    if (index == 0 || list -> next[index] < 0 || list -> prev[index] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    return list -> data[index];
}


/*
int ListNext (List* list)
{
    if (!ListOk (list))
        return 0;

    if (list -> next[list -> curentIndex] == 0)
        return 0;

    if (list -> next[list -> curentIndex] < 0 || list -> prev[list -> curentIndex] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    list -> curentIndex = list -> next[list -> curentIndex];
        
    return list -> curentIndex;
}

int ListPrev (List* list)
{
    if (!ListOk (list))
        return 0;

    if (list -> prev[list -> curentIndex] == 0)
        return 0;

    if (list -> next[list -> curentIndex] < 0 || list -> prev[list -> curentIndex] < 0)
    {
        list -> err.emptyCell = 1;
        return 0;
    }

    list -> curentIndex = list -> prev[list -> curentIndex];
        
    return list -> curentIndex;
}


int ListToBegin (List* list)
{
    if (!ListOk (list))
        return 0;
    
    list -> curentIndex = list -> next[0];

    return list -> curentIndex; 
}

int ListToEnd (List* list)
{
    if (!ListOk (list))
        return 0;
    
    list -> curentIndex = list -> prev[0];

    return list -> curentIndex; 
}
*/

bool ListDump (List* list)
{
    system ("mkdir -p ./dump");
    FILE* file = (FILE*) fopen ("./dump/test.dot", "w");

    fprintf (file, "digraph G{\n");

    fprintf (file, "graph [rankdir = LR];\
                    edge [weight = 100, minlen = 2];\n"); 

    for (int i = 0; i < list -> capacity; i ++)
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
        if (i + 1 < list -> capacity)
            fprintf (file, "item%d -> item%d [style = invis];\n", i, i + 1);
    }

    fprintf (file, "edge [weight = 1, minlen = 0];\n");

    for (int i = 0; i < list -> capacity; i ++)
    {
        if (list -> next[i] >= 0)
            fprintf (file, "item%d : next -> item%d : addr;\n", i, list -> next[i]);
        else
            fprintf (file, "item%d : next -> item%d : addr;\n", i, - list -> next[i]);

        if (list -> prev[i] >= 0)
            fprintf (file, "item%d : prev -> item%d : addr;\n", i, list -> prev[i]);
    }


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
    
    if (ok)
        fprintf (stdout, "no errors\n");
}

int _getFreeCell (List* list)
{
    unsigned int index = 0;

    while (index < list -> capacity && list -> prev[index] != -1)
    {
        index ++;
    }

    return index;
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
