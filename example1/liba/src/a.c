#include <a.h>
#include <mockc.h>

int AddNumbersA( int a, int b )
{
    return add( a, b );
}

const char* PrintNameA()
{
    return name();
}