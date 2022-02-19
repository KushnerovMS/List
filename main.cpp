#include <stdio.h>

#include "List.h"

int main ()
{
    ListError err = {};
    List* list = ListCtr (100, &err);
    ListErrPrint (err);

    err = ListDtr (list);
    ListErrPrint (err);
    return 0;
}
