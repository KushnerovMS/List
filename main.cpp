#include <stdio.h>

#include "List.h"

int main ()
{
    ListError err = {};
    List* list = ListCtr (100, &err);
    ListErrPrint (list -> err);
    

    ListAdd (list, 10);
    ListErrPrint (list -> err);
    ListAdd (list, 20);
    ListAdd (list, 30);
    ListAdd (list, 40);
    ListAdd (list, 50);

    ListErrPrint (list -> err);

    ListInsert (list, 15, 2);
    ListDelete (list, 4);

    ListErrPrint (list -> err);


    ListDump (list);


    err = ListDtr (list);
    ListErrPrint (err);

    return 0;
}
