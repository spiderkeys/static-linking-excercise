#include <a.h>
#include <b.h>

#include <stdio.h>
#include <string.h>

int main()
{
    int sum = AddNumbersA( 1, 2 );
    printf( "liba sum: %d\n", sum );

    sum = AddNumbersB( 1, 2 );
    printf( "libb sum: %d\n", sum );

    printf( "liba name: %s\n", PrintNameA() );
    printf( "libb name: %s\n", PrintNameB() );
}