#include "b.h"
#include <mockc.h>

int AddNumbersB( int a, int b )
{
    return add( a, b );
}

const char* PrintNameB()
{
    return name();
}