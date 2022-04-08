#include <stdio.h>

#include "List.h"
#include "./lib/GraphDump.h"


int mycmp (const void* a, const void* b)
{
    return *((const int*)a) == *((const int*)b);
}


void intDump (FILE* file, const void* a)
{
    Graph* graph = GraphOpen ();
    GraphNode node = {};
    node.id = a;
    node.color = 0xe0e0e0;

    GraphAddNode (graph, &node, "%d", *((const int*) a));

    char buff [100] = {};
    sprintf (buff, "%p.svg", a);
    GraphDraw (graph, buff, "svg");
}

int main ()
{
    ListError err = {};
    List* list = ListCtr (1, sizeof (int), &err);
    ListErrPrint (list -> err);
    
    printf ("%d\n", list -> free);

    int data = 10;
    ListAdd (list, (char*)&data);
    ListErrPrint (list -> err);
    data = 20;
    ListAdd (list, (char*)&data);
    data = 30;
    ListAdd (list, (char*)&data);


    data = 40;
    ListAdd (list, (char*)&data);
    data = 50;
    ListAdd (list, (char*)&data);

    ListErrPrint (list -> err);

    data = 15;
    ListInsert (list, (char*)&data, 2);
    ListDelete (list, 4);

    ListErrPrint (list -> err);

    ListDump (list, intDump, "testDump.svg");

    int key = 15;
    printf ("%d\n", ListFindItem (list, &key, mycmp));

    //ListFindItem (list, *mycmp ());

    err = ListDtr (list);
    ListErrPrint (err);

    return 0;
}
